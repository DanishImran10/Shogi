#pragma once
#include "Piece.h"
#include "DeadPieces.h"
class GoldGeneral :
    public Piece
{
protected:
    bool IsPromoted = 1;
public:
    GoldGeneral(int, int, Color, Board*);
    virtual void draw();
    virtual void drawDead(int, int, int);
    virtual bool getIsPromoted();
    virtual void setIsPromoted(bool);
    virtual bool IsLegalMove(Board* B, int sr, int sc, int dr, int dc);
    virtual bool IsLegalDrop(Board* B, int dr, int dc);
};

