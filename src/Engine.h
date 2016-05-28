#ifndef ENGINE_H_
#define ENGINE_H_

#include <SDL2/SDL.h>
#include "sdl.h"
#include "Config.h"
#include "Timer.h"


class Engine {
public:
	Engine(const Config&);
	~Engine();

	bool Tick();

private:
	const Config& config_;

	Timer<nanoseconds> timer_;
	
	sdl::WindowPointer 		window_;
	sdl::RendererPointer 	renderer_;

	static const int kFpsAverage = 100;

};


Engine::Engine(const Config &config): config_(config) {

	sdl::init();

	this->window_ = sdl::unique_ptr(
		SDL_CreateWindow(
			"Game",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			this->config_.window_width(),
			this->config_.window_height(),
			SDL_WINDOW_SHOWN
		)
	);

	this->renderer_ = sdl::unique_ptr(
		SDL_CreateRenderer(
			window_.get(),
			-1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
		)
	);

	SDL_RenderSetLogicalSize(this->renderer_.get(), this->config_.window_width(), this->config_.window_height());
	SDL_SetRenderDrawColor(this->renderer_.get(), 0, 0, 0, 255);

}


Engine::~Engine() {

	sdl::quit();

}


bool Engine::Tick() {

	SDL_Event e;

	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) {
			return false;
		}
	}

	SDL_Delay(1000);
	printf("%lld\n", timer_.Tick().count());

	return true;

}


#endif // ENGINE_H_
