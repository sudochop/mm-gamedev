#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "sdl.h"

namespace sdl {


int renderTextSolid(
	const RendererPointer 	&renderer,
	const FontPointer 		&font,
	int 					x,
	int 					y,
	const 					std::string &msg,
	SDL_Color 				fg) {

	static SDL_Rect text_rect;

	// Create text surface.
	auto text_surface { unique_ptr(
		TTF_RenderText_Solid(font.get(), msg.c_str(), fg)
	)};

	// Create text texture.
	auto text_texture { unique_ptr(
		SDL_CreateTextureFromSurface(renderer.get(), text_surface.get())
	)};

	SDL_QueryTexture(text_texture.get(), nullptr, nullptr, &text_rect.w, &text_rect.h);
	text_rect.x = x;
	text_rect.y = y;

	return SDL_RenderCopy(renderer.get(), text_texture.get(), nullptr, &text_rect);

}


// TODO: Move
int renderDebugText(
	const RendererPointer 	&renderer,
	int 					x,
	int 					y,
	const 					std::string &msg) {

	static auto font { unique_ptr(
		TTF_OpenFont("src/resources/fonts/Inconsolata-Regular.ttf", 14)
	)};

	return renderTextSolid(renderer, font, x, y, msg, {255, 255, 255});
}


} // namespace sdl

#endif // GRAPHICS_H_
