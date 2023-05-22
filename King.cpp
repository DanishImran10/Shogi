#include "King.h"
#include <iostream>
using namespace std;


King::King(int _r, int _c, Color _C, Board* _B)
	:Piece(_r, _c, _C, _B)
{
}

void King::draw()
{
	if(this->C == black)
		setfillstyle(SOLID_FILL, BLUE);
	else
		setfillstyle(SOLID_FILL, RED);
	bar((ci * CDIM) + (CDIM/2) - 3, (ri * RDIM) + 15, (ci * CDIM) + (CDIM/2) + 3, (ri * RDIM) + RDIM - 15);
	bar((ci * CDIM) + 15, (ri * RDIM) + (RDIM/2) - 3, (ci * CDIM) + CDIM - 15, (ri * RDIM) + (RDIM/2) + 3);
}

void King::drawDead(int r, int c, int nods)
{

}

bool King::getIsPromoted()
{
	return true;
}

void King::setIsPromoted(bool p)
{

}

bool King::IsLegalMove(Board* B, int sr, int sc, int dr, int dc)
{
	int DR = abs(sr - dr);
	int DC = abs(sc - dc);
	if (!(DR <= 1 && DC <= 1))
		return false;
	bool a = IsVerticalPathClear(B, sr, sc, dr, dc) || IsHorizontalPathClear(B, sr, sc, dr, dc)
		|| IsDiagPathClear(B, sr, sc, dr, dc);
	return a;
}

bool King::IsLegalDrop(Board* B, int dr, int dc)
{
	return true;
}
