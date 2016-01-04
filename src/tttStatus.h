#ifndef _SDLTTT_STATUS
#define _SDLTTT_STATUS


/*
 *  The board
*/

class <TttStatus>
{
private:
    /* board height/width. Doing it this way in case we make different
     * games.. All games that start with this mess should be square
    */
    static constexpr int BOARD_HW =  3;
    
    /* the "players" are just ints, but use these
     * in code in case we change that later
    */
    static constexpr int EX       =  1; // X
    static constexpr int OH       =  2; // O
    static constexpr int MT       = -1; // Empty
    
    int board [BOARD_HW][BOARD_HW] = { MT }; // init board as empty.
    int next_turn = EX; // init as "its X's move"
    
    int exWins = 0; // init no wins for X
    int ohWins = 0; // init no wins for O
    
public:
    bool move(int horiz, int vert);
    int  getNextTurn();
    bool checkForWins(int &dimension, int &player);
    void resetBoard();
    void getScore(int &xWins, int &oWins);
    void getBoardStatus(int array[][3]);
}


#endif