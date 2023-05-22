#include "Bishop.h"
#include <iostream>
using namespace std;

Bishop::Bishop(int _r, int _c, Color _C, Board* _B)
	:King(_r, _c, _C, _B)
{

}

void Bishop::draw()
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
	fillellipse(ci * CDIM + CDIM / 2, ri * RDIM + 20, 10, 10);
	circle(ci * CDIM + CDIM / 2, ri * RDIM + 20, 10);
	bar((ci * CDIM) + (CDIM / 2) - 3, (ri * RDIM) + 15, (ci * CDIM) + (CDIM / 2) + 3, (ri * RDIM) + RDIM - 15);
	if (this->IsPromoted)
	{
		setcolor(GREEN);
		bgiout << 'P';
		outstreamxy(ci * CDIM + 5, ri * RDIM + 3);
	}
}

bool Bishop::getIsPromoted()
{
	return this->IsPromoted;
}

void Bishop::setIsPromoted(bool p)
{
	this->IsPromoted = p;
}

void Bishop::drawDead(int r, int c, int nods)
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
	fillellipse(c * CDIM + CDIM / 2, r * RDIM + 20, 10, 10);
	circle(c * CDIM + CDIM / 2, r * RDIM + 20, 10);
	bar((c * CDIM) + (CDIM / 2) - 3, (r * RDIM) + 15, (c * CDIM) + (CDIM / 2) + 3, (r * RDIM) + RDIM - 15);
}

bool Bishop::IsLegalMove(Board* B, int sr, int sc, int dr, int dc)
{
	if (!this->IsPromoted)
		return IsDiagonalMove(sr, sc, dr, dc) && IsDiagPathClear(B, sr, sc, dr, dc);
	else
		return IsDiagonalMove(sr, sc, dr, dc) && IsDiagPathClear(B, sr, sc, dr, dc)
		|| King::IsLegalMove(B, sr, sc, dr, dc);
}

bool Bishop::IsLegalDrop(Board* B, int dr, int dc)
{
	return B->getCs(dr, dc)->getPs() == NULL;
}