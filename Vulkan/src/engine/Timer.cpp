#include "Timer.h"

#include <time.h>


Timer::Timer()
{
}

Timer* Timer::get()
{
	static Timer timer;
	return &timer;
}

void Timer::update()
{
	m_old_delta = m_new_delta;
	m_new_delta = (float)time(NULL); // time in seconds
	m_delta_time = m_old_delta ? ((m_new_delta - m_old_delta) / 1000.0f) : 0.0f;
}

Timer::~Timer()
{

}
