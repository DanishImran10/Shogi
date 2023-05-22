#include "Shogi.h"
#include "Header.h"
#include "Board.h"
#include "Cell.h"
#include "Piece.h"
#include "Player.h"
#include "DeadPieces.h"
#include "Rook.h"
#include "Bishop.h"
#include "King.h"
#include "GoldGeneral.h"
#include "SilverGeneral.h"
#include "Pawn.h"
#include "Lance.h"
#include "Knight.h"
#include <iostream>
#include "iomanip"
#include "Timer.h"
#include "graphics.h"
#include <vector>
#include <fstream>
#include <thread>
using namespace std;

Shogi::Shogi()
{
	B = new Board;
	turn = 1;
	P[0] = new Player("Blue", black);
	P[1] = new Player("Red", white);
}

void Shogi::turnmsg(Player* AP)
{
	setfillstyle(SOLID_FILL, LIGHTGRAY);
	bar(0, 650, 50, 670);
	setcolor(WHITE);
	bgiout << AP->getName() << "'s turn: \n";
	outstreamxy(0, 650);
}

void Shogi::sourceselect()
{
	do
	{
	} while (!ismouseclick(WM_LBUTTONDOWN));
	getmouseclick(WM_LBUTTONDOWN, sc, sr);
	sr = sr / RDIM;
	sc = sc / CDIM;
}

void Shogi::destselect()
{
	do
	{
	} while (!ismouseclick(WM_LBUTTONDOWN));
	getmouseclick(WM_LBUTTONDOWN, dc, dr);
	dr = dr / RDIM;
	dc = dc / CDIM;
}

bool Shogi::IsValidSource(int sr, int sc, Player* AP)
{
	if (sr < 0 || sr >= 9 || sc < 0 || sc >= 9)
		return false;
	return this->B->getCs(sr, sc)->getPs() != nullptr && AP->getColor() == this->B->getCs(sr, sc)->getPs()->getColor();
}

bool Shogi::IsValidDest(int dr, int dc, Player* AP)
{
	return !IsValidSource(dr, dc, AP) && !(dr < 0 || dr >= 9 || dc < 0 || dc >= 9);
}


void Shogi::turnchange()
{
	this->turn = (this->turn + 1) % 2;
}

void Shogi::Play()
{
	initwindow(1500, 1000, "Shogi");
	setfillstyle(SOLID_FILL, LIGHTGRAY);
	bar(0, 0, 1500, 1000);
	drawMenu();
	bool mode = false, over = false;
	while (true)
	{
		do
		{
		} while (!ismouseclick(WM_LBUTTONDOWN));
		int q, w;
		getmouseclick(WM_LBUTTONDOWN, q, w);
		q = q / CDIM;
		w = w / RDIM;
		if (q == 16 && w == 1)
		{
			drawModeMenu();
			while (true)
			{
				do
				{
				} while (!ismouseclick(WM_LBUTTONDOWN));
				int e, f;
				getmouseclick(WM_LBUTTONDOWN, e, f);
				e = e / CDIM;
				f = f / RDIM;
				if (e == 1 && f == 1)
				{
					mode = false;
					break;
				}
				else
				if (e == 3 && f == 1)
				{
					mode = true;
					break;
				}
			}
			this->B->drawBoard(0, 0, RDIM, CDIM);
			break;
		}
		else
		if (q == 16 && w == 3)
		{
			ifstream Rdr("LastConfiguration.txt");
			loadgame(Rdr, B);
			break;
		}
	}
	bool cimpasse = true, ccheck = false;
	int kr, kc;
	thread thr(&Shogi::timer, this, ref(mode), ref(over), ref(turn));
	do
	{
		if (over)
			break;
		bool D = false;
		int count = 0;
		this->turnchange();
		this->turnmsg(P[turn]);
		do
		{
			do
			{
				if (count > 0)
				{
					setcolor(WHITE);
					bgiout << "Invalid Destination.\n";
					outstreamxy(0, 670);
					count = 0;
				}
				do
				{
					if (count > 0)
					{
						setcolor(WHITE);
						bgiout << "Invalid Source.\n";
						outstreamxy(0, 670);
					}
					sourceselect();
					setfillstyle(SOLID_FILL, LIGHTGRAY);
					bar(0, 670, 150, 690);
					count++;
					if (sr == 3 && sc == 18 && !Check(P[(turn + 1) % 2], P[turn], this->B))
					{
						bool checkdrop = this->dropPiece(B, turn);
						if (checkdrop)
						{
							if (Check(this->P[turn], this->P[(turn + 1) % 2], this->B))
							{
								ccheck = true;
								findKing(this->P[(turn + 1) % 2], this->B, kr, kc);
								setcolor(MAGENTA);
								bgiout << 'C';
								outstreamxy(kc * CDIM + 5, kr * RDIM + 5);
							}
							D = true;
							break;
						}
					}
					if (sr == 1 && sc == 18)
					{
						mode = false;
						thr.join();
						return;
					}
				} while (!IsValidSource(sr, sc, P[turn]));
				if (D)
					break;
				for (int r = 0; r < 9; r++)
				{
					for (int c = 0; c < 9; c++)
					{
						if(IsValidDest(r, c, P[turn]))
							this->highlight(r, c);
					}
				}
				count = 0;
				destselect();
				count++;
				for (int r = 0; r < 9; r++)
				{
					for (int c = 0; c < 9; c++)
					{
						if (IsValidDest(r, c, P[turn]))
						{
							this->unhighlight(r, c);
							if (this->B->getCs(r, c)->getPs() != nullptr)
								this->B->getCs(r, c)->getPs()->draw();
						}
					}
				}
			} while (!IsValidDest(dr, dc, P[turn]));
			if (D)
				break;
		} while (!this->B->getCs(sr, sc)->getPs()->IsLegalMove(B, sr, sc, dr, dc)
			|| SelfCheck(P[(turn + 1) % 2], P[turn], sr, sc, dr, dc));
		if (D)
			continue;
		this->B->move(sr, sc, dr, dc);
		this->B->drawMove(sr, sc, dr, dc);
		if (ccheck)
		{
			ccheck = false;
			this->B->getCs(kr, kc)->drawCell(kc * CDIM, kr * RDIM, (kc + 1) * CDIM, (kr + 1) * RDIM);
			if (this->B->getCs(kr, kc)->getPs() != NULL)
				this->B->getCs(kr, kc)->getPs()->draw();
		}
		if (cimpasse && Impasse())
		{
			cimpasse = false;
			setcolor(WHITE);
			bgiout << "Impasse!\n";
			outstreamxy(100, 650);
			mode = false;
			thr.join();
			do 
			{
			} while (!ismouseclick(WM_LBUTTONDOWN));
			int c, d;
			getmouseclick(WM_LBUTTONDOWN, c, d);
			c = c / CDIM;
			d = d / RDIM;
			if (c == 18 && d == 5)
			{
				int ip1 = ImpassePoints(P[0]);
				int ip2 = ImpassePoints(P[1]);
				setcolor(WHITE);
				bgiout << this->P[0]->getName() << "'s Points: " << ip1 << "\n";
				outstreamxy(0, 680);
				setcolor(WHITE);
				bgiout << this->P[1]->getName() << "'s Points: " << ip2 << "\n";
				outstreamxy(0, 700);
				if (ip1 >= 24 && ip2 < 24)
				{
					setcolor(WHITE);
					bgiout << this->P[0]->getName() << "Wins\n";
					outstreamxy(100, 650);
					getch();
					return;
				}
				else
				if(ip1 < 24 && ip2 >= 24)
				{
					setcolor(WHITE);
					bgiout << this->P[1]->getName() << "Wins\n";
					outstreamxy(100, 650);
					getch();
					return;
				}
				else
				{
					setcolor(WHITE);
					bgiout << "Draw through Impasse!\n";
					outstreamxy(100, 650);
					getch();
					return;
				}
			}
			setfillstyle(SOLID_FILL, LIGHTGRAY);
			bar(100, 650, 150, 700);
		}
		if (!this->B->getCs(dr, dc)->getPs()->getIsPromoted() && promotionPossible(dr, dc))
		{
			setcolor(GREEN);
			rectangle(dc * CDIM, dr * RDIM, (dc + 1) * CDIM - 1, (dr + 1) * RDIM - 1);
			rectangle(dc * CDIM + 1, dr * RDIM + 1, (dc + 1) * CDIM - 2, (dr + 1) * RDIM - 2);
			do
			{
			} while (!ismouseclick(WM_LBUTTONDOWN));
			int x, y;
			getmouseclick(WM_LBUTTONDOWN, x, y);
			this->B->getCs(dr, dc)->drawCell(dc * CDIM, dr * RDIM, (dc + 1) * CDIM, (dr + 1) * RDIM);
			x = x / CDIM;
			y = y / RDIM;
			Knight* K = dynamic_cast <Knight*> (this->B->getCs(dr, dc)->getPs());
			Lance* L = dynamic_cast <Lance*> (this->B->getCs(dr, dc)->getPs());
			Pawn* P = dynamic_cast <Pawn*> (this->B->getCs(dr, dc)->getPs());
			if (x == 16 && y == 5 || (this->B->getCs(dr, dc)->getPs()->getColor() == white && dr == 0 && L != NULL)
				|| (this->B->getCs(dr, dc)->getPs()->getColor() == white && dr == 0 && P != NULL)
				|| (this->B->getCs(dr, dc)->getPs()->getColor() == white && dr <= 1 && K != NULL)
				|| (this->B->getCs(dr, dc)->getPs()->getColor() == black && dr == 8 && L != NULL)
				|| (this->B->getCs(dr, dc)->getPs()->getColor() == black && dr == 8 && P != NULL)
				|| (this->B->getCs(dr, dc)->getPs()->getColor() == black && dr >= 7 && K != NULL))

				this->B->getCs(dr, dc)->getPs()->setIsPromoted(true);
			this->B->getCs(dr, dc)->getPs()->draw();
		}
		ofstream Wtr("LastConfiguration.txt");
		savegame(Wtr, B);
		if (Check(this->P[turn], this->P[(turn + 1) % 2], this->B))
		{
			ccheck = true;
			findKing(this->P[(turn + 1) % 2], this->B, kr, kc);
			setcolor(MAGENTA);
			bgiout << 'C';
			outstreamxy(kc* CDIM + 5, kr* RDIM + 5);
		}
	} while (CanIMoveAPiece(P[turn], P[(turn + 1) % 2]));
	setfillstyle(SOLID_FILL, LIGHTGRAY);
	bar(0, 670, 150, 690);
	if (Checkmate(P[turn], P[(turn + 1) % 2]))
	{
		setcolor(WHITE);
		bgiout << "Checkmate!";
		outstreamxy(100, 650);
	}
	if (Stalemate(P[turn], P[(turn + 1) % 2]))
	{
		setcolor(WHITE);
		bgiout << "Stalemate!";
		outstreamxy(100, 650);
	}
	mode = false;
	thr.join();
	getch();
}

void Shogi::highlight(int r, int c)
{
	if (this->B->getCs(sr, sc)->getPs()->IsLegalMove(B, sr, sc, r, c) 
		&& !SelfCheck(P[(turn + 1) % 2], P[turn], sr, sc, r, c))
	{
		setcolor(LIGHTRED);
		rectangle(c * CDIM, r * RDIM, (c + 1) * CDIM - 1, (r + 1) * RDIM - 1);
		rectangle(c * CDIM + 1, r * RDIM + 1, (c + 1) * CDIM - 2, (r + 1) * RDIM - 2);
	}
}

void Shogi::unhighlight(int r, int c)
{
	if (this->B->getCs(sr, sc)->getPs()->IsLegalMove(B, sr, sc, r, c) 
		&& !SelfCheck(P[(turn + 1) % 2], P[turn], sr, sc, r, c))
	{
		this->B->getCs(sr, sc)->drawCell(c * CDIM, r * RDIM, (c + 1) * CDIM, (r + 1) * RDIM);
	}
}

bool Shogi::promotionPossible(int dr, int dc)
{
	return (dr <= 2 && this->B->getCs(dr, dc)->getPs()->getColor() == white)
		|| (dr >= 6 && this->B->getCs(dr, dc)->getPs()->getColor() == black);
}

bool Shogi::highlightdrops(Piece* P)
{
	Pawn* var = dynamic_cast <Pawn*> (P);
	bool h = false;
	for (int c = 0; c < 9; c++)
	{
		for (int r = 0; r < 9; r++)
		{
			if (P->IsLegalDrop(this->B, r, c))
			{
				if (var != NULL)
				{
					if (!this->PawnDropLegality(var, c) || DropPawnCheckmate(this->P[turn], this->P[(turn + 1) % 2], r, c))
						continue;
				}
				setcolor(LIGHTRED);
				rectangle(c * CDIM, r * RDIM, (c + 1) * CDIM - 1, (r + 1) * RDIM - 1);
				rectangle(c * CDIM + 1, r * RDIM + 1, (c + 1) * CDIM - 2, (r + 1) * RDIM - 2);
				h = true;
			}
		}
	}
	return h;
}

void Shogi::unhighlightdrops(Piece* P)
{
	Pawn* var = dynamic_cast <Pawn*> (P);
	bool c = false;
	for (int c = 0; c < 9; c++)
	{
		for (int r = 0; r < 9; r++)
		{
			if (P->IsLegalDrop(this->B, r, c))
			{
				if (var != NULL)
				{
					if (!this->PawnDropLegality(var, c) || DropPawnCheckmate(this->P[turn], this->P[(turn + 1) % 2], r, c))
						continue;
				}
				this->B->getCs(r, c)->drawCell(c * CDIM, r * RDIM, (c + 1) * CDIM, (r + 1) * RDIM);
			}
		}
	}
}

bool Shogi::dropPiece(Board* B, int turn)
{
	int a, b, x, y;
	bool ch = false;
	do
	{
	} while (!ismouseclick(WM_LBUTTONDOWN));
	getmouseclick(WM_LBUTTONDOWN, a, b);
	a = a / CDIM;
	b = b / RDIM;
	if (turn == 0)
	{
		if (a == 9 && b == 0)
		{
			if (B->getDPs(turn, 0)->getVs().size() == 0)
				return ch;
			highlightdrops(B->getDPs(turn, 0)->getPiece());
			do
			{
				do
				{
				} while (!ismouseclick(WM_LBUTTONDOWN));
				getmouseclick(WM_LBUTTONDOWN, x, y);
				x = x / CDIM;
				y = y / RDIM;
			} while (!(B->getDPs(turn, 0)->getPiece()->IsLegalDrop(B, y, x) && IsValidDest(y, x, this->P[turn])));
			unhighlightdrops(B->getDPs(turn, 0)->getPiece());
			B->getCs(y, x)->setPs(B->getDPs(turn, 0)->getPiece());
			B->getDPs(turn, 0)->getVs().pop_back();
			B->getCs(y, x)->getPs()->move(y, x);
			B->getCs(y, x)->getPs()->setIsPromoted(false);
			B->getCs(y, x)->getPs()->draw();
			if(B->getDPs(turn, 0)->getVs().size() > 0)
				B->getCs(y, x)->getPs()->drawDead(0, 9, B->getDPs(turn, 0)->getVs().size());
			else
			{
				setfillstyle(SOLID_FILL, LIGHTGRAY);
				bar(9 * CDIM, 0 * RDIM, 10 * CDIM, 1 * RDIM);
			}
		}
		else
		if (a == 10 && b == 0)
		{
			if (B->getDPs(turn, 1)->getVs().size() == 0)
				return ch;
			highlightdrops(B->getDPs(turn, 1)->getPiece());
			do
			{
				do
				{
				} while (!ismouseclick(WM_LBUTTONDOWN));
				getmouseclick(WM_LBUTTONDOWN, x, y);
				x = x / CDIM;
				y = y / RDIM;
			} while (!(B->getDPs(turn, 1)->getPiece()->IsLegalDrop(B, y, x) && IsValidDest(y, x, this->P[turn])));
			unhighlightdrops(B->getDPs(turn, 1)->getPiece());
			B->getCs(y, x)->setPs(B->getDPs(turn, 1)->getPiece());
			B->getDPs(turn, 1)->getVs().pop_back();
			B->getCs(y, x)->getPs()->move(y, x);
			B->getCs(y, x)->getPs()->setIsPromoted(false);
			B->getCs(y, x)->getPs()->draw();
			if (B->getDPs(turn, 1)->getVs().size() > 0)
				B->getCs(y, x)->getPs()->drawDead(10, 0, B->getDPs(turn, 1)->getVs().size());
			else
			{
				setfillstyle(SOLID_FILL, LIGHTGRAY);
				bar(10 * CDIM, 0 * RDIM, 11 * CDIM, 1 * RDIM);
			}
		}
		else
		if (a == 11 && b == 0)
		{
			if (B->getDPs(turn, 2)->getVs().size() == 0)
				return ch;
			highlightdrops(B->getDPs(turn, 2)->getPiece());
			do
			{
				do
				{
				} while (!ismouseclick(WM_LBUTTONDOWN));
				getmouseclick(WM_LBUTTONDOWN, x, y);
				x = x / CDIM;
				y = y / RDIM;
			} while (!(B->getDPs(turn, 2)->getPiece()->IsLegalDrop(B, y, x) && IsValidDest(y, x, this->P[turn])));
			unhighlightdrops(B->getDPs(turn, 2)->getPiece());
			B->getCs(y, x)->setPs(B->getDPs(turn, 2)->getPiece());
			B->getDPs(turn, 2)->getVs().pop_back();
			B->getCs(y, x)->getPs()->move(y, x);
			B->getCs(y, x)->getPs()->setIsPromoted(false);
			B->getCs(y, x)->getPs()->draw();
			if (B->getDPs(turn, 2)->getVs().size() > 0)
				B->getCs(y, x)->getPs()->drawDead(0, 11, B->getDPs(turn, 2)->getVs().size());
			else
			{
				setfillstyle(SOLID_FILL, LIGHTGRAY);
				bar(11 * CDIM, 0 * RDIM, 12 * CDIM, 1 * RDIM);
			}
		}
		else
		if (a == 12 && b == 0)
		{
			if (B->getDPs(turn, 3)->getVs().size() == 0)
				return ch;
			bool h = highlightdrops(B->getDPs(turn, 3)->getPiece());
			if (!h)
				return ch;
			do
			{
				do
				{
				} while (!ismouseclick(WM_LBUTTONDOWN));
				getmouseclick(WM_LBUTTONDOWN, x, y);
				x = x / CDIM;
				y = y / RDIM;
			} while (!(IsValidDest(y, x, P[turn]) && B->getDPs(turn, 3)->getPiece()->IsLegalDrop(B, y, x)
				&& this->PawnDropLegality(B->getDPs(turn, 3)->getPiece(), x)) || DropPawnCheckmate(P[turn], P[(turn + 1) % 2], y, x));
			unhighlightdrops(B->getDPs(turn, 3)->getPiece());
			B->getCs(y, x)->setPs(B->getDPs(turn, 3)->getPiece());
			B->getDPs(turn, 3)->getVs().pop_back();
			B->getCs(y, x)->getPs()->move(y, x);
			B->getCs(y, x)->getPs()->setIsPromoted(false);
			B->getCs(y, x)->getPs()->draw();
			if (B->getDPs(turn, 3)->getVs().size() > 0)
				B->getCs(y, x)->getPs()->drawDead(0, 12, B->getDPs(turn, 3)->getVs().size());
			else
			{
				setfillstyle(SOLID_FILL, LIGHTGRAY);
				bar(12 * CDIM, 0 * RDIM, 13 * CDIM, 1 * RDIM);
			}
		}
		else
		if (a == 9 && b == 1)
		{
			if (B->getDPs(turn, 4)->getVs().size() == 0)
				return ch;
			highlightdrops(B->getDPs(turn, 4)->getPiece());
			do
			{
				do
				{
				} while (!ismouseclick(WM_LBUTTONDOWN));
				getmouseclick(WM_LBUTTONDOWN, x, y);
				x = x / CDIM;
				y = y / RDIM;
			} while (!(IsValidDest(y, x, P[turn]) && B->getDPs(turn, 4)->getPiece()->IsLegalDrop(B, y, x)));
			unhighlightdrops(B->getDPs(turn, 4)->getPiece());
			B->getCs(y, x)->setPs(B->getDPs(turn, 4)->getPiece());
			B->getDPs(turn, 4)->getVs().pop_back();
			B->getCs(y, x)->getPs()->move(y, x);
			B->getCs(y, x)->getPs()->setIsPromoted(false);
			B->getCs(y, x)->getPs()->draw();
			if (B->getDPs(turn, 4)->getVs().size() > 0)
				B->getCs(y, x)->getPs()->drawDead(1, 9, B->getDPs(turn, 4)->getVs().size());
			else
			{
				setfillstyle(SOLID_FILL, LIGHTGRAY);
				bar(9 * CDIM, 1 * RDIM, 10 * CDIM, 2 * RDIM);
			}
		}
		else
		if (a == 10 && b == 1)
		{
			if (B->getDPs(turn, 5)->getVs().size() == 0)
				return ch;
			highlightdrops(B->getDPs(turn, 5)->getPiece());
			do
			{
				do
				{
				} while (!ismouseclick(WM_LBUTTONDOWN));
				getmouseclick(WM_LBUTTONDOWN, x, y);
				x = x / CDIM;
				y = y / RDIM;
			} while (!(IsValidDest(y, x, P[turn]) && B->getDPs(turn, 5)->getPiece()->IsLegalDrop(B, y, x)));
			unhighlightdrops(B->getDPs(turn, 5)->getPiece());
			B->getCs(y, x)->setPs(B->getDPs(turn, 5)->getPiece());
			B->getDPs(turn, 5)->getVs().pop_back();
			B->getCs(y, x)->getPs()->move(y, x);
			B->getCs(y, x)->getPs()->setIsPromoted(false);
			B->getCs(y, x)->getPs()->draw();
			if (B->getDPs(turn, 5)->getVs().size() > 0)
				B->getCs(y, x)->getPs()->drawDead(1, 10, B->getDPs(turn, 5)->getVs().size());
			else
			{
				setfillstyle(SOLID_FILL, LIGHTGRAY);
				bar(10 * CDIM, 1 * RDIM, 11 * CDIM, 2 * RDIM);
			}
		}
		else
		if (a == 11 && b == 1)
		{
			if (B->getDPs(turn, 6)->getVs().size() == 0)
				return ch;
			highlightdrops(B->getDPs(turn, 6)->getPiece());
			do
			{
				do
				{
				} while (!ismouseclick(WM_LBUTTONDOWN));
				getmouseclick(WM_LBUTTONDOWN, x, y);
				x = x / CDIM;
				y = y / RDIM;
			} while (!(IsValidDest(y, x, P[turn]) && B->getDPs(turn, 6)->getPiece()->IsLegalDrop(B, y, x)));
			unhighlightdrops(B->getDPs(turn, 6)->getPiece());
			B->getCs(y, x)->setPs(B->getDPs(turn, 6)->getPiece());
			B->getDPs(turn, 6)->getVs().pop_back();
			B->getCs(y, x)->getPs()->move(y, x);
			B->getCs(y, x)->getPs()->setIsPromoted(false);
			B->getCs(y, x)->getPs()->draw();
			if (B->getDPs(turn, 6)->getVs().size() > 0)
				B->getCs(y, x)->getPs()->drawDead(1, 11, B->getDPs(turn, 6)->getVs().size());
			else
			{
				setfillstyle(SOLID_FILL, LIGHTGRAY);
				bar(11 * CDIM, 1 * RDIM, 12 * CDIM, 2 * RDIM);
			}
		}
		else
			return ch;
	}
	else
	if (turn == 1)
	{
		if (a == 9 && b == 8)
		{
			if (B->getDPs(turn, 0)->getVs().size() == 0)
				return ch;
			highlightdrops(B->getDPs(turn, 0)->getPiece());
			do
			{
				do
				{
				} while (!ismouseclick(WM_LBUTTONDOWN));
				getmouseclick(WM_LBUTTONDOWN, x, y);
				x = x / CDIM;
				y = y / RDIM;
			} while (!(IsValidDest(y, x, P[turn]) && B->getDPs(turn, 0)->getPiece()->IsLegalDrop(B, y, x)));
			unhighlightdrops(B->getDPs(turn, 0)->getPiece());
			B->getCs(y, x)->setPs(B->getDPs(turn, 0)->getPiece());
			B->getDPs(turn, 0)->getVs().pop_back();
			B->getCs(y, x)->getPs()->move(y, x);
			B->getCs(y, x)->getPs()->setIsPromoted(false);
			B->getCs(y, x)->getPs()->draw();
			if (B->getDPs(turn, 0)->getVs().size() > 0)
				B->getCs(y, x)->getPs()->drawDead(8, 9, B->getDPs(turn, 0)->getVs().size());
			else
			{
				setfillstyle(SOLID_FILL, LIGHTGRAY);
				bar(9 * CDIM, 8 * RDIM, 10 * CDIM, 9 * RDIM);
			}
		}
		else
		if (a == 10 && b == 8)
		{
			if (B->getDPs(turn, 1)->getVs().size() == 0)
				return ch;
			highlightdrops(B->getDPs(turn, 1)->getPiece());
			do
			{
				do
				{
				} while (!ismouseclick(WM_LBUTTONDOWN));
				getmouseclick(WM_LBUTTONDOWN, x, y);
				x = x / CDIM;
				y = y / RDIM;
			} while (!(IsValidDest(y, x, P[turn]) && B->getDPs(turn, 1)->getPiece()->IsLegalDrop(B, y, x)));
			unhighlightdrops(B->getDPs(turn, 1)->getPiece());
			B->getCs(y, x)->setPs(B->getDPs(turn, 1)->getPiece());
			B->getDPs(turn, 1)->getVs().pop_back();
			B->getCs(y, x)->getPs()->move(y, x);
			B->getCs(y, x)->getPs()->setIsPromoted(false);
			B->getCs(y, x)->getPs()->draw();
			if (B->getDPs(turn, 1)->getVs().size() > 0)
				B->getCs(y, x)->getPs()->drawDead(8, 10, B->getDPs(turn, 1)->getVs().size());
			else
			{
				setfillstyle(SOLID_FILL, LIGHTGRAY);
				bar(10 * CDIM, 8 * RDIM, 11 * CDIM, 9 * RDIM);
			}
		}
		else
		if (a == 11 && b == 8)
		{
			if (B->getDPs(turn, 2)->getVs().size() == 0)
				return ch;
			highlightdrops(B->getDPs(turn, 2)->getPiece());
			do
			{
				do
				{
				} while (!ismouseclick(WM_LBUTTONDOWN));
				getmouseclick(WM_LBUTTONDOWN, x, y);
				x = x / CDIM;
				y = y / RDIM;
			} while (!(IsValidDest(y, x, P[turn]) && B->getDPs(turn, 2)->getPiece()->IsLegalDrop(B, y, x)));
			unhighlightdrops(B->getDPs(turn, 2)->getPiece());
			B->getCs(y, x)->setPs(B->getDPs(turn, 2)->getPiece());
			B->getDPs(turn, 2)->getVs().pop_back();
			B->getCs(y, x)->getPs()->move(y, x);
			B->getCs(y, x)->getPs()->setIsPromoted(false);
			B->getCs(y, x)->getPs()->draw();
			if (B->getDPs(turn, 2)->getVs().size() > 0)
				B->getCs(y, x)->getPs()->drawDead(8, 11, B->getDPs(turn, 2)->getVs().size());
			else
			{
				setfillstyle(SOLID_FILL, LIGHTGRAY);
				bar(11 * CDIM, 8 * RDIM, 12 * CDIM, 9 * RDIM);
			}
		}
		else
		if (a == 12 && b == 8)
		{
			if (B->getDPs(turn, 3)->getVs().size() == 0)
				return ch;
			bool h = highlightdrops(B->getDPs(turn, 3)->getPiece());
			if (!h)
				return ch;
			do
			{
				do
				{
				} while (!ismouseclick(WM_LBUTTONDOWN));
				getmouseclick(WM_LBUTTONDOWN, x, y);
				x = x / CDIM;
				y = y / RDIM;
			} while (!(IsValidDest(y, x, P[turn]) && B->getDPs(turn, 3)->getPiece()->IsLegalDrop(B, y, x)
				&& this->PawnDropLegality(B->getDPs(turn, 3)->getPiece(), x)) || DropPawnCheckmate(P[turn], P[(turn + 1) % 2], y, x));
			unhighlightdrops(B->getDPs(turn, 3)->getPiece());
			B->getCs(y, x)->setPs(B->getDPs(turn, 3)->getPiece());
			B->getDPs(turn, 3)->getVs().pop_back();
			B->getCs(y, x)->getPs()->move(y, x);
			B->getCs(y, x)->getPs()->setIsPromoted(false);
			B->getCs(y, x)->getPs()->draw();
			if (B->getDPs(turn, 3)->getVs().size() > 0)
				B->getCs(y, x)->getPs()->drawDead(8, 12, B->getDPs(turn, 3)->getVs().size());
			else
			{
				setfillstyle(SOLID_FILL, LIGHTGRAY);
				bar(12 * CDIM, 8 * RDIM, 13 * CDIM, 9 * RDIM);
			}
		}
		else
		if (a == 9 && b == 7)
		{
			if (B->getDPs(turn, 4)->getVs().size() == 0)
				return ch;
			highlightdrops(B->getDPs(turn, 4)->getPiece());
			do
			{
				do
				{
				} while (!ismouseclick(WM_LBUTTONDOWN));
				getmouseclick(WM_LBUTTONDOWN, x, y);
				x = x / CDIM;
				y = y / RDIM;
			} while (!(IsValidDest(y, x, P[turn]) && B->getDPs(turn, 4)->getPiece()->IsLegalDrop(B, y, x)));
			unhighlightdrops(B->getDPs(turn, 4)->getPiece());
			B->getCs(y, x)->setPs(B->getDPs(turn, 4)->getPiece());
			B->getDPs(turn, 4)->getVs().pop_back();
			B->getCs(y, x)->getPs()->move(y, x);
			B->getCs(y, x)->getPs()->setIsPromoted(false);
			B->getCs(y, x)->getPs()->draw();
			if (B->getDPs(turn, 4)->getVs().size() > 0)
				B->getCs(y, x)->getPs()->drawDead(7, 9, B->getDPs(turn, 4)->getVs().size());
			else
			{
				setfillstyle(SOLID_FILL, LIGHTGRAY);
				bar(9 * CDIM, 7 * RDIM, 10 * CDIM, 8 * RDIM);
			}
		}
		else
		if (a == 10 && b == 7)
		{
			if (B->getDPs(turn, 5)->getVs().size() == 0)
				return ch;
			highlightdrops(B->getDPs(turn, 5)->getPiece());
			do
			{
				do
				{
				} while (!ismouseclick(WM_LBUTTONDOWN));
				getmouseclick(WM_LBUTTONDOWN, x, y);
				x = x / CDIM;
				y = y / RDIM;
			} while (!(IsValidDest(y, x, P[turn]) && B->getDPs(turn, 5)->getPiece()->IsLegalDrop(B, y, x)));
			unhighlightdrops(B->getDPs(turn, 5)->getPiece());
			B->getCs(y, x)->setPs(B->getDPs(turn, 5)->getPiece());
			B->getDPs(turn, 5)->getVs().pop_back();
			B->getCs(y, x)->getPs()->move(y, x);
			B->getCs(y, x)->getPs()->setIsPromoted(false);
			B->getCs(y, x)->getPs()->draw();
			if (B->getDPs(turn, 5)->getVs().size() > 0)
				B->getCs(y, x)->getPs()->drawDead(7, 10, B->getDPs(turn, 5)->getVs().size());
			else
			{
				setfillstyle(SOLID_FILL, LIGHTGRAY);
				bar(10 * CDIM, 7 * RDIM, 11 * CDIM, 8 * RDIM);
			}
		}
		else
		if (a == 11 && b == 7)
		{
			if (B->getDPs(turn, 6)->getVs().size() == 0)
				return ch;
			highlightdrops(B->getDPs(turn, 6)->getPiece());
			do
			{
				do
				{
				} while (!ismouseclick(WM_LBUTTONDOWN));
				getmouseclick(WM_LBUTTONDOWN, x, y);
				x = x / CDIM;
				y = y / RDIM;
			} while (!(IsValidDest(y, x, P[turn]) && B->getDPs(turn, 6)->getPiece()->IsLegalDrop(B, y, x)));
			unhighlightdrops(B->getDPs(turn, 6)->getPiece());
			B->getCs(y, x)->setPs(B->getDPs(turn, 6)->getPiece());
			B->getDPs(turn, 6)->getVs().pop_back();
			B->getCs(y, x)->getPs()->move(y, x);
			B->getCs(y, x)->getPs()->setIsPromoted(false);
			B->getCs(y, x)->getPs()->draw();
			if (B->getDPs(turn, 6)->getVs().size() > 0)
				B->getCs(y, x)->getPs()->drawDead(7, 11, B->getDPs(turn, 6)->getVs().size());
			else
			{
				setfillstyle(SOLID_FILL, LIGHTGRAY);
				bar(11 * CDIM, 7 * RDIM, 12 * CDIM, 8 * RDIM);
			}
		}
		else
			return ch;
	}
	ch = true;
	return ch;
}

bool Shogi::PawnDropLegality(Piece* P, int col)
{
	for (int i = 0; i < 9; i++)
	{
		Pawn* HP = dynamic_cast <Pawn*> (this->B->getCs(i, col)->getPs());
		if (HP != NULL && this->B->getCs(i, col)->getPs()->getColor() == P->getColor() 
			&& !this->B->getCs(i, col)->getPs()->getIsPromoted())
			return false;
	}
	return true;
}

void Shogi::findKing(Player* AP, Board* b, int& kr, int& kc)
{
	for (int r = 0; r < 9; r++)
	{
		for (int c = 0; c < 9; c++)
		{
			if (b->getCs(r, c)->getPs() != NULL)
			{
				King* K = dynamic_cast <King*> (b->getCs(r, c)->getPs());
				if (K != NULL)
				{
					Bishop* B = dynamic_cast <Bishop*> (b->getCs(r, c)->getPs());
					Rook* R = dynamic_cast <Rook*> (b->getCs(r, c)->getPs());
					if (B == NULL && R == NULL)
					{
						if (K->getColor() == AP->getColor())
						{
							kr = r;
							kc = c;
							return;
						}
					}
				}
			}
		}
	}
}

bool Shogi::Check(Player* CP, Player* LP, Board* b)
{
	int kr, kc;
	this->findKing(LP, b, kr, kc);
	for (int r = 0; r < 9; r++)
	{
		for (int c = 0; c < 9; c++)
		{
			if (b->getCs(r, c)->getPs() != NULL)
			{
				if (b->getCs(r, c)->getPs()->getColor() == CP->getColor()
					&& b->getCs(r, c)->getPs()->IsLegalMove(b, r, c, kr, kc))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool Shogi::SelfCheck(Player* CP, Player* LP, int sri, int sci, int dri, int dci)
{
	Board* TB = new Board(*(this->B));
	TB->decoyMove(sri, sci, dri, dci);
	return this->Check(CP, LP, TB);
}

bool Shogi::DropPawnCheck(Player* CP, Player* LP, int dr, int dc)
{
	int kr, kc;
	findKing(LP, B, kr, kc);
	if (B->getDPs(turn, 3)->getPiece()->getColor() == CP->getColor()
		&& B->getDPs(turn, 3)->getPiece()->IsLegalMove(B, dr, dc, kr, kc))
	{
		return true;
	}
	else
		return false;
}

bool Shogi::DropPawnCheckmate(Player* CP, Player* LP, int dr, int dc)
{
	return DropPawnCheck(CP, LP, dr, dc) && !CanIMoveAPiece(CP, LP);
}

bool Shogi::CanIMoveAPiece(Player* CP, Player* LP)
{
	for (int sri = 0; sri < 9; sri++)
	{
		for (int sci = 0; sci < 9; sci++)
		{
			for (int dri = 0; dri < 9; dri++)
			{
				for (int dci = 0; dci < 9; dci++)
				{
					if (this->B->getCs(sri, sci)->getPs() != NULL 
						&& LP->getColor() == this->B->getCs(sri, sci)->getPs()->getColor() && IsValidDest(dri, dci, LP) 
						&& this->B->getCs(sri, sci)->getPs()->IsLegalMove(this->B, sri, sci, dri, dci))
					{
						if (!SelfCheck(CP, LP, sri, sci, dri, dci))
						{
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

bool Shogi::Checkmate(Player* CP, Player* LP)
{
	return Check(CP, LP, this->B) && !CanIMoveAPiece(CP, LP);
}

bool Shogi::Stalemate(Player* CP, Player* LP)
{
	return !Check(CP, LP, this->B) && !CanIMoveAPiece(CP, LP);
}

bool Shogi::Impasse()
{
	int kr1, kc1, kr2, kc2;
	findKing(P[0], this->B, kr1, kc1);
	findKing(P[1], this->B, kr2, kc2);
	return (this->B->getCs(kr1, kc1)->getPs()->getColor() == black && kr1 >= 6
		&& this->B->getCs(kr2, kc2)->getPs()->getColor() == white && kr2 <= 2);
}

int Shogi::ImpassePoints(Player* AP)
{
	int piecepoints = 0;
	for (int r = 0; r < 9; r++)
	{
		for (int c = 0; c < 9; c++)
		{
			if (this->B->getCs(r, c)->getPs() != NULL)
			{
				if (this->B->getCs(r, c)->getPs()->getColor() == AP->getColor())
				{
					Rook* R = dynamic_cast <Rook*> (this->B->getCs(r, c)->getPs());
					if (R != NULL)
					{
						piecepoints += 5;
						continue;
					}
					Bishop* Bis = dynamic_cast <Bishop*> (this->B->getCs(r, c)->getPs());
					if (Bis != NULL)
					{
						piecepoints += 5;
						continue;
					}
					Lance* L = dynamic_cast <Lance*> (this->B->getCs(r, c)->getPs());
					if (L != NULL)
					{
						piecepoints += 1;
						continue;
					}
					Pawn* P = dynamic_cast <Pawn*> (this->B->getCs(r, c)->getPs());
					if (P != NULL)
					{
						piecepoints += 1;
						continue;
					}
					Knight* K = dynamic_cast <Knight*> (this->B->getCs(r, c)->getPs());
					if (K != NULL)
					{
						piecepoints += 1;
						continue;
					}
					SilverGeneral* S = dynamic_cast <SilverGeneral*> (this->B->getCs(r, c)->getPs());
					if (S != NULL)
					{
						piecepoints += 1;
						continue;
					}
					GoldGeneral* G = dynamic_cast <GoldGeneral*> (this->B->getCs(r, c)->getPs());
					if (G != NULL)
					{
						piecepoints += 1;
						continue;
					}
				}
			}
		}
	}
	return piecepoints;
}

void Shogi::drawMenu()
{
	setfillstyle(SOLID_FILL, BLACK);
	bar(18 * CDIM, 1 * RDIM, 19 * CDIM, 2 * RDIM);
	drawCross(18*CDIM, 1*RDIM);

	bar(16 * CDIM, 1 * RDIM, 17 * CDIM, 2 * RDIM);
	bgiout << "New Game";
	outstreamxy(16 * CDIM, 1 * RDIM + RDIM / 2- 10);

	bar(16 * CDIM, 3 * RDIM, 17 * CDIM, 4 * RDIM);
	bgiout << "Saved" << endl << "Game";
	outstreamxy(16 * CDIM + 15, 3 * RDIM + RDIM /4 + 5);

	bar(18 * CDIM, 3 * RDIM, 19 * CDIM, 4 * RDIM);
	bgiout << "Drop";
	outstreamxy(18 * CDIM + 20, 3 * RDIM + RDIM/4 + 10);

	bar(16 * CDIM, 5 * RDIM, 17 * CDIM, 6 * RDIM);
	bgiout << "Promote";
	outstreamxy(16 * CDIM + 7, 5 * RDIM + RDIM/2 - 10);

	bar(18 * CDIM, 5 * RDIM, 19 * CDIM, 6 * RDIM);
	bgiout << "Impasse";
	outstreamxy(18 * CDIM + 7, 5 * RDIM + RDIM / 2 - 10);
}

void Shogi::drawModeMenu()
{
	setfillstyle(SOLID_FILL, BLACK);
	bar(1 * CDIM, 1 * RDIM, 2 * CDIM, 2 * RDIM);
	bgiout << "Classic" << endl << "Mode";
	outstreamxy(1 * CDIM + CDIM / 4 - 5, 1 * RDIM + RDIM / 2 - 10);

	bar(3 * CDIM, 1 * RDIM, 4 * CDIM, 2 * RDIM);
	bgiout << "Time" << endl << "Mode";
	outstreamxy(3 * CDIM + CDIM / 4, 1 * RDIM + RDIM / 2 - 10);
}

void Shogi::savegame(ofstream& Wtr, Board* B)
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (B->getCs(i, j)->getPs() != NULL)
			{
				Rook* R = dynamic_cast <Rook*> (B->getCs(i, j)->getPs());
				if (R != NULL)
				{
					if (R->getColor() == black)
					{
						if (R->getIsPromoted())
							Wtr << 'A';
						else
							Wtr << 'R';
					}
					else
					{
						if (R->getIsPromoted())
							Wtr << 'a';
						else
							Wtr << 'r';
					}
					continue;
				}
				Bishop* Bis = dynamic_cast <Bishop*> (B->getCs(i, j)->getPs());
				if (Bis != NULL)
				{
					if (Bis->getColor() == black)
					{
						if (Bis->getIsPromoted())
							Wtr << 'Y';
						else
							Wtr << 'B';
					}
					else
					{
						if (Bis->getIsPromoted())
							Wtr << 'y';
						else
							Wtr << 'b';
					}
					continue;
				}
				Lance* L = dynamic_cast <Lance*> (B->getCs(i, j)->getPs());
				if (L != NULL)
				{
					if (L->getColor() == black)
					{
						if (L->getIsPromoted())
							Wtr << 'E';
						else
							Wtr << 'L';
					}
					else
					{
						if (L->getIsPromoted())
							Wtr << 'e';
						else
							Wtr << 'l';
					}
					continue;
				}
				Pawn* P = dynamic_cast <Pawn*> (B->getCs(i, j)->getPs());
				if (P != NULL)
				{
					if (P->getColor() == black)
					{
						if (P->getIsPromoted())
							Wtr << 'O';
						else
							Wtr << 'P';
					}
					else
					{
						if (P->getIsPromoted())
							Wtr << 'o';
						else
							Wtr << 'p';
					}
					continue;
				}
				Knight* N = dynamic_cast <Knight*> (B->getCs(i, j)->getPs());
				if (N != NULL)
				{
					if (N->getColor() == black)
					{
						if (N->getIsPromoted())
							Wtr << 'M';
						else
							Wtr << 'N';
					}
					else
					{
						if (N->getIsPromoted())
							Wtr << 'm';
						else
							Wtr << 'n';
					}
					continue;
				}
				SilverGeneral* S = dynamic_cast <SilverGeneral*> (B->getCs(i, j)->getPs());
				if (S != NULL)
				{
					if (S->getColor() == black)
					{
						if (S->getIsPromoted())
							Wtr << 'T';
						else
							Wtr << 'S';
					}
					else
					{
						if (S->getIsPromoted())
							Wtr << 't';
						else
							Wtr << 's';
					}
					continue;
				}
				GoldGeneral* G = dynamic_cast <GoldGeneral*> (B->getCs(i, j)->getPs());
				if(G != NULL)
				{
					if (G->getColor() == black)
						Wtr << 'G';
					else
						Wtr << 'g';
					continue;
				}
				King* K = dynamic_cast <King*> (B->getCs(i, j)->getPs());
				if (K != NULL)
				{
					if (K->getColor() == black)
						Wtr << 'K';
					else
						Wtr << 'k';
					continue;
				}
			}
			else
				Wtr << '-';
		}
		Wtr << endl;
	}
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			Wtr << B->getDPs(i, j)->getVs().size() << ' ';
		}
	}
}

void Shogi::loadgame(ifstream& Rdr, Board* B)
{
	char ch;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			Rdr >> ch;
			if (ch == 'K')
			{
				B->getCs(i, j)->setPs(new King(i, j, black, B));
				continue;
			}
			if (ch == 'k')
			{
				B->getCs(i, j)->setPs(new King(i, j, white, B));
				continue;
			}
			if (ch == 'R')
			{
				B->getCs(i, j)->setPs(new Rook(i, j, black, B));
				continue;
			}
			if (ch == 'r')
			{
				B->getCs(i, j)->setPs(new Rook(i, j, white, B));
				continue;
			}
			if (ch == 'A')
			{
				B->getCs(i, j)->setPs(new Rook(i, j, black, B));
				B->getCs(i, j)->getPs()->setIsPromoted(true);
				continue;
			}
			if (ch == 'a')
			{
				B->getCs(i, j)->setPs(new Rook(i, j, white, B));
				B->getCs(i, j)->getPs()->setIsPromoted(true);
				continue;
			}
			if (ch == 'B')
			{
				B->getCs(i, j)->setPs(new Bishop(i, j, black, B));
				continue;
			}
			if (ch == 'b')
			{
				B->getCs(i, j)->setPs(new Bishop(i, j, white, B));
				continue;
			}
			if (ch == 'Y')
			{
				B->getCs(i, j)->setPs(new Bishop(i, j, black, B));
				B->getCs(i, j)->getPs()->setIsPromoted(true);
				continue;
			}
			if (ch == 'y')
			{
				B->getCs(i, j)->setPs(new Bishop(i, j, white, B));
				B->getCs(i, j)->getPs()->setIsPromoted(true);
				continue;
			}
			if (ch == 'L')
			{
				B->getCs(i, j)->setPs(new Lance(i, j, black, B));
				continue;
			}
			if (ch == 'l')
			{
				B->getCs(i, j)->setPs(new Lance(i, j, white, B));
				continue;
			}
			if (ch == 'E')
			{
				B->getCs(i, j)->setPs(new Lance(i, j, black, B));
				B->getCs(i, j)->getPs()->setIsPromoted(true);
				continue;
			}
			if (ch == 'e')
			{
				B->getCs(i, j)->setPs(new Lance(i, j, white, B));
				B->getCs(i, j)->getPs()->setIsPromoted(true);
				continue;
			}
			if (ch == 'P')
			{
				B->getCs(i, j)->setPs(new Pawn(i, j, black, B));
				continue;
			}
			if (ch == 'p')
			{
				B->getCs(i, j)->setPs(new Pawn(i, j, white, B));
				continue;
			}
			if (ch == 'O')
			{
				B->getCs(i, j)->setPs(new Pawn(i, j, black, B));
				B->getCs(i, j)->getPs()->setIsPromoted(true);
				continue;
			}
			if (ch == 'o')
			{
				B->getCs(i, j)->setPs(new Pawn(i, j, white, B));
				B->getCs(i, j)->getPs()->setIsPromoted(true);
				continue;
			}
			if (ch == 'G')
			{
				B->getCs(i, j)->setPs(new GoldGeneral(i, j, black, B));
				continue;
			}
			if (ch == 'g')
			{
				B->getCs(i, j)->setPs(new GoldGeneral(i, j, white, B));
				continue;
			}
			if (ch == 'S')
			{
				B->getCs(i, j)->setPs(new SilverGeneral(i, j, black, B));
				continue;
			}
			if (ch == 's')
			{
				B->getCs(i, j)->setPs(new SilverGeneral(i, j, white, B));
				continue;
			}
			if (ch == 'T')
			{
				B->getCs(i, j)->setPs(new SilverGeneral(i, j, black, B));
				B->getCs(i, j)->getPs()->setIsPromoted(true);
				continue;
			}
			if (ch == 't')
			{
				B->getCs(i, j)->setPs(new SilverGeneral(i, j, white, B));
				B->getCs(i, j)->getPs()->setIsPromoted(true);
				continue;
			}
			if (ch == 'N')
			{
				B->getCs(i, j)->setPs(new Knight(i, j, black, B));
				continue;
			}
			if (ch == 'n')
			{
				B->getCs(i, j)->setPs(new Knight(i, j, white, B));
				continue;
			}
			if (ch == 'M')
			{
				B->getCs(i, j)->setPs(new Knight(i, j, black, B));
				B->getCs(i, j)->getPs()->setIsPromoted(true);
				continue;
			}
			if (ch == 'm')
			{
				B->getCs(i, j)->setPs(new Knight(i, j, white, B));
				B->getCs(i, j)->getPs()->setIsPromoted(true);
				continue;
			}
			B->getCs(i, j)->setPs(NULL);
		}
	}
	B->drawBoard(0, 0, RDIM, CDIM);
	int n;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			Rdr >> n;
			for (int k = 0; k < n; k++)
			{
				if (i == 0 && j == 0)
					B->getDPs(i, j)->getVs().push_back(new Rook(i, j, black, B));
				else if (i == 1 && j == 0)
					B->getDPs(i, j)->getVs().push_back(new Rook(i, j, white, B));
				else if (i == 0 && j == 1)
					B->getDPs(i, j)->getVs().push_back(new Bishop(i, j, black, B));
				else if (i == 1 && j == 1)
					B->getDPs(i, j)->getVs().push_back(new Bishop(i, j, white, B));
				else if (i == 0 && j == 2)
					B->getDPs(i, j)->getVs().push_back(new SilverGeneral(i, j, black, B));
				else if (i == 1 && j == 2)
					B->getDPs(i, j)->getVs().push_back(new SilverGeneral(i, j, white, B));
				else if (i == 0 && j == 3)
					B->getDPs(i, j)->getVs().push_back(new Pawn(i, j, black, B));
				else if (i == 1 && j == 3)
					B->getDPs(i, j)->getVs().push_back(new Pawn(i, j, white, B));
				else if (i == 0 && j == 4)
					B->getDPs(i, j)->getVs().push_back(new Knight(i, j, black, B));
				else if (i == 1 && j == 4)
					B->getDPs(i, j)->getVs().push_back(new Knight(i, j, white, B));
				else if (i == 0 && j == 5)
					B->getDPs(i, j)->getVs().push_back(new Lance(i, j, black, B));
				else if (i == 1 && j == 5)
					B->getDPs(i, j)->getVs().push_back(new Lance(i, j, white, B));
				else if (i == 0 && j == 6)
					B->getDPs(i, j)->getVs().push_back(new GoldGeneral(i, j, black, B));
				else if (i == 1 && j == 6)
					B->getDPs(i, j)->getVs().push_back(new GoldGeneral(i, j, white, B));
			}
			if (B->getDPs(i, j)->getVs().size() > 0)
			{
				if (i == 0 && j == 0)
					B->getDPs(i, j)->getPiece()->drawDead(0, 9, B->getDPs(i, j)->getVs().size());
				else if(i == 0 && j == 1)
					B->getDPs(i, j)->getPiece()->drawDead(0, 10, B->getDPs(i, j)->getVs().size());
				else if (i == 0 && j == 2)
					B->getDPs(i, j)->getPiece()->drawDead(0, 11, B->getDPs(i, j)->getVs().size());
				else if (i == 0 && j == 3)
					B->getDPs(i, j)->getPiece()->drawDead(0, 12, B->getDPs(i, j)->getVs().size());
				else if (i == 0 && j == 4)
					B->getDPs(i, j)->getPiece()->drawDead(1, 9, B->getDPs(i, j)->getVs().size());
				else if (i == 0 && j == 5)
					B->getDPs(i, j)->getPiece()->drawDead(1, 10, B->getDPs(i, j)->getVs().size());
				else if (i == 0 && j == 6)
					B->getDPs(i, j)->getPiece()->drawDead(1, 11, B->getDPs(i, j)->getVs().size());
				else if (i == 1 && j == 0)
					B->getDPs(i, j)->getPiece()->drawDead(8, 9, B->getDPs(i, j)->getVs().size());
				else if (i == 1 && j == 1)
					B->getDPs(i, j)->getPiece()->drawDead(8, 10, B->getDPs(i, j)->getVs().size());
				else if (i == 1 && j == 2)
					B->getDPs(i, j)->getPiece()->drawDead(8, 11, B->getDPs(i, j)->getVs().size());
				else if (i == 1 && j == 3)
					B->getDPs(i, j)->getPiece()->drawDead(8, 12, B->getDPs(i, j)->getVs().size());
				else if (i == 1 && j == 4)
					B->getDPs(i, j)->getPiece()->drawDead(7, 9, B->getDPs(i, j)->getVs().size());
				else if (i == 1 && j == 5)
					B->getDPs(i, j)->getPiece()->drawDead(7, 10, B->getDPs(i, j)->getVs().size());
				else if (i == 1 && j == 6)
					B->getDPs(i, j)->getPiece()->drawDead(7, 11, B->getDPs(i, j)->getVs().size());
			}
		}
	}
}

int Shogi::timer(bool& mode, bool& over, int& turn)
{
	while (true)
	{
		static Timer Ts1(10, 0), Ts2(10, 0), Cs;
		while (turn == 0)
		{
			if (!mode)
				return -1;
			setcolor(WHITE);
			bgiout << "Time: " << setfill('0') << setw(2) << Ts1.getMin() << ':'
				<< setfill('0') << setw(2) << Ts1.getSec();
			outstreamxy(700, 310);
			if (Ts1 == Cs)
			{
				bgiout << "Blue's Time Over!";
				outstreamxy(0, 710);
				over = true;
				return 0;
			}
			Sleep(1000);
			Ts1--;
		}
		while (turn == 1)
		{
			if (!mode)
				return -1;
			setcolor(WHITE);
			bgiout << "Time: " << setfill('0') << setw(2) << Ts2.getMin() << ':'
				<< setfill('0') << setw(2) << Ts2.getSec();
			outstreamxy(700, 380);
			if (Ts2 == Cs)
			{
				bgiout << "Red's Time Over!";
				outstreamxy(0, 710);
				over = true;
				return 1;
			}
			Sleep(1000);
			Ts2--;
		}
	}
}