#ifndef _SDLTTT_STATUS
#define _SDLTTT_STATUS


/*
 *  The board 
 *
 *
 */

class <TttStatus>
{
private:
    static constexpr int EX =  1; // X
    static constexpr int OH =  2; // O
    static constexpr int MT = -1; // Empty
    // Use these in code in case they need to change later
    
    int board [3][3] = { MT };
    int next_turn = EX;
    
    int exWins = 0;
    int ohWins = 0;
    
public:
    bool move(int location);
    int getNextTurn();
    bool checkForWins(int &dimension, int &player);
    void resetBoard();
    void getScore(int &xWins, int &oWins);
}


#endif