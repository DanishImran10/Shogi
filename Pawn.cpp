#include "Pawn.h"
#include <iostream>
using namespace std;
Pawn::Pawn(int _r, int _c, Color _C, Board* _B)
	:GoldGeneral(_r, _c, _C, _B)
{

}

void Pawn::draw()
{
	if (this->C == black)
	{
		setfillstyle(SOLID_FILL, BLUE);
		setcolor(BLUE);
	}
	else
	{
		setfillstyle(SOLID_FILL, RED);
		setcolor(RED);
	}
	fillellipse(ci * CDIM + CDIM / 2, ri * RDIM + RDIM / 2, 10, 10);
	circle(ci * CDIM + CDIM / 2, ri * RDIM + RDIM / 2, 10);
	
	if (this->IsPromoted)
	{
		setcolor(GREEN);
		bgiout << 'P';
		outstreamxy(ci * CDIM + 5, ri * RDIM + 3);
	}
}

void Pawn::drawDead(int r, int c, int nods)
{
	setcolor(WHITE);
	bgiout << 'x' << nods;
	outstreamxy(c * CDIM + 2, r * RDIM + 2);
	if (this->C == white)
	{
		setfillstyle(SOLID_FILL, RED);
		setcolor(RED);
	}
	else
	{
		setfillstyle(SOLID_FILL, BLUE);
		setcolor(BLUE);
	}
	fillellipse(c * CDIM + CDIM / 2, r * RDIM + RDIM / 2, 10, 10);
	circle(c * CDIM + CDIM / 2, r * RDIM + RDIM / 2, 10);
}

bool Pawn::getIsPromoted()
{
	return this->IsPromoted;
}
void Pawn::setIsPromoted(bool p)
{
	this->IsPromoted = p;
}

bool Pawn::IsLegalMove(Board* B, int sr, int sc, int dr, int dc)
{
	if (!this->IsPromoted)
	{
		int adr = sr - dr;
		return (IsVerticalMove(sr, sc, dr, dc) && IsVerticalPathClear(B, sr, sc, dr, dc)
			&& (this->C == white && adr == 1 || this->C == black && adr == -1));
	}
	else
		return GoldGeneral::IsLegalMove(B, sr, sc, dr, dc);
}

bool Pawn::IsLegalDrop(Board* B, int dr, int dc)
{
	return B->getCs(dr, dc)->getPs() == NULL && ((this->C == white && dr >= 1) || (this->C == black && dr <= 7));
}
