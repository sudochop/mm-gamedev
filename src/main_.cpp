#include <stdio.h>
#include <stdexcept>
#include <memory>
#include <string>
#include <array>
#include <chrono>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "sdl.h"
#include "graphics.h"

#define FPS_AVERAGE_OF 100


int main(int argc, char* argv[]) {

	using std::chrono::high_resolution_clock;
	using std::chrono::microseconds;
	using std::chrono::duration_cast;

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
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
		))
	};


	// Set renderer size and clear color.
	SDL_RenderSetLogicalSize(renderer.get(), window_width, window_height);
	SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);


	// Random background image to test SDL_Image.
	auto img_bg { sdl::unique_ptr(
		IMG_LoadTexture(renderer.get(), "src/resources/background.png")
	)};

	int img_bg_w, img_bg_h;
	SDL_QueryTexture(img_bg.get(), nullptr, nullptr, &img_bg_w, &img_bg_h);
	
	SDL_Rect img_bg_rect;
	img_bg_rect.x = 0;
	img_bg_rect.x = 0;
	img_bg_rect.w = img_bg_w;
	img_bg_rect.h = img_bg_h;


	// FPS stuff.
	auto fps {0};
	auto fps_frame_count {0};
	auto fps_time_last {high_resolution_clock::now()};
	std::array<microseconds, FPS_AVERAGE_OF> fps_times;
	

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

		
		// Render background image.
		SDL_RenderCopy(renderer.get(), img_bg.get(), NULL, &img_bg_rect);


		// Calculate and draw average fps.
		int fps_times_index {fps_frame_count % FPS_AVERAGE_OF};
		const auto fps_time_current {high_resolution_clock::now()};

		fps_times[fps_times_index] = duration_cast<microseconds>(fps_time_current - fps_time_last);
		fps_time_last = fps_time_current;

		fps = 0;
		for(const auto& i: fps_times) {
			fps += i.count();
		}

		fps /= FPS_AVERAGE_OF;
		fps = 1000000.f / fps;


		// Render some stats.
		sdl::renderDebugText(renderer, 0, 0, "Frame: " + std::to_string(fps_frame_count));
		sdl::renderDebugText(renderer, 0, 16, "FPS:   " + std::to_string((int)fps));
		
		// Update screen.
		SDL_RenderPresent(renderer.get());

		// Update frame count.
		++fps_frame_count;

	}

	sdl::quit();

	return 0;

}
