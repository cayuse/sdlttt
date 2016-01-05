#include "res_path.h"
#include "tttStatus.h"

#ifdef USING_OSX_FRAMEWORKS
#   include <SDL2/SDL.h>
#   include <SDL2_image/SDL_image.h>
#else
#   include <SDL.h>
#   include <SDL_image.h>
#endif

#include <iostream>
#include <memory>
#include <string>

// an alias for an otherwise cumbersome template type:
// a unique_ptr with simple function pointer as deleter
template<typename T>
using cleanup_unique_ptr = std::unique_ptr<T, void (*)(T *)>;

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;
const int TILE_SIZE = 40;


/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message to
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void logSDLError(std::ostream &os, const std::string &msg){
    os << msg << " error: " << SDL_GetError() << std::endl;
}

/**
* Loads an image into a texture on the rendering device
* @param file The image file to load
* @param ren The renderer to load the texture onto
* @return the loaded texture, or nullptr if something went wrong.
*/
cleanup_unique_ptr<SDL_Texture>
loadTexture(const std::string &file, SDL_Renderer *ren){
    SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
    if (texture == nullptr)
    {
        logSDLError(std::cout, "LoadTexture");
    }
    return cleanup_unique_ptr<SDL_Texture>(texture, SDL_DestroyTexture);
}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired
* width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
* @param w The width of the texture to draw
* @param h The height of the texture to draw
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
    //Setup the destination rectangle to be at the position we want
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
    int w, h;
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    renderTexture(tex, ren, x, y, w, h);
}

int main(int argc, char **argv){
    
    // Main SDL Init
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        logSDLError(std::cout, "SDL_Init");
        return 1;
    }
    
    atexit(SDL_Quit);

    // Init SDL Image specifically for png files
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        logSDLError(std::cout, "IMG_Init");
        return 1;
    }
    
    atexit(IMG_Quit);
    
    // not exactly sure how this works, its wrappng the window intit into some sort of templeted
    // pointer cleanup goodness
    cleanup_unique_ptr<SDL_Window> window(
        SDL_CreateWindow(
            "Tic Tac Toe",
            100, 100,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN),
        SDL_DestroyWindow);
    if (window == nullptr)
    {
        logSDLError(std::cout, "CreateWindow");
        return 1;
    }
    
    // same as above, but wrapping the creation of a renderer into a pointer cleaner upper???
    cleanup_unique_ptr<SDL_Renderer> renderer(
        SDL_CreateRenderer(
            window.get(),
            -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
        SDL_DestroyRenderer);
    if (renderer == nullptr)
    {
        logSDLError(std::cout, "CreateRenderer");
        return 1;
    }

    /*
    const std::string resPath = getResourcePath("Lesson2");
    SDL_Texture *background = loadTexture(resPath + "background.bmp", renderer);
    SDL_Texture *image = loadTexture(resPath + "image.bmp", renderer);
    if (background == nullptr || image == nullptr){
     cleanup(background, image, renderer, window);
     return 1;
}
    */

    const std::string resPath = getResourcePath("Lesson3");
    auto background = loadTexture(resPath + "background.png", renderer.get());
    auto image = loadTexture(resPath + "image.png", renderer.get());
    //Make sure they both loaded ok
    if (background == nullptr || image == nullptr)
    {
        return 1;
    }

    SDL_RenderClear(renderer.get());

    int bW, bH;
    SDL_QueryTexture(background.get(), NULL, NULL, &bW, &bH);
    renderTexture(background.get(), renderer.get(), 0, 0);
    renderTexture(background.get(), renderer.get(), bW, 0);
    renderTexture(background.get(), renderer.get(), 0, bH);
    renderTexture(background.get(), renderer.get(), bW, bH);

    int iW, iH;
    SDL_QueryTexture(image.get(), NULL, NULL, &iW, &iH);
    int x = SCREEN_WIDTH / 2 - iW / 2;
    int y = SCREEN_HEIGHT / 2 - iH / 2;
    renderTexture(image.get(), renderer.get(), x, y);

    SDL_RenderPresent(renderer.get());
    SDL_Delay(1000);

    //Determine how many tiles we'll need to fill the screen
    int xTiles = SCREEN_WIDTH / TILE_SIZE;
    int yTiles = SCREEN_HEIGHT / TILE_SIZE;

    //Draw the tiles by calculating their positions
    for (int i = 0; i < xTiles * yTiles; ++i)
    {
        int x = i % xTiles;
        int y = i / xTiles;
        renderTexture(background.get(), renderer.get(), x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE,
                      TILE_SIZE);
    }

    SDL_QueryTexture(image.get(), NULL, NULL, &iW, &iH);
    x = SCREEN_WIDTH / 2 - iW / 2;
    y = SCREEN_HEIGHT / 2 - iH / 2;
    renderTexture(image.get(), renderer.get(), x, y);



    SDL_RenderPresent(renderer.get());
    SDL_Delay(2000);

    /*
    SDL_Event e;
    bool quit = false;
    while (!quit){
     while (SDL_PollEvent(&e)){
      if (e.type == SDL_QUIT){
       quit = true;
      }
      if (e.type == SDL_KEYDOWN){
       quit = true;
      }
      if (e.type == SDL_MOUSEBUTTONDOWN){
       quit = true;
      }
     }
     //Render the scene
     SDL_RenderClear(renderer);
     renderTexture(image, renderer, x, y);
     SDL_RenderPresent(renderer);
}
    */
    SDL_Event event;
    int alien_x = x;
    int alien_y = y;
    int alien_xvel, alien_yvel = 0;
    bool quit = false;
    while (!quit)
    {
        while( SDL_PollEvent( &event ) )
        {
            switch( event.type )
            {
                /* Look for a keypress */
            case SDL_KEYDOWN:
                /* Check the SDLKey values and move change the coords */
                switch( event.key.keysym.sym )
                {
                case SDLK_LEFT:
                    alien_xvel = -1;
                    break;
                case SDLK_RIGHT:
                    alien_xvel =  1;
                    break;
                case SDLK_UP:
                    alien_yvel = -1;
                    break;
                case SDLK_DOWN:
                    alien_yvel =  1;
                    break;
                default:
                    break;
                }
                break;
                /* We must also use the SDL_KEYUP events to zero the x */
                /* and y velocity variables. But we must also be       */
                /* careful not to zero the velocities when we shouldn't*/
            case SDL_KEYUP:
                switch( event.key.keysym.sym )
                {
                case SDLK_LEFT:
                    /* We check to make sure the alien is moving */
                    /* to the left. If it is then we zero the    */
                    /* velocity. If the alien is moving to the   */
                    /* right then the right key is still press   */
                    /* so we don't tocuh the velocity            */
                    if( alien_xvel < 0 )
                        alien_xvel = 0;
                    break;
                case SDLK_RIGHT:
                    if( alien_xvel > 0 )
                        alien_xvel = 0;
                    break;
                case SDLK_UP:
                    if( alien_yvel < 0 )
                        alien_yvel = 0;
                    break;
                case SDLK_DOWN:
                    if( alien_yvel > 0 )
                        alien_yvel = 0;
                    break;
                default:
                    break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                quit = true;
                break;
            default:
                break;
            }


        }
        alien_x += alien_xvel;
        alien_y += alien_yvel;

        SDL_RenderClear(renderer.get());
        renderTexture(image.get(), renderer.get(), alien_x, alien_y);
        SDL_RenderPresent(renderer.get());
    }
}