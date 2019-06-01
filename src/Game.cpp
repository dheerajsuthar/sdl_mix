#include <Game.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <utility>
#include <unistd.h>
#include <string>
#include <sstream>

#include "Sprite.h"
#include "SDL_ttf.h"

Game::Game() {
	std::cout << "Game on" << "\n";
	mResourceList.insert(
			std::make_pair(ResourceType::TEXTURE,
					Resource { "adventurer", std::string(SDL_GetBasePath())
							+ "assets/sprite/adventurer.png" }));
}

void Game::run() {
	setup();
	loadResources();
	createWorld();
	loop();
}

void Game::setup() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "couldn't start SDL\n";
		exit(1);
	}

	mWindow.reset(SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
	SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
	if (!mWindow) {
		std::cout << "window not created\n";
	}
	mRenderer.reset(
			SDL_CreateRenderer(mWindow.get(), -1,
					SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
			sdl_deleter());
	if (!mRenderer) {
		std::cout << "Renderer could not be created! SDL Error:", SDL_GetError();
	}
	if (TTF_Init() == -1) {
		std::cout << "TTF engined couldn't be initiated! TTF Error:", TTF_GetError();
	}
	std::string fontPath = std::string(SDL_GetBasePath())
			+ "assets/font/OpenSans-Regular.ttf";
	std::cout << "font: " << fontPath << "\n";
	mFont.reset(TTF_OpenFont(fontPath.c_str(), 28), sdl_deleter());
	if (!mFont) {
		std::cout << "Font could not be created! TTF Error:", TTF_GetError();
	}

	std::cout << "SDL up and running\n";
}
texture Game::textureFromText(std::string text, SDL_Color color) {
	surface textSurface(TTF_RenderText_Solid(mFont.get(), text.c_str(), color),
			sdl_deleter());
	if (!textSurface) {
		std::cout << "Unable to create surface ! SDL Error:" << SDL_GetError();
	}
	texture tempTexture(
			SDL_CreateTextureFromSurface(mRenderer.get(), textSurface.get()),
			sdl_deleter());
	if (!tempTexture) {
		std::cout << "Unable to create texture from rendered text! SDL Error:"
				<< SDL_GetError();
	}
	return tempTexture;
}

void Game::loadResources() {
	char path[256];
	for (auto const& resource : mResourceList) {
		surface tempSurface(IMG_Load(resource.second.path.c_str()),
				sdl_deleter());
		if (!tempSurface) {
			std::cout << "Couldn't load surface.\n" << IMG_GetError();
			getcwd(path, sizeof(path));
			std::cout << "cwd: " << path << " sdl base path: "
					<< SDL_GetBasePath() << "\n";
		}
		texture tempTexture(
				SDL_CreateTextureFromSurface(mRenderer.get(),
						tempSurface.release()));
		if (!tempTexture) {
			std::cout << "Couldn't load texture.\n";
		}
		mTextureHolder.insert(
				std::make_pair(resource.second.name, std::move(tempTexture)));
		std::cout << "Texture loaded\n";
	}

}

void Game::createWorld() {
	std::cout << "Player laoding\n";

	mPlayer = std::make_unique<Sprite>(mRenderer,
			std::move(mTextureHolder["adventurer"]), 50, 37, 0, 3, 10, true, 4);
	std::cout << "Player loaded\n";
}

void Game::loop() {
	bool stop = false;
	uint32_t fpsStartTime = SDL_GetTicks();
	uint32_t playerFPS = 1;
	uint32_t currentTime = 0;
	SDL_Event e;
	while (!stop) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				stop = true;
			}
		}

		//clear screen
		SDL_RenderClear(mRenderer.get());

		//render texture
//		SDL_RenderCopy(mRenderer.get(),mTextureHolder["adventurer"].get(),NULL,NULL);
		currentTime = SDL_GetTicks();
		mPlayer->renderFrame(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, currentTime);

//update frame rate
		std::stringstream fps;
		uint32_t fpsDelta = currentTime - fpsStartTime;
		fpsStartTime = currentTime;
		fps << "Frame rate: " << (1000.f / fpsDelta);
		texture fpsTexture = textureFromText(fps.str(),
				{ 0xFF, 0xFF, 0xFF, 0xFF });

		int textWidth;
		int textHeight;
		SDL_QueryTexture(fpsTexture.get(), NULL, NULL, &textWidth, &textHeight);
		SDL_Rect tgtForText = { 10, 10, textWidth, textHeight };
		SDL_RenderCopy(mRenderer.get(), fpsTexture.get(), NULL, &tgtForText);
		//update screen
		SDL_RenderPresent(mRenderer.get());
	}
}

Game::~Game() = default;
