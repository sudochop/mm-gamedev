#ifndef ENGINE_H_
#define ENGINE_H_

#include <array>
#include <algorithm>
#include <SDL2/SDL.h>
#include "sdl.h"
#include "graphics.h"
#include "Config.h"
#include "Timer.h"


class Engine {
public:
	Engine(const Config&);
	~Engine();

	bool Tick();

private:
	void RenderDebug(nanoseconds);

	const Config& config_;

	Timer<nanoseconds> timer_;
	
	sdl::WindowPointer 		window_;
	sdl::RendererPointer 	renderer_;

	static const int kTickAverage = 50;
	std::array<int, kTickAverage> tick_times_;

};


Engine::Engine(const Config &config): config_(config) {

	sdl::init();

	window_ = sdl::unique_ptr(
		SDL_CreateWindow(
			"Game",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			config_.window_width(),
			config_.window_height(),
			SDL_WINDOW_SHOWN
		)
	);

	renderer_ = sdl::unique_ptr(
		SDL_CreateRenderer(
			window_.get(),
			-1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
		)
	);

	SDL_RenderSetLogicalSize(renderer_.get(), config_.window_width(), config_.window_height());
	SDL_SetRenderDrawColor(renderer_.get(), 0, 0, 0, 255);

}


Engine::~Engine() {

	sdl::quit();

}


bool Engine::Tick() {

	auto delta = timer_.Tick();

	SDL_Event e;

	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) {
			return false;
		}
	}

	SDL_RenderClear(renderer_.get());

	RenderDebug(delta);

	SDL_RenderPresent(renderer_.get());

	return true;

}


void Engine::RenderDebug(nanoseconds delta) {

	std::rotate(
		std::begin(tick_times_),
		std::next(std::begin(tick_times_)),
		std::end(tick_times_)
	);
	*std::rbegin(tick_times_) = delta.count();


	int fps = 0;
	for(const auto& i: tick_times_) {
		fps += i;
	}

	fps /= kTickAverage;
	fps = timer_.second() / fps;

	sdl::renderDebugText(renderer_, 0, 16, "FPS:   " + std::to_string(fps));

}


#endif // ENGINE_H_
