#pragma once
#include "GoldGeneral.h"
class Pawn :
    public GoldGeneral
{
protected:
    bool IsPromoted = 0;
public:
    Pawn(int, int, Color, Board*);
    void draw();
    void drawDead(int, int, int);
    bool getIsPromoted();
    void setIsPromoted(bool);
    bool IsLegalMove(Board* B, int sr, int sc, int dr, int dc);
    bool IsLegalDrop(Board* B, int dr, int dc);
};

