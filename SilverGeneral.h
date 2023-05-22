#pragma once
#include "GoldGeneral.h"
class SilverGeneral :
    public GoldGeneral
{
protected:
    bool IsPromoted = 0;
public:
    SilverGeneral(int, int, Color, Board*);
    void draw();
    void drawDead(int, int, int);
    bool getIsPromoted();
    void setIsPromoted(bool);
    bool IsLegalMove(Board* B, int sr, int sc, int dr, int dc);
    bool IsLegalDrop(Board* B, int dr, int dc);
};

