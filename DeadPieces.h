#pragma once
#include <vector>
#include "Piece.h"
#include "Header.h"
using namespace std;

class DeadPieces
{
protected:
	vector <Piece*> Vs;
public:
	vector <Piece*>& getVs();
	Piece* getPiece();
	void setPiece(Piece*);
	Color getColor();
	void setColor(Color);
	void draw(int, int, int);
};

