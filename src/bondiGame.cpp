#include "res_path.h"
#include "bondiGameInterface.h"
#include "ttt.h"

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

const int SCREEN_WIDTH  = 800;
const int SCREEN_HEIGHT = 600;

const int BOARD_SIZE = 500;
const int BOARD_X = 270;
const int BOARD_Y = 70;

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

/**
 * Draw an SDL_Texture to a cell on the board (located at x,y) preserving
 * the texture's width and height, but centered within that location
 * this method is aware of the concept of a 'board' and is zero referenced
 * i.e. call with renderTextureXY(tex, ren, 0, 0) for upper left square
*/
void renderTextureXY(SDL_Texture *tex, SDL_Renderer *ren, int x, int y)
{
    x++;
    y++;
    
    int w, h;
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    
    int x_offset = w / 2;
    int y_offset = h / 2;
    
    int half_width = (BOARD_SIZE / BOARD_HW) / 2;
    int x_cent, y_cent;
    
    x_cent = ( (BOARD_SIZE * x) / BOARD_HW ) + BOARD_X - half_width - x_offset;
    y_cent = ( (BOARD_SIZE * y) / BOARD_HW ) + BOARD_Y - half_width - y_offset;

    renderTexture(tex, ren, x_cent, y_cent, w, h);
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
            50, 50,
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
    
    // LOAD ALL THE Images
    // background
    const std::string backgroundPath = getResourcePath("backgrounds");
    auto background = loadTexture(backgroundPath + "background.png", renderer.get());
    // board
    const std::string boardPath = getResourcePath("boards");
    auto board = loadTexture(boardPath + "board.png", renderer.get());
    // pieces
    const std::string piecesPath = getResourcePath("pieces");
    auto ex = loadTexture(piecesPath + "ex.png", renderer.get());
    auto oh = loadTexture(piecesPath + "oh.png", renderer.get());
    
    //Make sure they all loaded ok
    if (background == nullptr || board == nullptr || ex == nullptr || oh == nullptr)
    {
        return 1;
    }

    //initial display stuff.. this needs to be put into a function eventually... all this does.. or better yet a class
    SDL_RenderClear(renderer.get());

    renderTexture(background.get(), renderer.get(), 0, 0 );
    renderTexture(board.get(), renderer.get(), BOARD_X, BOARD_Y );
    renderTextureXY(ex.get(),    renderer.get(), 1, 1 );
    renderTextureXY(ex.get(),    renderer.get(), 1, 2 );
    renderTextureXY(oh.get(),    renderer.get(), 2, 2 );
    renderTextureXY(oh.get(),    renderer.get(), 3, 3 );


    SDL_RenderPresent(renderer.get());
    SDL_Delay(5000);

    return 1;
}
