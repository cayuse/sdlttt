#include "tttStatus.h"

/*  bool move(int location)
 *  This method is used to place a move on the board.
 *  The move is given as an int, and references the board as such.
 *  1|2|3
 *  4|5|6
 *  7|8|9
 *
 *  if the location for the move is taken, then the method returns FALSE
 *  if the location is available, then the method returns TRUE  AND
 *  it changes the next_turn variable (to the other player)
 */
bool move(int location)
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
 *  bool = TRUE if the game has become a catsgame
 *  if catsgame or no player has won, then
 *  dimension and player will bet set to -1
 *
 *  if there has been a winner, then the dimension
 *  will be set as follows:
 *
 *  vertical 123
 *  wins     |||
 *  
 *  Horizontal  4-
 *  wins        5-
 *              6-
 *  diagnal 7 /
 *  diagnal 8 \
 *
 *  player is set to EX for X winner and OH for O Winner
 */
bool checkForWins(int &dimension, int &player)
{
    
}

/*
 *  voidReseBoard()
 *  simply resets the array and any other
 */
void resetBoard()
{
    
}

/*
 *  void getScore(int &xWins, int &oWins)
 *  just stores the current tally of wins for the
 *  players into the int references
 */

void getScore(int &xWins, int &oWins)
{
    
}