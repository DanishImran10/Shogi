#include "Knight.h"
#include <iostream>
using namespace std;

Knight::Knight(int _r, int _c, Color _C, Board* _B)
	:GoldGeneral(_r, _c, _C, _B)
{

}

void Knight::draw()
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
	arc(ci * CDIM + CDIM / 2, ri * RDIM + RDIM / 2, 45, 135, 20);
	arc(ci * CDIM + CDIM / 2, ri * RDIM + RDIM / 2, 45, 135, 21);
	arc(ci * CDIM + CDIM / 2, ri * RDIM + RDIM / 2, 45, 135, 22);
	arc(ci * CDIM + CDIM / 2, ri * RDIM + RDIM / 2, 45, 135, 23);
	arc(ci * CDIM + CDIM / 2, ri * RDIM + RDIM / 2, 45, 135, 24);

	bar((ci * CDIM) + (CDIM / 2) - 18, (ri * RDIM) + 20, (ci * CDIM) + (CDIM / 2) - 15, (ri * RDIM) + RDIM - 15);
	bar((ci * CDIM) + (CDIM / 2) + 18, (ri * RDIM) + 20, (ci * CDIM) + (CDIM / 2) + 15, (ri * RDIM) + RDIM - 15);
	bar((ci * CDIM) + (CDIM / 2) - 15, (ri * RDIM) + RDIM - 15, (ci * CDIM) + (CDIM / 2) + 15, (ri * RDIM) + RDIM - 18);

	bar((ci * CDIM) + (CDIM / 2) - 3, (ri * RDIM) + 28, (ci * CDIM) + (CDIM / 2) + 3, (ri * RDIM) + RDIM - 21);
	bar((ci * CDIM) + (CDIM / 2) - 12, (ri * RDIM) + 25, (ci * CDIM) + (CDIM / 2) + 12, (ri * RDIM) + 28);

	if (this->IsPromoted)
	{
		setcolor(GREEN);
		bgiout << 'P';
		outstreamxy(ci * CDIM + 5, ri * RDIM + 3);
	}
}

void Knight::drawDead(int r, int c, int nods)
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
	arc(c * CDIM + CDIM / 2, r * RDIM + RDIM / 2, 45, 135, 20);
	arc(c * CDIM + CDIM / 2, r * RDIM + RDIM / 2, 45, 135, 21);
	arc(c * CDIM + CDIM / 2, r * RDIM + RDIM / 2, 45, 135, 22);
	arc(c * CDIM + CDIM / 2, r * RDIM + RDIM / 2, 45, 135, 23);
	arc(c * CDIM + CDIM / 2, r * RDIM + RDIM / 2, 45, 135, 24);

	bar((c * CDIM) + (CDIM / 2) - 18, (r * RDIM) + 20, (c * CDIM) + (CDIM / 2) - 15, (r * RDIM) + RDIM - 15);
	bar((c * CDIM) + (CDIM / 2) + 18, (r * RDIM) + 20, (c * CDIM) + (CDIM / 2) + 15, (r * RDIM) + RDIM - 15);
	bar((c * CDIM) + (CDIM / 2) - 15, (r * RDIM) + RDIM - 15, (c * CDIM) + (CDIM / 2) + 15, (r * RDIM) + RDIM - 18);

	bar((c * CDIM) + (CDIM / 2) - 3, (r * RDIM) + 28, (c * CDIM) + (CDIM / 2) + 3, (r * RDIM) + RDIM - 21);
	bar((c * CDIM) + (CDIM / 2) - 12, (r * RDIM) + 25, (c * CDIM) + (CDIM / 2) + 12, (r * RDIM) + 28);
}

bool Knight::getIsPromoted()
{
	return this->IsPromoted;
}
void Knight::setIsPromoted(bool p)
{
	this->IsPromoted = p;
}

bool Knight::IsLegalMove(Board* B, int sr, int sc, int dr, int dc)
{
	if (!this->IsPromoted)
	{
		int DR = sr - dr;
		int DC = abs(sc - dc);
		return this->C == white && DR == 2 && DC == 1 || this->C == black && DR == -2 && DC == 1;
	}
	else
		return GoldGeneral::IsLegalMove(B, sr, sc, dr, dc);
}

bool Knight::IsLegalDrop(Board* B, int dr, int dc)
{
	return B->getCs(dr, dc)->getPs() == NULL && ((this->C == white && dr >= 2) || (this->C == black && dr <= 6));
}