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
	auto second() const;

private:
	Clock::time_point time_start_;
	Clock::time_point time_last_;
};


template<typename T>
Timer<T>::Timer() {
	Reset();
}


template<typename T>
void Timer<T>::Reset() {
	time_start_	= Clock::now();
	time_last_	= Clock::now();
}


template<typename T>
T Timer<T>::Tick() {
	auto now = Clock::now();
	auto delta = now - time_last_;
	time_last_ = now;
	return std::chrono::duration_cast<T>(delta);
}


template<typename T>
auto Timer<T>::second() const {
	using namespace std::chrono_literals;
	T s = 1s;
	return s.count();
}


#endif // TIMER_H_
