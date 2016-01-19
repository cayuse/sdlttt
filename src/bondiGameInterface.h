
enum Marker { MT, EX, OH};

class bondiGameInterface
{
public:
    virtual bool move(int horiz, int vert) = 0;
    virtual Marker getCurrentPlayer() = 0;
    virtual bool checkForWins(Marker &player) = 0;
    virtual bool getWinDimension(int &x1, int &y1, int &x2, int &y2) = 0;
    virtual void resetBoard() = 0;
    virtual void getScores(int &xWins, int &oWins) = 0;
    virtual Marker getMarkerAt(int horiz, int vert) = 0;
};