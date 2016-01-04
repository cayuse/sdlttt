#include "tttStatus.h"

/*  bool move(int location)
 *  This method is used to place a move on the board.
 *  The move is given as a pair of ints, and references the board as such.
 *  [0][0] | [0][1] | [0][2]
 * --------|--------|--------
 *  [1][0] | [1][1] | [1][2]
 * --------|--------|--------
 *  [2][0] | [2][1] | [2][2]
 *
 *  (if) the location for the move is taken, then the method returns FALSE
 *  
 *  (if) the location is available, then the method returns TRUE  _AND_
 *  it puts the current player's "token" on that spot _AND_
 *  it changes the next_turn variable (to the other player)
 */
bool move(int horiz, int vert)
{
    
}

/*
 *  int getNextTurn()
 *  this method simply returns a value stating whose turn it is:
 *  EX for X || OH for O
 */

int getNextTurn()
{
    
}

/*
 *  bool checkForWins(int &dimension, int &player)
 *  this method checks the status of the game board and
 *  returns information based on that status.
 *
 *  (if) the game has become a catsgame then bool = TRUE 
 *  (if) catsgame or no player has won, then
 *  dimension and player will bet set to MT
 *
 *  (if) there has been a winner, then the dimension
 *  will be set as follows:
 *
 *  Vertical 012
 *  wins     |||
 *  
 *  Horizontal  3-
 *  wins        4-
 *              5-
 *  diagnal 6 /
 *  diagnal 7 \
 *
 *  player is set to EX for X winner and OH for O Winner
 */
bool checkForWins(int &dimension, int &player)
{
    
}

/*
 *  voidReseBoard()
 *  checks for a winner and if there was one, increments
 *  that player's total wins _THEN_
 *
 *  resets the gamestate to initial settings
 */
void resetBoard()
{
    
}

/*
 *  void getScores(int &xWins, int &oWins)
 *  just stores the current tally of wins for the
 *  players into the int references
 */

void getScores(int &xWins, int &oWins)
{
    
}

/*
 *  void getBoardStatus(int array[][3])
 *
 *  this will essentially copy the master array status into a 'passed-in' array
 *  so that the rendering system can draw it quicker.
 *
 *  to be clear, the array will be expected in this order relative to the board
 *
 *  [0][0] | [0][1] | [0][2]
 * --------|--------|--------
 *  [1][0] | [1][1] | [1][2]
 * --------|--------|--------
 *  [2][0] | [2][1] | [2][2]
 *
 */
void getBoardStatus(int array[][BOARD_HW])
{
    
}