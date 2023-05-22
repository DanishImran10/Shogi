#pragma once
#include"Header.h"
#include<math.h>
#include "graphics.h"
#include "Board.h"
#include "Cell.h"

class Piece
{
protected:
	bool IsPromoted;
	int ri, ci;
	Color C;
	Board* B;
	static bool IsHorizontalMove(int sri, int sci, int dri, int dci)
	{
		return sri == dri;
	}
	static bool IsVerticalMove(int sri, int sci, int dri, int dci)
	{
		return sci == dci;
	}
	static bool IsDiagonalMove(int sri, int sci, int dri, int dci)
	{
		int SR = abs(sci - dci);
		int DR = abs(sri - dri);
		return SR == DR;
	}
	static bool IsHorizontalPathClear(Board* B, int sr, int sc, int dr, int dc)
	{
		if (sc < dc)
		{
			for (int c = sc + 1; c < dc; c++)
			{
				if (B->getCs(sr, c)->getPs() != NULL)
				{
					return false;
				}
			}
			return true;
		}
		else
		{
			for (int c = sc - 1; c > dc; c--)
			{
				if (B->getCs(sr, c)->getPs() != NULL)
				{
					return false;
				}
			}
			return true;
		}
	}
	static bool IsVerticalPathClear(Board* B, int sr, int sc, int dr, int dc)
	{
		if (sr < dr)
		{
			for (int r = sr + 1; r < dr; r++)
			{
				if (B->getCs(r, sc)->getPs() != NULL)
				{
					return false;
				}
			}
			return true;
		}
		else
		{
			for (int r = sr - 1; r > dr; r--)
			{
				if (B->getCs(r, sc)->getPs() != NULL)
				{
					return false;
				}
			}
			return true;
		}
	}
	static bool IsDiagPathClear(Board* B, int sr, int sc, int dr, int dc)
	{
		int t = abs(dr - sr);
		int DR = dr - sr;
		int DC = dc - sc;

		if (DR < 0 && DC < 0)
		{
			for (int i = 1; i < t; i++)
			{
				if (B->getCs(sr - i, sc - i)->getPs() != NULL)
				{
					return false;
				}
			}
			return true;
		}
		else
		if (DR > 0 && DC > 0)
		{
			for (int i = 1; i < t; i++)
			{
				if (B->getCs(sr + i, sc + i)->getPs() != NULL)
				{
					return false;
				}
			}
			return true;
		}
		else
		if (DR > 0 && DC < 0)
		{
			for (int i = 1; i < t; i++)
			{
				if (B->getCs(sr + i, sc - i)->getPs() != NULL)
				{
					return false;
				}
			}
			return true;
		}
		else
		{
			for (int i = 1; i < t; i++)
			{
				if (B->getCs(sr - i, sc + i)->getPs() != NULL)
				{
					return false;
				}
			}
			return true;
		}
	}
public:
	Piece();
	Piece(int, int, Color, Board*);
	virtual void draw() = 0;
	virtual void drawDead(int, int, int) = 0;
	Color getColor();
	void setColor(Color);
	virtual bool getIsPromoted() = 0;
	virtual void setIsPromoted(bool) = 0;
	void move(int r, int c);
	virtual bool IsLegalMove(Board*, int sr, int sc, int dr, int dc) = 0;
	virtual bool IsLegalDrop(Board*, int dr, int dc) = 0;
};

