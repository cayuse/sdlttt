#ifndef _SDLTTT_STATUS
#define _SDLTTT_STATUS


/*
 *  The board
*/

// Users for Players and markers
enum Marker { MT, EX, OH};

class <TttStatus>
{
private:
    /* board height/width. Doing it this way in case we make different
     * games.. All games that start with this mess should be square
    */
    static constexpr int BOARD_HW =  3;
    
    Marker board [BOARD_HW][BOARD_HW] = { MT }; // init board as empty.
    Marker next_turn = EX; // init as "its X's move"
    
    int exWins = 0; // init no wins for X
    int ohWins = 0; // init no wins for O
    
public:
    bool move(int horiz, int vert);
    Marker getCurrentPlayer();
    bool checkForWins(int &dimension, Marker &player);
    void resetBoard();
    void getScores(int &xWins, int &oWins);
    Marker getMarkerAt(int horiz, int vert);
}


#endif