#include "Timer.h"

Timer::Timer()
{
	min = 0;
	sec = 0;
}

Timer::Timer(int mi, int se)
{
	min = mi;
	sec = se;
}

Timer Timer::operator--(int)
{
	Timer R = *this;
	if (this->sec == 0)
	{
		if (this->min == 0)
			return R;
		this->min -= 1;
		this->sec = 59;
		return R;
	}
	this->sec -= 1;
	return R;
}

bool Timer::operator<(const Timer& T)const
{
	if (min < T.min)
		return true;
	if (min > T.min)
		return false;
	if (sec < T.sec)
		return true;
	return false;
}

bool Timer::operator>(const Timer& T)const
{
	return T.operator<(*this);
}

bool Timer::operator==(const Timer& T)const
{
	return !(this->operator<(T) || this->operator>(T));
}

int Timer::getMin()
{
	return min;
}

int Timer::getSec()
{
	return sec;
}