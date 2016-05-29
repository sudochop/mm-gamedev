#ifndef INPUT_H_
#define INPUT_H_

#include <map>
#include <SDL2/SDL.h>
#include "Timer.h"


template<typename T>
class Input {
public:
	void PollEvent();
	bool IsPressed(SDL_Scancode key);
	bool IsReleased(SDL_Scancode key);
	bool SignalQuit();
	T Elapsed(SDL_Scancode key);
private:
	SDL_Event e;
	bool signal_quit_ = false;
	std::map<SDL_Scancode, Timer<T>> key_times_;
	std::map<SDL_Scancode, bool> keys_pressed_;
	std::map<SDL_Scancode, bool> keys_released_;
};


template<typename T>
void Input<T>::PollEvent() {
	
	keys_released_.clear();
	signal_quit_ = false;

	while (SDL_PollEvent(&e) != 0) {
		switch (e.type) {
		case SDL_QUIT:
			signal_quit_ = true;
			break;
		case SDL_KEYDOWN:
			keys_pressed_[e.key.keysym.scancode] = true;
			key_times_[e.key.keysym.scancode].Start();
			break;
		case SDL_KEYUP:
			keys_released_[e.key.keysym.scancode] = true;
			keys_pressed_[e.key.keysym.scancode] = false;
			key_times_[e.key.keysym.scancode].Reset();
			break;
		default:
			break;
		}
	}
}

template<typename T>
bool Input<T>::IsPressed(SDL_Scancode key) {
	return keys_pressed_[key];
}

template<typename T>
bool Input<T>::IsReleased(SDL_Scancode key) {
	return keys_released_[key];
}

template<typename T>
T Input<T>::Elapsed(SDL_Scancode key) {
	return key_times_[key].Elapsed();
}

template<typename T>
bool Input<T>::SignalQuit() {
	return signal_quit_;
}


#endif
