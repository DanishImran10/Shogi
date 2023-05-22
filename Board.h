#pragma once

class Cell;
class Piece;
class DeadPieces;

class Board
{
protected:
	Cell*** Cs;
	DeadPieces* DPs[2][7];
public:
	Board();
	Board(const Board&);
	void drawBoard(int, int, int, int);
	void move(int sr, int sc, int dr, int dc);
	void decoyMove(int sr, int sc, int dr, int dc);
	void drawMove(int, int, int, int);
	Cell* getCs(int, int);
	DeadPieces* getDPs(int, int);
	~Board();
};

