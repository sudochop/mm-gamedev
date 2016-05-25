#include <stdio.h>
#include <stdexcept>
#include <memory>
#include <string>
#include <array>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "sdl.h"

#define FPS_AVERAGE_OF 100


int renderDebugText(const sdl::RendererPointer &renderer, int x, int y, const std::string &msg);


int main(int argc, char* argv[]) {

	bool running {true};
	const int window_width {640};
	const int window_height {480};


	sdl::init();


	// Create our window.
	auto window { sdl::unique_ptr(
		SDL_CreateWindow(
			"Game",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			window_width,
			window_height,
			SDL_WINDOW_SHOWN
		))
	};


	// Create renderer.
	auto renderer { sdl::unique_ptr(
		SDL_CreateRenderer(
			window.get(),
			-1,
			SDL_RENDERER_ACCELERATED
		))
	};


	// Set renderer size and clear color.
	SDL_RenderSetLogicalSize(renderer.get(), window_width, window_height);
	SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);


	// FPS stuff.
	float 	fps {0};
	Uint32 	fps_frame_count {0};
	Uint32 	fps_time_last {SDL_GetTicks()};

	std::array<Uint32, FPS_AVERAGE_OF> fps_times;
	fps_times.fill(0);
	

	// Game loop.
	SDL_Event e;
	while (running) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				running = false;
			}
		}

		// Clear renderer.
		SDL_RenderClear(renderer.get());

		
		// Do stuff..


		// Calculate and draw average fps.
		Uint32 fps_times_index = fps_frame_count % FPS_AVERAGE_OF;
		Uint32 fps_time_current = SDL_GetTicks();

		fps_times[fps_times_index] = fps_time_current - fps_time_last;
		fps_time_last = fps_time_current;

		fps = 0;
		for(const auto& i: fps_times) {
			fps += i;
		}

		fps /= FPS_AVERAGE_OF;
		fps = 1000.f / fps;


		// Render some stats.
		renderDebugText(renderer, 0, 0, "Frame: " + std::to_string(fps_frame_count));
		renderDebugText(renderer, 0, 16, "FPS: " + std::to_string((int)fps));
		
		// Update screen.
		SDL_RenderPresent(renderer.get());

		// Update frame count.
		++fps_frame_count;

	}

	sdl::quit();

	return 0;

}


int renderDebugText(const sdl::RendererPointer &renderer, int x, int y, const std::string &msg) {

	static SDL_Rect text_rect;

	// Open font.
	static auto font { sdl::unique_ptr(
		TTF_OpenFont("src/resources/fonts/Inconsolata-Regular.ttf", 14)
	)};

	// Create text surface.
	auto text_surface { sdl::unique_ptr(
		TTF_RenderText_Solid(font.get(), msg.c_str(), {255, 255, 255})
	)};

	// Create text texture.
	auto text_texture { sdl::unique_ptr(
		SDL_CreateTextureFromSurface(renderer.get(), text_surface.get())
	)};

	SDL_QueryTexture(text_texture.get(), nullptr, nullptr, &text_rect.w, &text_rect.h);
	text_rect.x = x;
	text_rect.y = y;

	return SDL_RenderCopy(renderer.get(), text_texture.get(), nullptr, &text_rect);

}
