#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdexcept>
#include <memory>

int main(int argc, char* argv[]) {

	bool running {true};

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init\n");
        throw std::runtime_error(SDL_GetError());
    }

    if (SDL_ShowCursor(SDL_DISABLE) < 0) {
        printf("SDL_ShowCursor\n");
        throw std::runtime_error(SDL_GetError());
    }

	std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window(
		SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN), 
		SDL_DestroyWindow
	);

	if(window == NULL) {
		printf("Window could not be created\n");
		throw std::runtime_error(SDL_GetError());
	}

	SDL_Event e;

	SDL_Surface* screenSurface = SDL_GetWindowSurface(window.get());

	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));

	SDL_UpdateWindowSurface(window.get());

	while(running) {
		while(SDL_PollEvent(&e) != 0) {
			if(e.type == SDL_QUIT) {
				running = false;
			}
		}
	}

	SDL_Quit();

	return 0;

}
