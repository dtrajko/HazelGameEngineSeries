#pragma once

class Timer
{
public:
	Timer();
	static Timer* get();
	void update();
	virtual ~Timer();

private:
	float m_old_delta;
	float m_new_delta;
	float m_delta_time;
};
