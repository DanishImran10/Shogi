#include "GoldGeneral.h"
#include "Piece.h"
#include <iostream>
using namespace std;

GoldGeneral::GoldGeneral(int _r, int _c, Color _C, Board* _B)
	:Piece(_r, _c, _C, _B)
{

}

void GoldGeneral::draw()
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
	bar((ci* CDIM) + (CDIM / 2) - 20, (ri* RDIM) + 20, (ci* CDIM) + (CDIM / 2) - 15, (ri* RDIM) + RDIM - 10);

	arc(ci * CDIM + CDIM / 2 - 1, ri * RDIM + 34, 45, 135, 20);
	arc(ci * CDIM + CDIM / 2 - 1, ri * RDIM + 34, 45, 135, 21);
	arc(ci * CDIM + CDIM / 2 - 1, ri * RDIM + 34, 45, 135, 22);
	arc(ci * CDIM + CDIM / 2 - 1, ri * RDIM + 34, 45, 135, 23);
	arc(ci * CDIM + CDIM / 2 - 1, ri * RDIM + 34, 45, 135, 24);

	line((ci * CDIM) + CDIM / 2 + 15, (ri * RDIM) + 15, (ci * CDIM) + (CDIM / 2) - 15, (ri * RDIM) + RDIM - 11);
	line((ci * CDIM) + CDIM / 2 + 16, (ri * RDIM) + 16, (ci * CDIM) + (CDIM / 2) - 14, (ri * RDIM) + RDIM - 10);
	line((ci * CDIM) + CDIM / 2 + 17, (ri * RDIM) + 17, (ci * CDIM) + (CDIM / 2) - 15, (ri * RDIM) + RDIM - 9);

	setcolor(BROWN);
	bgiout << 'G';
	outstreamxy(ci * CDIM + CDIM / 2, ri * RDIM + 40);
}

void GoldGeneral::drawDead(int r, int c, int nods)
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
	bar((c * CDIM) + (CDIM / 2) - 20, (r * RDIM) + 20, (c * CDIM) + (CDIM / 2) - 15, (r * RDIM) + RDIM - 10);

	arc(c * CDIM + CDIM / 2 - 1, r * RDIM + 34, 45, 135, 20);
	arc(c * CDIM + CDIM / 2 - 1, r * RDIM + 34, 45, 135, 21);
	arc(c * CDIM + CDIM / 2 - 1, r * RDIM + 34, 45, 135, 22);
	arc(c * CDIM + CDIM / 2 - 1, r * RDIM + 34, 45, 135, 23);
	arc(c * CDIM + CDIM / 2 - 1, r * RDIM + 34, 45, 135, 24);

	line((c * CDIM) + CDIM / 2 + 15, (r * RDIM) + 15, (c * CDIM) + (CDIM / 2) - 15, (r * RDIM) + RDIM - 11);
	line((c * CDIM) + CDIM / 2 + 16, (r * RDIM) + 16, (c * CDIM) + (CDIM / 2) - 14, (r * RDIM) + RDIM - 10);
	line((c * CDIM) + CDIM / 2 + 17, (r * RDIM) + 17, (c * CDIM) + (CDIM / 2) - 15, (r * RDIM) + RDIM - 9);

	setcolor(BROWN);
	bgiout << 'G';
	outstreamxy(c * CDIM + CDIM / 2, r * RDIM + 40);
}

bool GoldGeneral::getIsPromoted()
{
	return true;
}
void GoldGeneral::setIsPromoted(bool p)
{

}

bool GoldGeneral::IsLegalMove(Board* B, int sr, int sc, int dr, int dc)
{
	int DR = abs(sr - dr);
	int DC = abs(sc - dc);
	int adr = sr - dr;
	if (!(DR <= 1 && DC <= 1))
		return false;
	bool a = IsVerticalPathClear(B, sr, sc, dr, dc) || IsHorizontalPathClear(B, sr, sc, dr, dc)
		|| IsDiagPathClear(B, sr, sc, dr, dc);
	if (DC == 1)
		return a && (this->C == white && adr >= 0 || this->C == black && adr <= 0);
	else
		return a;
}

bool GoldGeneral::IsLegalDrop(Board* B, int dr, int dc)
{
	return B->getCs(dr, dc)->getPs() == NULL;
}