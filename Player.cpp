#include "Player.h"

Player::Player(string _n, Color _C)
{
	this->Name = _n;
	this->C = _C;
}

string Player::getName()
{
	return this->Name;
}

Color Player::getColor()
{
	return this->C;
}
