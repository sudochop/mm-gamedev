#ifndef ENGINE_H_
#define ENGINE_H_

#include <SDL2/SDL.h>
#include "sdl.h"


class Engine {
private:
	const Config& config;
	
	sdl::WindowPointer 		window;
	sdl::RendererPointer 	renderer;

	bool running = true;

public:
	Engine(const Config&);
	~Engine();

	void run();
};


Engine::Engine(const Config &config_): config(config_) {

	sdl::init();

	this->window = sdl::unique_ptr(
		SDL_CreateWindow(
			"Game",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			this->config.windowWidth,
			this->config.windowHeight,
			SDL_WINDOW_SHOWN
		)
	);

	this->renderer = sdl::unique_ptr(
		SDL_CreateRenderer(
			window.get(),
			-1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
		)
	);

	SDL_RenderSetLogicalSize(this->renderer.get(), this->config.windowWidth, this->config.windowHeight);
	SDL_SetRenderDrawColor(this->renderer.get(), 0, 0, 0, 255);

}


Engine::~Engine() {

	sdl::quit();

}


void Engine::run() {

	SDL_Event e;

	while (this->running) {

		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				running = false;
			}
		}

	}

}


#endif // ENGINE_H_
