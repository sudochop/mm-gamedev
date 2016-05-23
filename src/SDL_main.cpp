#include <SDL2/SDL.h>
#include <stdio.h>
#include <memory>

int main(int argc, char* argv[]) {

	bool running = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {

		printf("SDL could not initialize. SDL_Error: %s\n", SDL_GetError());

	} else {

		std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window(
        	SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN), 
        	SDL_DestroyWindow
        );

		if(window == NULL) {

			printf("Window could not be created. SDL_Error: %s\n", SDL_GetError());

		} else {

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
		}
	}

	SDL_Quit();

	return 0;

}
