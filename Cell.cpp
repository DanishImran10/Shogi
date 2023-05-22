#include "Cell.h"
#include "Piece.h"
#include "Board.h"

Cell::Cell(Piece* _Ps)
{
	this->Ps = _Ps;
}

Piece* Cell::getPs()
{
	return this->Ps;
}

void Cell::setPs(Piece* _Ps)
{
	this->Ps = _Ps;
}

void Cell::drawCell(int sr, int sc, int rdim, int cdim)
{
	int R = sr / RDIM;
	int C = sc / CDIM;
	if((R + C) % 2 == 0)
		setfillstyle(SOLID_FILL, BLACK);
	else
		setfillstyle(SOLID_FILL, CYAN);
	bar(sr, sc, rdim, cdim);
}

void Cell::highlightcell(Board* B, int sr, int sc, int dr, int dc)
{
	if (this->Ps->IsLegalMove(B, sr, sc, dr, dc))
	{
			setcolor(LIGHTRED);
			rectangle(dc * CDIM, dr * RDIM, (dc + 1) * CDIM - 1, (dr + 1) * RDIM - 1);
			rectangle(dc * CDIM + 1, dr * RDIM + 1, (dc + 1) * CDIM - 2, (dr + 1) * RDIM - 2);
	}
}

void Cell::unhighlightcell(Board* B, int sr, int sc, int dr, int dc)
{
	if (this->Ps->IsLegalMove(B, sr, sc, dr, dc))
	{
		this->drawCell(dc * CDIM, dr * RDIM, (dc + 1) * CDIM, (dr + 1) * RDIM);
	}
}