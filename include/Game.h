#pragma once

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <memory>
#include <string>
#include <unordered_map>

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
};
class Game {
public:
	Game();
	void run();

private:
	void setup();
	void loadResources();
	void loop();

	using window = std::unique_ptr<SDL_Window, sdl_deleter>;
	using renderer = std::shared_ptr<SDL_Renderer>;
	using texture = std::unique_ptr<SDL_Texture, sdl_deleter>;
	using surface = std::unique_ptr<SDL_Surface, sdl_deleter>;

	window mWindow;
	renderer mRenderer;

	struct Resource {
		std::string name;
		std::string path;
	};
	enum class ResourceType {
		TEXTURE, IMAGE, AUDIO
	};

	std::unordered_map<ResourceType, Resource> mResourceList;
	std::unordered_map<std::string, texture> mTextureHolder;
};
