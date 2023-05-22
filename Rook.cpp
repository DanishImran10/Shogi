#include "Rook.h"
#include <iostream>
using namespace std;

Rook::Rook(int _r, int _c, Color _C, Board* _B)
	:King(_r, _c, _C, _B)
{
}

void Rook::draw()
{
	if (this->C == black)
		setfillstyle(SOLID_FILL, BLUE);
	else
		setfillstyle(SOLID_FILL, RED);
	bar((ci * CDIM) + (CDIM / 2) - 15, (ri * RDIM) + 15, (ci * CDIM) + (CDIM / 2) - 10, (ri * RDIM) + 30);
	bar((ci * CDIM) + (CDIM / 2) - 2, (ri * RDIM) + 15, (ci * CDIM) + (CDIM / 2) + 3, (ri * RDIM) + 30);
	bar((ci * CDIM) + (CDIM / 2) + 10, (ri * RDIM) + 15, (ci * CDIM) + (CDIM / 2) + 15, (ri * RDIM) + 30);
	bar((ci * CDIM) + (CDIM / 2) - 15, (ri * RDIM) + 30, (ci * CDIM) + (CDIM / 2) + 15, (ri * RDIM) + RDIM - 10);

	if (this->IsPromoted)
	{
		setcolor(GREEN);
		bgiout << 'P';
		outstreamxy(ci * CDIM + 5, ri * RDIM + 3);
	}
}

void Rook::drawDead(int r, int c, int nods)
{
	setcolor(WHITE);
	bgiout << 'x' << nods;
	outstreamxy(c * CDIM + 2, r * RDIM + 2);
	if (this->C == white)
		setfillstyle(SOLID_FILL, RED);
	else
		setfillstyle(SOLID_FILL, BLUE);
	bar((c * CDIM) + (CDIM / 2) - 15, (r * RDIM) + 15, (c * CDIM) + (CDIM / 2) - 10, (r * RDIM) + 30);
	bar((c * CDIM) + (CDIM / 2) - 2, (r * RDIM) + 15, (c * CDIM) + (CDIM / 2) + 3, (r * RDIM) + 30);
	bar((c * CDIM) + (CDIM / 2) + 10, (r * RDIM) + 15, (c * CDIM) + (CDIM / 2) + 15, (r * RDIM) + 30);
	bar((c * CDIM) + (CDIM / 2) - 15, (r * RDIM) + 30, (c * CDIM) + (CDIM / 2) + 15, (r * RDIM) + RDIM - 10);
}

bool Rook::getIsPromoted()
{
	return this->IsPromoted;
}

void Rook::setIsPromoted(bool p)
{
	this->IsPromoted = p;
}

bool Rook::IsLegalMove(Board* B, int sr, int sc, int dr, int dc)
{
	if (!this->IsPromoted)
		return (IsHorizontalMove(sr, sc, dr, dc) && IsHorizontalPathClear(B, sr, sc, dr, dc))
		|| (IsVerticalMove(sr, sc, dr, dc) && IsVerticalPathClear(B, sr, sc, dr, dc));
	else
		return (IsHorizontalMove(sr, sc, dr, dc) && IsHorizontalPathClear(B, sr, sc, dr, dc))
		|| (IsVerticalMove(sr, sc, dr, dc) && IsVerticalPathClear(B, sr, sc, dr, dc))
		|| King::IsLegalMove(B, sr, sc, dr, dc);
}

bool Rook::IsLegalDrop(Board* B, int dr, int dc)
{
	return B->getCs(dr, dc)->getPs() == NULL;
}
