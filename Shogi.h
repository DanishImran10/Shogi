#pragma once
#include "graphics.h"
#include "Header.h"
#include <fstream>
using namespace std;
class Board;
class Player;
class Piece;

class Shogi
{
protected:
	int sr, sc, dr, dc;
	int turn;
	Board* B;
	Player* P[2];
	void drawCross(int c, int r)
	{
		setcolor(WHITE);
		line(c + 10, r + 10, c + CDIM - 10, r + RDIM - 10);
		line(c + 10, r + 9, c + CDIM - 10, r + RDIM - 11);
		line(c + CDIM - 10, r + 10, c + 10, r + RDIM - 10);
		line(c + CDIM - 10, r + 9, c + 10, r + RDIM - 11);
	}
public:
	Shogi();
	void turnmsg(Player*);
	void sourceselect();
	void destselect();
	bool IsValidSource(int sr, int sc, Player*);
	bool IsValidDest(int dr, int dc, Player*);
	void turnchange();
	void Play();
	void highlight(int r, int c);
	void unhighlight(int r, int c);
	bool promotionPossible(int, int);
	bool highlightdrops(Piece*);
	void unhighlightdrops(Piece*);
	bool dropPiece(Board*, int);
	bool PawnDropLegality(Piece*, int);
	void findKing(Player*, Board*, int&, int&);
	bool Check(Player*, Player*, Board*);
	bool SelfCheck(Player*, Player*, int sri, int sci, int dri, int dci);
	bool DropPawnCheck(Player*, Player*, int dr, int dc);
	bool DropPawnCheckmate(Player*, Player*, int, int);
	bool CanIMoveAPiece(Player*, Player*);
	bool Checkmate(Player*, Player*);
	bool Stalemate(Player*, Player*);
	bool Impasse();
	int ImpassePoints(Player*);
	void drawMenu();
	void drawModeMenu();
	void savegame(ofstream& Wtr, Board*);
	void loadgame(ifstream& Rdr, Board*);
	int timer(bool& mode, bool& over, int& turn);
};

