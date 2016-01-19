#ifndef _BONDIGAMEINTERFACE_
#define _BONDIGAMEINTERFACE_

enum Marker { MT, EX, OH};

class bondiGameInterface
{
public:
    virtual string getBoard() = 0;
    virtual string getExPiece() = 0;
    virtual string getOhPiece() = 0;
    virtual bool move(int horiz, int vert) = 0;
    virtual Marker getCurrentPlayer() = 0;
    virtual bool checkForWins(Marker &player) = 0;
    virtual bool getWinDimension(int &x1, int &y1, int &x2, int &y2) = 0;
    virtual void resetBoard() = 0;
    virtual void getScores(int &xWins, int &oWins) = 0;
    virtual Marker getMarkerAt(int horiz, int vert) = 0;
};


#endif