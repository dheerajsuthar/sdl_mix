#include <Game.h>
#include <SDL_error.h>
#include <SDL_image.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_video.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <utility>
#include <unistd.h>
#include <string>


Game::Game(){
		std::cout << "Game on" << "\n";
		mResourceList.insert(std::make_pair(
				ResourceType::TEXTURE,
				Resource{"adventurer",std::string(SDL_GetBasePath()) + "assets/sprite/adventurer.png"})
		);
	}

void Game::run(){
	setup();
	loadResources();
	loop();
}

void Game::setup(){
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "couldn't start SDL\n";
		exit(1);
	}
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;
	mWindow.reset(SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN ));
	if(!mWindow){
		std::cout << "window not created\n";
	}
	mRenderer.reset(SDL_CreateRenderer( mWindow.get(), -1, SDL_RENDERER_ACCELERATED ),sdl_deleter());
	            if( !mRenderer )
	            {
	                std::cout <<  "Renderer could not be created! SDL Error:", SDL_GetError() ;
	            }

	std::cout << "SDL up and running\n";
}

void Game::loadResources(){
	char path[256];
	for(auto const& resource: mResourceList){
		surface tempSurface(IMG_Load( resource.second.path.c_str()), sdl_deleter() );
		if(!tempSurface){
					std::cout << "Couldn't load surface.\n" << IMG_GetError();
					getcwd(path,sizeof(path));
					std::cout << "cwd: " << path << " sdl base path: " << SDL_GetBasePath() << "\n";
		}
		texture tempTexture( SDL_CreateTextureFromSurface( mRenderer.get(), tempSurface.release()));
		if(!tempTexture){
			std::cout << "Couldn't load texture.\n";
		}
		mTextureHolder.insert(std::make_pair(resource.second.name, std::move(tempTexture)));
		std::cout << "Texture loaded\n";
	}

}

void Game::loop(){
	bool stop = false;
	SDL_Event e;
	while(!stop){
		while(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_QUIT){
				stop = true;
			}
		}

		//clear screen
		SDL_RenderClear(mRenderer.get());

		//render texture
		SDL_RenderCopy(mRenderer.get(),mTextureHolder["adventurer"].get(),NULL,NULL);

		//update screen
		SDL_RenderPresent(mRenderer.get());
	}
}

