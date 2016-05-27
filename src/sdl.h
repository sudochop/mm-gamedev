#ifndef SDL_H_
#define SDL_H_

#include <memory>
#include <type_traits>
#include <stdexcept>
#include <system_error>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace sdl {


struct Deleter {
	void operator()(SDL_Window*		p) const { SDL_DestroyWindow(p); }
	void operator()(SDL_Renderer*	p) const { SDL_DestroyRenderer(p); }
	void operator()(SDL_Texture*	p) const { SDL_DestroyTexture(p); }
	void operator()(SDL_Surface*	p) const { SDL_FreeSurface(p); }
	void operator()(TTF_Font*		p) const { TTF_CloseFont(p); }
};


using WindowPointer 	= std::unique_ptr<SDL_Window, 	Deleter>;
using RendererPointer 	= std::unique_ptr<SDL_Renderer, Deleter>;
using TexturePointer 	= std::unique_ptr<SDL_Texture, 	Deleter>;
using SurfacePointer 	= std::unique_ptr<SDL_Surface, 	Deleter>;
using FontPointer 		= std::unique_ptr<TTF_Font, 	Deleter>;


template<typename T>
auto unique_ptr(T* t) {
	if (!t) { throw std::system_error(errno, std::generic_category()); }
	return std::unique_ptr<std::decay_t<T>, decltype(Deleter())>(t, Deleter());
}


void init() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0 ||
		SDL_ShowCursor(SDL_DISABLE) < 0 ||
		TTF_Init() < 0) {
		
		throw std::runtime_error(SDL_GetError());
	}
}


void quit() {
	SDL_Quit();
}


} // namespace sdl

#endif // SDL_H_
