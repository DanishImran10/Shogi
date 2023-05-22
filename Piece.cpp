#include "Piece.h"
#include "Board.h"
#include"Cell.h"

Piece::Piece()
{

}

Piece::Piece(int _ri, int _ci, Color _C, Board* _B)
{
	this->ri = _ri;
	this->ci = _ci;
	this->C = _C;
	this->B = _B;
}

Color Piece::getColor()
{
	return this->C;
}

void Piece::setColor(Color _C)
{
	this->C = _C;
}

bool Piece::getIsPromoted()
{
	return this->IsPromoted;
}

void Piece::setIsPromoted(bool p)
{
	this->IsPromoted = p;
}

void Piece::move(int r, int c)
{
	this->ri = r;
	this->ci = c;
	//if (this->C == white && this->ri == 2 || this->C == black && this->ri == 6)
		//this->IsPromoted = true;
}
