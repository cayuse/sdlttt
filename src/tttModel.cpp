#include "tttModel.h"

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
 *  it changes the current_player variable (to the other player)
 */
bool move(int horiz, int vert)
{
    
}

/*
 *  Marker getCurrentPlayer()
 *  this method simply returns a Marker corresponding to whose turn it is:
 *  EX for X || OH for O
 */
Marker getCurrentPlayer()
{
    
}

/*
 *  bool checkForWins(int &dimension, Marker &player)
 *  this method checks the status of the game board and
 *  returns information based on that status.
 *
 *  (if) the game has become a catsgame then return bool = TRUE 
 *  (if) catsgame or no player has won, then
 *       player will bet set to MT
 *
 *  (if) there has been a winner:
 *       player is set to EX for X winner and OH for O Winner
 */
bool checkForWins(Marker &player)
{
    
}

/*
 * bool getWinDimension(int &x1, int &y1, int &x2, int &y2)
 * this method sets x1,y1 and x2,y2 to mark the location of
 * the win.
 * in ttt that might be (0,0),(3,3) or (0,0),(0,3) whatever
 * it will be slightly more interesting in something like
 * connect 4 where it can be any 4 anywhere in the field.
 * 
 * (if) this is an appropriate way to score a win set return true
 *      and a line will be procedurarly drawn over the covered area
 *      
 * (if) this is not appropriate and the board should not be marked
 *      return false (e.g. othello)
*/
bool getWinDimension(int &x1, int &y1, int &x2, int &y2)
{
  
}

/*
 *  void resetBoard()
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
 *  Marker getMarkerAt(int horiz, int vert)
 *  this will return the marker at the specified location
 *  it should match the contents of the array.
 *
 *  [0][0] | [0][1] | [0][2]
 * --------|--------|--------
 *  [1][0] | [1][1] | [1][2]
 * --------|--------|--------
 *  [2][0] | [2][1] | [2][2]
 *
 */
Marker getMarkerAt(int horiz, int vert)
{
    
}