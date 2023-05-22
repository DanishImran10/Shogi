#include "DeadPieces.h"
#include "Piece.h"


vector <Piece*>& DeadPieces::getVs()
{
	return this->Vs;
}

Piece* DeadPieces::getPiece()
{
	return this->Vs[Vs.size() - 1];
}

void DeadPieces::setPiece(Piece* _P)
{
	this->Vs[Vs.size() - 1] = _P;
}

Color DeadPieces::getColor()
{
	return this->Vs[0]->getColor();
}

void DeadPieces::setColor(Color _C)
{
	this->Vs[0]->setColor(_C);
}

void DeadPieces::draw(int r, int c, int nods)
{
	if (this->Vs[0]->getColor() == black)
		this->Vs[0]->setColor(white);
	else
		this->Vs[0]->setColor(black);
	this->Vs[0]->drawDead(r, c, nods);
}


