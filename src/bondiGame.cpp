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
cleanup_unique_ptr<SDL_Texture>
        loadTexture(const std::string &file, SDL_Renderer *ren);

// function declarations
void logSDLError(std::ostream &os, const std::string &msg);

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);

void renderTextureXY(SDL_Texture *tex, SDL_Renderer *ren, int hw, int x, int y);

bool getClickedCell(int hw, int mousex, int mousey, int &clickedx, int &clickedy);

struct Game {
  bondiGameInterface *game;
  cleanup_unique_ptr<SDL_Texture> board;
  cleanup_unique_ptr<SDL_Texture> ex;
  cleanup_unique_ptr<SDL_Texture> oh;

  Game(bondiGameInterface *game,
       cleanup_unique_ptr<SDL_Texture> board,
       cleanup_unique_ptr<SDL_Texture> ex,
       cleanup_unique_ptr<SDL_Texture> oh)
          : game(game),
            board(std::move(board)),
            ex(std::move(ex)),
            oh(std::move(oh)) { }
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
  cleanup_unique_ptr<SDL_Window> window(
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
  cleanup_unique_ptr<SDL_Renderer> renderer(
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
  Game tictac(&ttt,
              loadTexture(boardPath + ttt.getBoardBG(), renderer.get()),
              loadTexture(piecesPath + ttt.getExPiece(), renderer.get()),
              loadTexture(piecesPath + ttt.getOhPiece(), renderer.get()));
  if (tictac.board == nullptr || tictac.ex == nullptr || tictac.oh == nullptr) {
    return 1;
  }

  // create instance of othello
  Othello oth;
  Game othello(&oth,
               loadTexture(boardPath + oth.getBoardBG(), renderer.get()),
               loadTexture(piecesPath + oth.getExPiece(), renderer.get()),
               loadTexture(piecesPath + oth.getOhPiece(), renderer.get())
  );
  if (othello.board == nullptr || othello.ex == nullptr || othello.oh == nullptr) {
    return 1;
  }

  // create instance of connect-4
  Connect con;
  Game connect(&con,
               loadTexture(boardPath + con.getBoardBG(), renderer.get()),
               loadTexture(piecesPath + con.getExPiece(), renderer.get()),
               loadTexture(piecesPath + con.getOhPiece(), renderer.get())
  );
  if (connect.board == nullptr || connect.ex == nullptr || connect.oh == nullptr) {
    return 1;
  }

  Game *currentGame;
  SDL_Event event;
  int mouse_x = 0;
  int mouse_y = 0;
  int clicked_x = 0;
  int clicked_y = 0;

  bool menu = true; // start with the menu system
  bool quit = false;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (menu) { // if the menu is on, do these things
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
                currentGame = &othello;
                break;
              case SDLK_3:
                menu = false;
                currentGame = &connect;
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
            mouse_x = event.button.x;
            mouse_y = event.button.y;
            if ( getClickedCell(currentGame->game->getBoardHW(), mouse_x, mouse_y, clicked_x, clicked_y) )
            {
              currentGame->game->move(clicked_x, clicked_y);
            }
            break;
          default:
            break;
        }
      }
    }
    SDL_RenderClear(renderer.get());
    if (menu) {
      renderTexture(menuScreen.get(), renderer.get(), 0, 0);
    } else {
      renderTexture(background.get(), renderer.get(), 0, 0);
      renderTexture(currentGame->board.get(), renderer.get(), BOARD_X, BOARD_Y);
      renderTextureXY(currentGame->ex.get(), renderer.get(), currentGame->game->getBoardHW(), 0, 0);
      renderTextureXY(currentGame->ex.get(), renderer.get(), currentGame->game->getBoardHW(), 1, 1);
      renderTextureXY(currentGame->oh.get(), renderer.get(), currentGame->game->getBoardHW(), 0, 2);
      renderTextureXY(currentGame->oh.get(), renderer.get(), currentGame->game->getBoardHW(), 0, 2);
      renderTextureXY(currentGame->oh.get(), renderer.get(), currentGame->game->getBoardHW(), 7, 7);
      renderTextureXY(currentGame->oh.get(), renderer.get(), currentGame->game->getBoardHW(), 6, 6);
    }
    SDL_RenderPresent(renderer.get());
  }
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
cleanup_unique_ptr<SDL_Texture>
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

// returns false if it didn't get a valid square
bool getClickedCell(int hw, int mousex, int mousey, int &clickedx, int &clickedy)
{
  clickedx = -1;
  clickedy = -1;
  for (int i = 0; i < hw; i++) {
    int cellLeft  = BOARD_X + (BOARD_SIZE * i) / hw;
    int cellRight = BOARD_X + (BOARD_SIZE * (i + 1) ) / hw;
    if (mousex > cellLeft && mousex < cellRight) {
      clickedx = i;
    }
  }
  for (int i = 0; i < hw; i++) {
    int cellLeft  = BOARD_Y + (BOARD_SIZE * i) / hw;
    int cellRight = BOARD_Y + (BOARD_SIZE * (i + 1) ) / hw;
    if (mousey > cellLeft && mousey < cellRight) {
      clickedy = i;
    }
  } 

  if (clickedx >= 0 && clickedy >= 0){
    return true;
  } else {
    return false;
  }
}
