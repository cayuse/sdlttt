#include "res_path.h"
#include "bondiGameInterface.h"
#include "ttt.h"
#include "othello.h"
#include "connect.h"

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

template<typename T>
using cleanup_unique_ptr = std::unique_ptr<T, void (*)(T *)>;
// an alias for an otherwise cumbersome template type:
// a unique_ptr with simple function pointer as deleter
cleanup_unique_ptr <SDL_Texture>
        loadTexture(const std::string &file, SDL_Renderer *ren);

// function declarations
void logSDLError(std::ostream &os, const std::string &msg);

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);

void renderTextureXY(SDL_Texture *tex, SDL_Renderer *ren, int hw, int x, int y);

struct Game {
  bondiGameInterface *game;
  SDL_Texture *board;
  SDL_Texture *ex;
  SDL_Texture *oh;
};


const
int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int BOARD_SIZE = 500;
const int BOARD_X = 270;
const int BOARD_Y = 70;


int main(int argc, char **argv) {
//############# SDL INIT SEQUENCE #################  
  // Main SDL Init
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    logSDLError(std::cout, "SDL_Init");
    return 1;
  }

  atexit(SDL_Quit);

  // Init SDL Image specifically for png files
  if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
    logSDLError(std::cout, "IMG_Init");
    return 1;
  }

  atexit(IMG_Quit);

  // not exactly sure how this works, its wrappng the window intit into some sort of templeted
  // pointer cleanup goodness
  cleanup_unique_ptr <SDL_Window> window(
          SDL_CreateWindow(
                  "Tic Tac Toe",
                  50, 50,
                  SCREEN_WIDTH, SCREEN_HEIGHT,
                  SDL_WINDOW_SHOWN),
          SDL_DestroyWindow);
  if (window == nullptr) {
    logSDLError(std::cout, "CreateWindow");
    return 1;
  }

  // same as above, but wrapping the creation of a renderer into a pointer cleaner upper???
  cleanup_unique_ptr <SDL_Renderer> renderer(
          SDL_CreateRenderer(
                  window.get(),
                  -1,
                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
          SDL_DestroyRenderer);
  if (renderer == nullptr) {
    logSDLError(std::cout, "CreateRenderer");
    return 1;
  }
//############# END SDL INIT SEQUENCE #################
  // LOAD ALL THE Objects and Images
  const std::string backgroundPath = getResourcePath("backgrounds");
  const std::string boardPath = getResourcePath("boards");
  const std::string piecesPath = getResourcePath("pieces");

  // background
  auto background = loadTexture(backgroundPath + "background.png", renderer.get());
  // menuScreen
  auto menuScreen = loadTexture(backgroundPath + "menu.png", renderer.get());


  //Make sure they all loaded ok
  if (background == nullptr || menuScreen == nullptr) {
    return 1;
  }

  // THERE HAS TO BE A BETTER WAY TO DO THIS!
  // create instance of ttt
  Ttt ttt;
  Game tictac;
  tictac.game = &ttt;
  auto tttBoard = loadTexture(boardPath + ttt.getBoardBG(), renderer.get());
  tictac.board = &tttBoard;
  auto tttEX = loadTexture(piecesPath + ttt.getExPiece(), renderer.get());
  tictac.ex = &tttEX;
  auto tttOH = loadTexture(piecesPath + ttt.getOhPiece(), renderer.get());
  tictac.oh = &tttOH;
  //Make sure they all loaded ok
  if (tictac.board == nullptr || tictac.ex == nullptr || tictac.oh == nullptr) {
    return 1;
  }

  // create instance of othello
  Othello oth;
  Game othello;
  othello.game = &oth;
  auto othBoard = loadTexture(boardPath + oth.getBoardBG(), renderer.get());
  othello.board = &othBoard;
  auto othEX = loadTexture(piecesPath + oth.getExPiece(), renderer.get());
  othello.ex = &othEX;
  auto othOH = loadTexture(piecesPath + oth.getOhPiece(), renderer.get());
  othello.oh = &othOH;
  if (othello.board == nullptr || othello.ex == nullptr || othello.oh == nullptr) {
    return 1;
  }

  // create instance of connect-4
  Connect con;
  Game connect;
  connect.game = &con;
  auto conBoard = loadTexture(boardPath + con.getBoardBG(), renderer.get());
  connect.board = &conBoard;
  auto conEX = loadTexture(piecesPath + con.getExPiece(), renderer.get());
  connect.ex = &conEX;
  auto conOH = loadTexture(piecesPath + con.getOhPiece(), renderer.get());
  connect.oh = &conOH;
  if (connect.board == nullptr || connect.ex == nullptr || connect.oh == nullptr) {
    return 1;
  }

  Game *currentGame;

  bool menu = true; // start with the menu system
  bool quit = false;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (menu == true) { // if the menu is on, do these things
        switch (event.type) {
          /* Look for a keypress */
          case SDL_KEYDOWN:
            /* Check the SDLKey values and move change the coords */
            switch (event.key.keysym.sym) {
              case SDLK_0:
                quit = true;
                break;
              case SDLK_1:
                menu = false;
                currentGame = &tictac;
                break;
              case SDLK_2:
                menu = false;
                currentGame = &tictac;
                break;
              case SDLK_3:
                menu = false;
                currentGame = &tictac;
                break;
              default:
                break;
            }
            break;
        }
      } else {  // if the menu is off, do these things
        switch (event.type) {
          case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
              case SDLK_ESCAPE:
                menu = true;
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
    }
    SDL_RenderClear(renderer.get());
    if (menu == true){
      renderTexture(menuScreen.get(), renderer.get(), 0, 0 );
    } else {
      renderTexture(background.get(), renderer.get(), 0, 0 );
      renderTexture(current->board.get(), renderer.get(), BOARD_X, BOARD_Y );
      renderTextureXY(current->ex.get(),    renderer.get(), current->game.getBoardHW(), 1, 1 );
      renderTextureXY(current->ex.get(),    renderer.get(), current->game.getBoardHW(), 1, 2 );
      renderTextureXY(current->oh.get(),    renderer.get(), current->game.getBoardHW(), 0, 2 );
      renderTextureXY(current->oh.get(),    renderer.get(), current->game.getBoardHW(), 0, 3 );
    }
  }
  /* Update the alien position */

/*  LEGACY CODE TO DIG THROUGH AS NEEDED
    // LOAD ALL THE Images
    // background
    const std::string backgroundPath = getResourcePath("backgrounds");
    auto background = loadTexture(backgroundPath + "background.png", renderer.get());
    // board
    const std::string boardPath = getResourcePath("boards");
    auto board = loadTexture(boardPath + tictac.getBoardBG(), renderer.get());
    // pieces
    const std::string piecesPath = getResourcePath("pieces");
    auto ex = loadTexture(piecesPath + tictac.getExPiece(), renderer.get());
    auto oh = loadTexture(piecesPath + tictac.getOhPiece(), renderer.get());
    
    //Make sure they all loaded ok
    if (background == nullptr || board == nullptr || ex == nullptr || oh == nullptr)
    {
        return 1;
    }

    //initial display stuff.. this needs to be put into a function eventually... all this does.. or better yet a class
    SDL_RenderClear(renderer.get());

    renderTexture(background.get(), renderer.get(), 0, 0 );
    renderTexture(board.get(), renderer.get(), BOARD_X, BOARD_Y );
    renderTextureXY(ex.get(),    renderer.get(), tictac.getBoardHW(), 1, 1 );
    renderTextureXY(ex.get(),    renderer.get(), tictac.getBoardHW(), 1, 2 );
    renderTextureXY(oh.get(),    renderer.get(), tictac.getBoardHW(), 0, 2 );
    renderTextureXY(oh.get(),    renderer.get(), tictac.getBoardHW(), 0, 3 );


    SDL_RenderPresent(renderer.get());
    SDL_Delay(5000);
*/
  return 0;
}


/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message to
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void logSDLError(std::ostream &os, const std::string &msg) {
  os << msg << " error: " << SDL_GetError() << std::endl;
}

/**
* Loads an image into a texture on the rendering device
* @param file The image file to load
* @param ren The renderer to load the texture onto
* @return the loaded texture, or nullptr if something went wrong.
*/
cleanup_unique_ptr <SDL_Texture>
loadTexture(const std::string &file, SDL_Renderer *ren) {
  SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
  if (texture == nullptr) {
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
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h) {
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
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y) {
  int w, h;
  SDL_QueryTexture(tex, NULL, NULL, &w, &h);
  renderTexture(tex, ren, x, y, w, h);
}

/**
 * Draw an SDL_Texture to a cell on the board (located at x,y) preserving
 * the texture's width and height, but centered within that location
 * this method is aware of the concept of a 'board' and is zero referenced
 * i.e. call with renderTextureXY(tex, ren, 3, 0, 0) for upper left square
*/
void renderTextureXY(SDL_Texture *tex, SDL_Renderer *ren, int hw, int x, int y) {
  x++;
  y++;

  int w, h;
  SDL_QueryTexture(tex, NULL, NULL, &w, &h);

  int x_offset = w / 2;
  int y_offset = h / 2;

  int half_width = (BOARD_SIZE / hw) / 2;
  int x_cent, y_cent;

  x_cent = ((BOARD_SIZE * x) / hw) + BOARD_X - half_width - x_offset;
  y_cent = ((BOARD_SIZE * y) / hw) + BOARD_Y - half_width - y_offset;

  renderTexture(tex, ren, x_cent, y_cent, w, h);
}
