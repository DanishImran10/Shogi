#pragma once
#include <iomanip>
class Timer
{
	int min;
	int sec;
public:
	Timer();
	Timer(int mi, int se);
	Timer operator--(int);
	bool operator<(const Timer&)const;
	bool operator>(const Timer&)const;
	bool operator==(const Timer&)const;
	int getMin();
	int getSec();
};

