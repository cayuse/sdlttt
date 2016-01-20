#ifndef _CONNECT_
#define _CONNECT_

#include "bondiGameInterface.h"

// Class Definition for Tic Tac Toe
class Connect: public bondiGameInterface
{
private:
    /* board height/width. Doing it this way in case we make different
     * games.. All games that start with this mess should be square
     * this may change if i get ambitious
    */
    
    int exWins  = 0; // init no wins for X
    int ohWins  = 0; // init no wins for O
    int boardHW = 8; // height/width of board
    std::string boardBG = "tttboard.png";
    std::string exPiece = "tttex.png";
    std::string ohPiece = "tttoh.png";

    Marker board [boardHW][boardHW] = { {MT} }; // init board as empty.
    Marker current_player = EX; // init as "its X's move"
    
public:
    std::string getBoardBG();
    std::string getExPiece();
    std::string getOhPiece();
    int getBoardHW();
    bool move(int horiz, int vert);
    Marker getCurrentPlayer();
    bool checkForWins(Marker &player);
    bool getWinDimension(int &x1, int &y1, int &x2, int &y2);
    void resetBoard();
    void getScores(int &xWins, int &oWins);
    Marker getMarkerAt(int horiz, int vert);
};


#endif
