#ifndef ENGINE_H_
#define ENGINE_H_

#include <array>
#include <algorithm>
#include <SDL2/SDL.h>
#include "sdl.h"
#include "graphics.h"
#include "Config.h"
#include "Timer.h"
#include "Input.h"

typedef nanoseconds TimePrecision;



class Engine {
public:
	Engine(const Config&);
	~Engine();

	bool Tick();

private:
	void RenderDebug(TimePrecision);

	const Config& config_;

	Timer<TimePrecision> timer_;
	Input<TimePrecision> input_;

	SDL_Event event_;
	
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

	timer_.Start();

}


Engine::~Engine() {

	sdl::quit();

}


bool Engine::Tick() {

	auto delta = timer_.Tick();

	static auto up = 0.0f;
	static auto down = 0.0f;
	static auto left = 0.0f;
	static auto right = 0.0f;


	input_.PollEvent(event_);

	if (input_.SignalQuit()) {
		return false;
	}

	if (input_.IsPressed(SDL_SCANCODE_UP)) {
		up = input_.Elapsed(SDL_SCANCODE_UP).count() / (double)timer_.second();
	}
	if (input_.IsPressed(SDL_SCANCODE_DOWN)) {
		down = input_.Elapsed(SDL_SCANCODE_DOWN).count() / (double)timer_.second();
	}
	if (input_.IsPressed(SDL_SCANCODE_LEFT)) {
		left = input_.Elapsed(SDL_SCANCODE_LEFT).count() / (double)timer_.second();
	}
	if (input_.IsPressed(SDL_SCANCODE_RIGHT)) {
		right = input_.Elapsed(SDL_SCANCODE_RIGHT).count() / (double)timer_.second();
	}

	if (input_.IsReleased(SDL_SCANCODE_UP)) {
		up = 0.0f;
	}
	if (input_.IsReleased(SDL_SCANCODE_DOWN)) {
		down = 0.0f;
	}
	if (input_.IsReleased(SDL_SCANCODE_LEFT)) {
		left = 0.0f;
	}
	if (input_.IsReleased(SDL_SCANCODE_RIGHT)) {
		right = 0.0f;
	}


	SDL_RenderClear(renderer_.get());

	RenderDebug(delta);
	sdl::renderDebugText(renderer_, 0, 16, "k_U: " + std::to_string(up));
	sdl::renderDebugText(renderer_, 0, 32, "k_D: " + std::to_string(down));
	sdl::renderDebugText(renderer_, 0, 48, "k_L: " + std::to_string(left));
	sdl::renderDebugText(renderer_, 0, 64, "k_R: " + std::to_string(right));

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

	sdl::renderDebugText(renderer_, 0, 0, "FPS: " + std::to_string(fps));

}


#endif // ENGINE_H_
