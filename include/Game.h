#pragma once


#include <string>
#include <unordered_map>
#include "SDL.h"

class Sprite;

class Game {
public:
	Game();
	~Game();
	void run();

	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;

private:
	void setup();
	void loadResources();
	void createWorld();
	void loop();
	texture textureFromText(std::string text, SDL_Color color);

	window mWindow;
	renderer mRenderer;

	using sprite = std::unique_ptr<Sprite>;
	sprite mPlayer;
	texture mFrameRate;
	font mFont;

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
