#include <stdio.h>
#include <stdexcept>
#include <memory>
#include <string>
#include <array>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


#define FPS_AVERAGE_OF 100


struct SDLDeleter {
	void operator()(SDL_Window*		p) const { SDL_DestroyWindow(p); }
	void operator()(SDL_Renderer*	p) const { SDL_DestroyRenderer(p); }
	void operator()(SDL_Texture*	p) const { SDL_DestroyTexture(p); }
	void operator()(SDL_Surface*	p) const { SDL_FreeSurface(p); }
	void operator()(TTF_Font*		p) const { TTF_CloseFont(p); }
};

using SDLWindowPointer 		= std::unique_ptr<SDL_Window, 	SDLDeleter>;
using SDLRendererPointer 	= std::unique_ptr<SDL_Renderer, SDLDeleter>;
using SDLTexturePointer 	= std::unique_ptr<SDL_Texture, 	SDLDeleter>;
using SDLSurfacePointer 	= std::unique_ptr<SDL_Surface, 	SDLDeleter>;
using SDLFontPointer 		= std::unique_ptr<TTF_Font, 	SDLDeleter>;


int renderDebugText(const SDLRendererPointer &renderer, int x, int y, const std::string &msg) {

	static const std::string fontpath {"src/resources/fonts/Inconsolata-Regular.ttf"};

	static SDLFontPointer font(
		TTF_OpenFont(fontpath.c_str(), 14),
		SDLDeleter()
	);

	if (font == nullptr) {
		throw std::runtime_error("Failed to load font at " + fontpath);
	}


	// Create text surface.
	SDLSurfacePointer text_surface(
		TTF_RenderText_Solid(font.get(), msg.c_str(), {255, 255, 255}),
		SDLDeleter()
	);

	if (text_surface.get() == nullptr) {
		throw std::runtime_error(SDL_GetError());
	}


	// Create text texture.
	SDLTexturePointer text_texture(
		SDL_CreateTextureFromSurface(renderer.get(), text_surface.get()),
		SDLDeleter()
	);

	if (text_texture.get() == nullptr) {
		throw std::runtime_error(SDL_GetError());
	}


	static SDL_Rect text_rect;

	SDL_QueryTexture(text_texture.get(), nullptr, nullptr, &text_rect.w, &text_rect.h);
	text_rect.x = x;
	text_rect.y = y;

	return SDL_RenderCopy(renderer.get(), text_texture.get(), nullptr, &text_rect);

}


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
	SDLWindowPointer window(
		SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN),
		SDLDeleter()
	);

	if (window.get() == nullptr) {
		throw std::runtime_error(SDL_GetError());
	}


	// Create renderer.
	SDLRendererPointer renderer(
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

	SDL_Quit();

	return 0;

}
