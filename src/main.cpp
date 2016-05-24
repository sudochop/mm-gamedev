#include <stdio.h>
#include <stdexcept>
#include <memory>
#include <string>
#include <array>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


#define FPS_AVERAGE_OF 100


struct SDLDeleter {
	void operator()(SDL_Window *p) const { SDL_DestroyWindow(p); }
	void operator()(SDL_Renderer *p) const { SDL_DestroyRenderer(p); }
	void operator()(SDL_Texture *p) const { SDL_DestroyTexture(p); }
	void operator()(SDL_Surface *p) const { SDL_FreeSurface(p); }
	void operator()(TTF_Font *p) const { TTF_CloseFont(p); }
};


int main(int argc, char* argv[]) {

	bool running {true};
	const int window_width {640};
	const int window_height {480};


	// Initialize video subsystem.
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		throw std::runtime_error("SDL_Init");
	}

	// Make sure we have a cursor.
	if (SDL_ShowCursor(SDL_DISABLE) < 0) {
		throw std::runtime_error("SDL_ShowCursor");
	}

	// Initialize the truetype font API. 
	if (TTF_Init() < 0) {
		throw std::runtime_error(TTF_GetError());
	}


	// Create our window.
	std::unique_ptr<SDL_Window, SDLDeleter> window(
		SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN),
		SDLDeleter()
	);

	if (window.get() == nullptr) {
		throw std::runtime_error(SDL_GetError());
	}


	// Load a system font.
	const std::string fontpath {"src/resources/fonts/Inconsolata-Regular.ttf"};

	std::unique_ptr<TTF_Font, SDLDeleter> font(
		TTF_OpenFont(fontpath.c_str(), 14),
		SDLDeleter()
	);

	if (font == nullptr) {
		throw std::runtime_error("Failed to load font at " + fontpath);
	}


	// Create renderer.
	std::unique_ptr<SDL_Renderer, SDLDeleter> renderer(
		SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED),
		SDLDeleter()
	);

	if (renderer.get() == nullptr) {
		throw std::runtime_error(SDL_GetError());
	}


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

		
		// Do stuff..


		// Calculate and draw average fps.
		Uint32 fps_times_index = fps_frame_count % FPS_AVERAGE_OF;
		Uint32 fps_time_current = SDL_GetTicks();

		fps_times[fps_times_index] = fps_time_current - fps_time_last;
		fps_time_last = fps_time_current;

		++fps_frame_count;

		fps = 0;
		for(const auto& i: fps_times) {
			fps += i;
		}

		fps /= FPS_AVERAGE_OF;
		fps = 1000.f / fps;


		// Create text surface.
		std::unique_ptr<SDL_Surface, SDLDeleter> text_surface(
			TTF_RenderText_Solid(font.get(), ("FPS: " + std::to_string(fps)).c_str(), {255, 255, 255}),
			SDLDeleter()
		);

		if (text_surface.get() == nullptr) {
			throw std::runtime_error(SDL_GetError());
		}


		// Create text texture.
		std::unique_ptr<SDL_Texture, SDLDeleter> text_texture(
			SDL_CreateTextureFromSurface(renderer.get(), text_surface.get()),
			SDLDeleter()
		);

		if (text_texture.get() == nullptr) {
			throw std::runtime_error(SDL_GetError());
		}

		static SDL_Rect text_rect;

		// Set Rect dimensions based on font.
		SDL_QueryTexture(text_texture.get(), nullptr, nullptr, &text_rect.w, &text_rect.h);
		text_rect.x = 0;
		text_rect.y = 0;


		// Render font.
		SDL_RenderClear(renderer.get());
		SDL_RenderCopy(renderer.get(), text_texture.get(), nullptr, &text_rect); 
		SDL_RenderPresent(renderer.get());


	}

	SDL_Quit();

	return 0;

}
