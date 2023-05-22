#include "Board.h"
#include "Cell.h"
#include "King.h"
#include "Rook.h"
#include "Bishop.h"
#include "GoldGeneral.h"
#include "Pawn.h"
#include "Lance.h"
#include "Knight.h"
#include "SilverGeneral.h"
#include "DeadPieces.h"
#include <iostream>
using namespace std;

Board::Board()
{
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 7; j++)
			DPs[i][j] = new DeadPieces;
	Cs = new Cell * *[9];
	for (int r = 0; r < 9; r++)
	{
		Cs[r] = new Cell * [9];
		for (int c = 0; c < 9; c++)
		{
			if (r == 2)
				Cs[r][c] = new Cell(new Pawn(r, c, black, this));
			else if (r == 6)
				Cs[r][c] = new Cell(new Pawn(r, c, white, this));
			else if (r == 1 && c == 7)
				Cs[r][c] = new Cell(new Bishop(r, c, black, this));
			else if (r == 7 && c == 1)
				Cs[r][c] = new Cell(new Bishop(r, c, white, this));
			else if (r == 1 && c == 1)
				Cs[r][c] = new Cell(new Rook(r, c, black, this));
			else if (r == 7 && c == 7)
				Cs[r][c] = new Cell(new Rook(r, c, white, this));
			else if (r == 0 && c == 0 || r == 0 && c == 8)
				Cs[r][c] = new Cell(new Lance(r, c, black, this));
			else if (r == 8 && c == 0 || r == 8 && c == 8)
				Cs[r][c] = new Cell(new Lance(r, c, white, this));
			else if (r == 0 && c == 1 || r == 0 && c == 7)
				Cs[r][c] = new Cell(new Knight(r, c, black, this));
			else if (r == 8 && c == 1 || r == 8 && c == 7)
				Cs[r][c] = new Cell(new Knight(r, c, white, this));
			else if (r == 0 && c == 2 || r == 0 && c == 6)
				Cs[r][c] = new Cell(new SilverGeneral(r, c, black, this));
			else if (r == 8 && c == 2 || r == 8 && c == 6)
				Cs[r][c] = new Cell(new SilverGeneral(r, c, white, this));
			else if (r == 0 && c == 3 || r == 0 && c == 5)
				Cs[r][c] = new Cell(new GoldGeneral(r, c, black, this));
			else if (r == 8 && c == 3 || r == 8 && c == 5)
				Cs[r][c] = new Cell(new GoldGeneral(r, c, white, this));
			else if (r == 0 && c == 4)
				Cs[r][c] = new Cell(new King(r, c, black, this));
			else if (r == 8 && c == 4)
				Cs[r][c] = new Cell(new King(r, c, white, this));
			else
				Cs[r][c] = new Cell(nullptr);
		}
	}
}

Board::Board(const Board& B)
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			DPs[i][j] = new DeadPieces;
			DPs[i][j] = B.DPs[i][j];
		}
	}
	Cs = new Cell * *[9];
	for (int r = 0; r < 9; r++)
	{
		Cs[r] = new Cell * [9];
		for (int c = 0; c < 9; c++)
		{
			Rook* R = dynamic_cast <Rook*> (B.Cs[r][c]->getPs());
			if (R != NULL)
			{
				Cs[r][c] = new Cell(new Rook(r, c, B.Cs[r][c]->getPs()->getColor(), this));
				Cs[r][c]->getPs()->setIsPromoted(B.Cs[r][c]->getPs()->getIsPromoted());
				continue;
			}
			Bishop* Bis = dynamic_cast <Bishop*> (B.Cs[r][c]->getPs());
			if (Bis != NULL)
			{
				Cs[r][c] = new Cell(new Bishop(r, c, B.Cs[r][c]->getPs()->getColor(), this));
				Cs[r][c]->getPs()->setIsPromoted(B.Cs[r][c]->getPs()->getIsPromoted());
				continue;
			}
			King* K = dynamic_cast <King*> (B.Cs[r][c]->getPs());
			if (K != NULL)
			{
				Cs[r][c] = new Cell(new King(r, c, B.Cs[r][c]->getPs()->getColor(), this));
				Cs[r][c]->getPs()->setIsPromoted(B.Cs[r][c]->getPs()->getIsPromoted());
				continue;
			}
			SilverGeneral* S = dynamic_cast <SilverGeneral*> (B.Cs[r][c]->getPs());
			if (S != NULL)
			{
				Cs[r][c] = new Cell(new SilverGeneral(r, c, B.Cs[r][c]->getPs()->getColor(), this));
				Cs[r][c]->getPs()->setIsPromoted(B.Cs[r][c]->getPs()->getIsPromoted());
				continue;
			}
			Lance* L = dynamic_cast <Lance*> (B.Cs[r][c]->getPs());
			if (L != NULL)
			{
				Cs[r][c] = new Cell(new Lance(r, c, B.Cs[r][c]->getPs()->getColor(), this));
				Cs[r][c]->getPs()->setIsPromoted(B.Cs[r][c]->getPs()->getIsPromoted());
				continue;
			}
			Knight* Kni = dynamic_cast <Knight*> (B.Cs[r][c]->getPs());
			if (Kni != NULL)
			{
				Cs[r][c] = new Cell(new Knight(r, c, B.Cs[r][c]->getPs()->getColor(), this));
				Cs[r][c]->getPs()->setIsPromoted(B.Cs[r][c]->getPs()->getIsPromoted());
				continue;
			}
			Pawn* P = dynamic_cast <Pawn*> (B.Cs[r][c]->getPs());
			if (P != NULL)
			{
				Cs[r][c] = new Cell(new Pawn(r, c, B.Cs[r][c]->getPs()->getColor(), this));
				Cs[r][c]->getPs()->setIsPromoted(B.Cs[r][c]->getPs()->getIsPromoted());
				continue;
			}
			GoldGeneral* G = dynamic_cast <GoldGeneral*> (B.Cs[r][c]->getPs());
			if (G != NULL)
			{
				Cs[r][c] = new Cell(new GoldGeneral(r, c, B.Cs[r][c]->getPs()->getColor(), this));
				Cs[r][c]->getPs()->setIsPromoted(B.Cs[r][c]->getPs()->getIsPromoted());
				continue;
			}
			Cs[r][c] = new Cell(NULL);
		}
	}
}

void Board::drawBoard(int sr, int sc, int rdim, int cdim)
{
	for (int r = sr, ri = 0; r < (9 * cdim); r += rdim, ri++)
	{
		for (int c = sc, ci = 0; c < (9 * cdim); c += cdim, ci++)
		{
			this->Cs[ri][ci]->drawCell(r, c, (ri + 1) * rdim, (ci + 1) * cdim);
		}
	}
	for (int r = 0; r < 9; r++)
	{
		for (int c = 0; c < 9; c++)
		{
			if (Cs[r][c]->getPs() != nullptr)
				Cs[r][c]->getPs()->draw();
		}
		cout << endl;
	}
}

void Board::move(int sr, int sc, int dr, int dc)
{
	if (Cs[dr][dc]->getPs() != NULL)
	{
		while (true)
		{
			if (Cs[dr][dc]->getPs()->getColor() == white)
			{
				Rook* var1 = dynamic_cast <Rook*>(Cs[dr][dc]->getPs());
				if (var1 != nullptr)
				{
					Cs[dr][dc]->getPs()->setColor(black);
					DPs[0][0]->getVs().push_back(Cs[dr][dc]->getPs());
					Cs[dr][dc]->getPs()->drawDead(0, 9, DPs[0][0]->getVs().size());
					break;
				}
				Bishop* var2 = dynamic_cast <Bishop*>(Cs[dr][dc]->getPs());
				if (var2 != nullptr)
				{
					Cs[dr][dc]->getPs()->setColor(black);
					DPs[0][1]->getVs().push_back(Cs[dr][dc]->getPs());
					Cs[dr][dc]->getPs()->drawDead(0, 10, DPs[0][1]->getVs().size());
					break;
				}
				SilverGeneral* var4 = dynamic_cast <SilverGeneral*>(Cs[dr][dc]->getPs());
				if (var4 != nullptr)
				{
					Cs[dr][dc]->getPs()->setColor(black);
					DPs[0][2]->getVs().push_back(Cs[dr][dc]->getPs());
					Cs[dr][dc]->getPs()->drawDead(0, 11, DPs[0][2]->getVs().size());
					break;
				}
				Pawn* var7 = dynamic_cast <Pawn*>(Cs[dr][dc]->getPs());
				if (var7 != nullptr)
				{
					Cs[dr][dc]->getPs()->setColor(black);
					DPs[0][3]->getVs().push_back(Cs[dr][dc]->getPs());
					Cs[dr][dc]->getPs()->drawDead(0, 12, DPs[0][3]->getVs().size());
					break;
				}
				Knight* var5 = dynamic_cast <Knight*>(Cs[dr][dc]->getPs());
				if (var5 != nullptr)
				{
					Cs[dr][dc]->getPs()->setColor(black);
					DPs[0][4]->getVs().push_back(Cs[dr][dc]->getPs());
					Cs[dr][dc]->getPs()->drawDead(1, 9, DPs[0][4]->getVs().size());
					break;
				}
				Lance* var6 = dynamic_cast <Lance*>(Cs[dr][dc]->getPs());
				if (var6 != nullptr)
				{
					Cs[dr][dc]->getPs()->setColor(black);
					DPs[0][5]->getVs().push_back(Cs[dr][dc]->getPs());
					Cs[dr][dc]->getPs()->drawDead(1, 10, DPs[0][5]->getVs().size());
					break;
				}
				GoldGeneral* var3 = dynamic_cast <GoldGeneral*>(Cs[dr][dc]->getPs());
				if (var3 != nullptr)
				{
					Cs[dr][dc]->getPs()->setColor(black);
					DPs[0][6]->getVs().push_back(Cs[dr][dc]->getPs());
					Cs[dr][dc]->getPs()->drawDead(1, 11, DPs[0][6]->getVs().size());
					break;
				}
			}
			else
			{
				Rook* var1 = dynamic_cast <Rook*>(Cs[dr][dc]->getPs());
				if (var1 != nullptr)
				{
					Cs[dr][dc]->getPs()->setColor(white);
					DPs[1][0]->getVs().push_back(Cs[dr][dc]->getPs());
					Cs[dr][dc]->getPs()->drawDead(8, 9, DPs[1][0]->getVs().size());
					break;
				}
				Bishop* var2 = dynamic_cast <Bishop*>(Cs[dr][dc]->getPs());
				if (var2 != nullptr)
				{
					Cs[dr][dc]->getPs()->setColor(white);
					DPs[1][1]->getVs().push_back(Cs[dr][dc]->getPs());
					Cs[dr][dc]->getPs()->drawDead(8, 10, DPs[1][1]->getVs().size());
					break;
				}
				SilverGeneral* var4 = dynamic_cast <SilverGeneral*>(Cs[dr][dc]->getPs());
				if (var4 != nullptr)
				{
					Cs[dr][dc]->getPs()->setColor(white);
					DPs[1][2]->getVs().push_back(Cs[dr][dc]->getPs());
					Cs[dr][dc]->getPs()->drawDead(8, 11, DPs[1][2]->getVs().size());
					break;
				}
				Pawn* var7 = dynamic_cast <Pawn*>(Cs[dr][dc]->getPs());
				if (var7 != nullptr)
				{
					Cs[dr][dc]->getPs()->setColor(white);
					DPs[1][3]->getVs().push_back(Cs[dr][dc]->getPs());
					Cs[dr][dc]->getPs()->drawDead(8, 12, DPs[1][3]->getVs().size());
					break;
				}
				Knight* var5 = dynamic_cast <Knight*>(Cs[dr][dc]->getPs());
				if (var5 != nullptr)
				{
					Cs[dr][dc]->getPs()->setColor(white);
					DPs[1][4]->getVs().push_back(Cs[dr][dc]->getPs());
					Cs[dr][dc]->getPs()->drawDead(7, 9, DPs[1][4]->getVs().size());
					break;
				}
				Lance* var6 = dynamic_cast <Lance*>(Cs[dr][dc]->getPs());
				if (var6 != nullptr)
				{
					Cs[dr][dc]->getPs()->setColor(white);
					DPs[1][5]->getVs().push_back(Cs[dr][dc]->getPs());
					Cs[dr][dc]->getPs()->drawDead(7, 10, DPs[1][5]->getVs().size());
					break;
				}
				GoldGeneral* var3 = dynamic_cast <GoldGeneral*>(Cs[dr][dc]->getPs());
				if (var3 != nullptr)
				{
					Cs[dr][dc]->getPs()->setColor(white);
					DPs[1][6]->getVs().push_back(Cs[dr][dc]->getPs());
					Cs[dr][dc]->getPs()->drawDead(7, 11, DPs[1][6]->getVs().size());
					break;
				}
				break;
			}
		}
	}
	Cs[dr][dc]->setPs(Cs[sr][sc]->getPs());
	Cs[sr][sc]->setPs(nullptr);
	Cs[dr][dc]->getPs()->move(dr, dc);
}

void Board::decoyMove(int sr, int sc, int dr, int dc)
{
	Cs[dr][dc]->setPs(Cs[sr][sc]->getPs());
	Cs[sr][sc]->setPs(nullptr);
	Cs[dr][dc]->getPs()->move(dr, dc);
}

void Board::drawMove(int sr, int sc, int dr, int dc)
{
	Cs[sr][sc]->drawCell(sc * CDIM, sr * RDIM, (sc + 1) * CDIM, (sr + 1) * RDIM);
	Cs[dr][dc]->drawCell(dc * CDIM, dr * RDIM, (dc + 1) * CDIM, (dr + 1) * RDIM);
	Cs[dr][dc]->getPs()->draw();
}

Cell* Board::getCs(int r, int c)
{
	return this->Cs[r][c];
}

DeadPieces* Board::getDPs(int i , int j)
{
	return this->DPs[i][j];
}

Board::~Board()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			delete[] this->Cs[i][j]->getPs();
			delete[] this->Cs[i][j];
		}
	}
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 7; j++)
			delete[] this->DPs[i][j];
	}
}
