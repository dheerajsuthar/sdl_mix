/*
 * SDL.h
 *
 *  Created on: 01-Jun-2019
 *      Author: dheeraj
 */

#ifndef INCLUDE_SDL_H_
#define INCLUDE_SDL_H_

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <SDL_error.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <memory>

struct sdl_deleter {
	void operator()(SDL_Window *w) {
		SDL_DestroyWindow(w);
	}
	void operator()(SDL_Renderer *w) {
			SDL_DestroyRenderer(w);
		}
	void operator()(SDL_Surface *w) {
			SDL_FreeSurface(w);
		}
	void operator()(SDL_Texture *w) {
			SDL_DestroyTexture(w);
		}
	void operator()(TTF_Font *w){
		TTF_CloseFont(w);
	}
};

using window = std::unique_ptr<SDL_Window, sdl_deleter>;
using renderer = std::shared_ptr<SDL_Renderer>;
using texture = std::unique_ptr<SDL_Texture, sdl_deleter>;
using surface = std::unique_ptr<SDL_Surface, sdl_deleter>;
using font = std::shared_ptr<TTF_Font>;

#endif /* INCLUDE_SDL_H_ */
