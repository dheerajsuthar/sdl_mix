#pragma once


#include <string>
#include <unordered_map>
#include <unistd.h>
#include "SDL.h"

class Sprite;

class Game {
public:
	Game();
	~Game();
	void run();

	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;
	const int FONT_SIZE = 10;
	const int FPS_X = SCREEN_WIDTH-80;
	const int FPS_Y = SCREEN_HEIGHT-20;

private:
	void setup();
	void loadResources();
	void createWorld();
	void loop();
	void showFPS(uint32_t currentTime);
	texture textureFromText(std::string text, SDL_Color color);

	window mWindow;
	renderer mRenderer;

	using sprite = std::unique_ptr<Sprite>;
	sprite mPlayer;
	texture mFrameRate;
	font mFont;
	uint32_t mFPSStartTime;

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
