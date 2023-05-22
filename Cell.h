#pragma once
#include"Header.h"
#include "graphics.h"
class Piece;
class Board;

class Cell
{
protected:
	Piece* Ps;
	Color C;
public:
	Cell(Piece*);
	Piece* getPs();
	void setPs(Piece*);
	void drawCell(int, int, int, int);
	void highlightcell(Board*, int, int, int, int);
	void unhighlightcell(Board*, int, int, int, int);
};

