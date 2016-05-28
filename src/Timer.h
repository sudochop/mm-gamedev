#ifndef TIMER_H_
#define TIMER_H_

#include <chrono>


typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::nanoseconds nanoseconds;
typedef std::chrono::microseconds microseconds;
typedef std::chrono::milliseconds milliseconds;
typedef std::chrono::seconds seconds;
typedef std::chrono::minutes minutes;
typedef std::chrono::hours hours;


template<typename T>
class Timer {
public:
	Timer();
	T Tick();
	void Reset();

private:
	Clock::time_point time_start_;
	Clock::time_point time_last_;
};


template<typename T>
Timer<T>::Timer() {
	this->Reset();
}


template<typename T>
void Timer<T>::Reset() {
	this->time_start_	= Clock::now();
	this->time_last_	= Clock::now();
}


template<typename T>
T Timer<T>::Tick() {
	auto now = Clock::now();
	auto delta = now - this->time_last_;
	this->time_last_ = now;
	return std::chrono::duration_cast<T>(delta);
}


#endif // TIMER_H_
