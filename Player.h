#pragma once
#include "Header.h"
#include <string>
using namespace std;
class Player
{
protected:
	string Name;
	Color C;
public:
	Player(string, Color);
	string getName();
	Color getColor();
};

