#include <stdio.h>
#include <stdexcept>
#include <memory>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


struct sdl_deleter {
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
	std::unique_ptr<SDL_Window, sdl_deleter> window(
		SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN),
		sdl_deleter()
	);

	if (window.get() == nullptr) {
		throw std::runtime_error(SDL_GetError());
	}


	// Load a system font.
	const std::string fontPath {"src/resources/fonts/Inconsolata-Regular.ttf"};

	std::unique_ptr<TTF_Font, sdl_deleter> font(
		TTF_OpenFont(fontPath.c_str(), 14),
		sdl_deleter()
	);

	if (font == nullptr) {
		throw std::runtime_error("Failed to load font at " + fontPath);
	}


	// Create renderer.
	std::unique_ptr<SDL_Renderer, sdl_deleter> renderer(
		SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED),
		sdl_deleter()
	);

	if (renderer.get() == nullptr) {
		throw std::runtime_error(SDL_GetError());
	}


	// Set renderer size and clear color.
	SDL_RenderSetLogicalSize(renderer.get(), window_width, window_height);
	SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);


	int frame {1};
	SDL_Rect solidRect;
	SDL_Event e;


	// Game loop.
	while (running) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				running = false;
			}
		}

		// Create text surface.
		std::unique_ptr<SDL_Surface, sdl_deleter> textSurface(
			TTF_RenderText_Solid(font.get(), ("Frame: " + std::to_string(frame)).c_str(), {255, 255, 255}),
			sdl_deleter()
		);

		if (textSurface.get() == nullptr) {
			throw std::runtime_error(SDL_GetError());
		}


		// Create text texture.
		std::unique_ptr<SDL_Texture, sdl_deleter> textTexture(
			SDL_CreateTextureFromSurface(renderer.get(), textSurface.get()),
			sdl_deleter()
		);

		if (textTexture.get() == nullptr) {
			throw std::runtime_error(SDL_GetError());
		}


		// Set Rect dimensions based on font.
		SDL_QueryTexture(textTexture.get(), nullptr, nullptr, &solidRect.w, &solidRect.h);
		solidRect.x = 0;
		solidRect.y = 0;


		// Render font.
		SDL_RenderClear(renderer.get());
		SDL_RenderCopy(renderer.get(), textTexture.get(), nullptr, &solidRect); 
		SDL_RenderPresent(renderer.get());


		frame++;

	}

	SDL_Quit();

	return 0;

}
