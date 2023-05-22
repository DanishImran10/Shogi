#include "Lance.h"
#include <iostream>
using namespace std;

Lance::Lance(int _r, int _c, Color _C, Board* _B)
	:GoldGeneral(_r, _c, _C, _B)
{

}

void Lance::draw()
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
	line((ci * CDIM) + 10, (ri * RDIM) + RDIM - 10, (ci * CDIM) + CDIM - 30, (ri * RDIM) + 15);
	line((ci * CDIM) + 11, (ri * RDIM) + RDIM - 10, (ci * CDIM) + CDIM - 29, (ri * RDIM) + 15);
	line((ci * CDIM) + 12, (ri * RDIM) + RDIM - 10, (ci * CDIM) + CDIM - 28, (ri * RDIM) + 15);
	line((ci * CDIM) + 13, (ri * RDIM) + RDIM - 10, (ci * CDIM) + CDIM - 27, (ri * RDIM) + 15);
	line((ci * CDIM) + 14, (ri * RDIM) + RDIM - 10, (ci * CDIM) + CDIM - 26, (ri * RDIM) + 15);

	line((ci * CDIM) + CDIM - 36, (ri * RDIM) + 13, (ci * CDIM) + CDIM - 23, (ri * RDIM) + 10);
	line((ci * CDIM) + CDIM - 37, (ri * RDIM) + 12, (ci * CDIM) + CDIM - 22, (ri * RDIM) + 9);
	line((ci * CDIM) + CDIM - 38, (ri * RDIM) + 11, (ci * CDIM) + CDIM - 21, (ri * RDIM) + 8);

	line((ci * CDIM) + CDIM - 23, (ri * RDIM) + 19, (ci * CDIM) + CDIM - 23, (ri * RDIM) + 10);
	line((ci * CDIM) + CDIM - 22, (ri * RDIM) + 20, (ci * CDIM) + CDIM - 22, (ri * RDIM) + 9);
	line((ci * CDIM) + CDIM - 21, (ri * RDIM) + 21, (ci * CDIM) + CDIM - 21, (ri * RDIM) + 8);

	if (this->IsPromoted)
	{
		setcolor(GREEN);
		bgiout << 'P';
		outstreamxy(ci * CDIM + 5, ri * RDIM + 3);
	}
}

void Lance::drawDead(int r, int c, int nods)
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
	line((c * CDIM) + 10, (r * RDIM) + RDIM - 10, (c * CDIM) + CDIM - 30, (r * RDIM) + 15);
	line((c * CDIM) + 11, (r * RDIM) + RDIM - 10, (c * CDIM) + CDIM - 29, (r * RDIM) + 15);
	line((c * CDIM) + 12, (r * RDIM) + RDIM - 10, (c * CDIM) + CDIM - 28, (r * RDIM) + 15);
	line((c * CDIM) + 13, (r * RDIM) + RDIM - 10, (c * CDIM) + CDIM - 27, (r * RDIM) + 15);
	line((c * CDIM) + 14, (r * RDIM) + RDIM - 10, (c * CDIM) + CDIM - 26, (r * RDIM) + 15);

	line((c * CDIM) + CDIM - 36, (r * RDIM) + 13, (c * CDIM) + CDIM - 23, (r * RDIM) + 10);
	line((c * CDIM) + CDIM - 37, (r * RDIM) + 12, (c * CDIM) + CDIM - 22, (r * RDIM) + 9);
	line((c * CDIM) + CDIM - 38, (r * RDIM) + 11, (c * CDIM) + CDIM - 21, (r * RDIM) + 8);

	line((c * CDIM) + CDIM - 23, (r * RDIM) + 19, (c * CDIM) + CDIM - 23, (r * RDIM) + 10);
	line((c * CDIM) + CDIM - 22, (r * RDIM) + 20, (c * CDIM) + CDIM - 22, (r * RDIM) + 9);
	line((c * CDIM) + CDIM - 21, (r * RDIM) + 21, (c * CDIM) + CDIM - 21, (r * RDIM) + 8);
}

bool Lance::getIsPromoted()
{
	return this->IsPromoted;
}
void Lance::setIsPromoted(bool p)
{
	this->IsPromoted = p;
}
bool Lance::IsLegalMove(Board* B, int sr, int sc, int dr, int dc)
{
	if (!this->IsPromoted)
	{
		int adr = sr - dr;
		return (IsVerticalMove(sr, sc, dr, dc) && IsVerticalPathClear(B, sr, sc, dr, dc)
			&& (this->C == white && adr > 0 || this->C == black && adr < 0));
	}
	else
		return GoldGeneral::IsLegalMove(B, sr, sc, dr, dc);
}

bool Lance::IsLegalDrop(Board* B, int dr, int dc)
{
	return B->getCs(dr, dc)->getPs() == NULL && ((this->C == white && dr >= 1) || (this->C == black && dr <= 7));
}