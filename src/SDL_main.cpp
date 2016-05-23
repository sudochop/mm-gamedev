#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char* argv[]) {

	bool running = true;

	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {

		printf("SDL could not initialize. SDL_Error: %s\n", SDL_GetError());

	} else {

		window = SDL_CreateWindow("SDL Skeleton", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 400, 300, SDL_WINDOW_SHOWN);

		if(window == NULL) {

			printf("Window could not be created. SDL_Error: %s\n", SDL_GetError());

		} else {

			SDL_Event e;

			screenSurface = SDL_GetWindowSurface(window);

			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));

			SDL_UpdateWindowSurface(window);

			while(running) {
				while(SDL_PollEvent(&e) != 0) {
					if(e.type == SDL_QUIT) {
						running = false;
					}
				}
			}
		}
	}

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;

}
