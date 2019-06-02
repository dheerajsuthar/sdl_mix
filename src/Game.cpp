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
#include "PlayerAction.h"

Game::Game():mFPSStartTime(0),
    mGameState(GameState::STOPPED){
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
        std::cout << "Renderer could not be created! SDL Error:" <<  SDL_GetError();
    }
    if (TTF_Init() == -1) {
        std::cout << "TTF engined couldn't be initiated! TTF Error:" << TTF_GetError();
    }
    std::string fontPath = std::string(SDL_GetBasePath())
            + "assets/font/OpenSans-Regular.ttf";
    std::cout << "font: " << fontPath << "\n";
    mFont.reset(TTF_OpenFont(fontPath.c_str(), FONT_SIZE), sdl_deleter());
    if (!mFont) {
        std::cout << "Font could not be created! TTF Error:" << TTF_GetError();
    }

    std::cout << "SDL up and running\n";
}

void Game::showFPS(uint32_t currentTime) {
    std::stringstream fps;
    uint32_t fpsDelta = currentTime - mFPSStartTime;
    mFPSStartTime = currentTime;
    fps << "Frame rate: " << (1000.f / fpsDelta);
    texture fpsTexture = textureFromText(fps.str(),
    { 0xFF, 0xFF, 0xFF, 0xFF });

    int textWidth;
    int textHeight;
    SDL_QueryTexture(fpsTexture.get(), NULL, NULL, &textWidth, &textHeight);
    SDL_Rect tgtForText = { FPS_X, FPS_Y, textWidth, textHeight };
    SDL_RenderCopy(mRenderer.get(), fpsTexture.get(), NULL, &tgtForText);
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
    //    (renderer renderer, texture texture, uint32_t width,
    //            uint32_t height,  uint32_t fps,
    //            bool loop)
    mPlayer = std::make_unique<Sprite>(mRenderer,
                                       std::move(mTextureHolder["adventurer"]), 50, 37, 10);
    std::vector<Sprite::offset> idle{{0,0},{50,0},{100,0},{150,0}};
    std::vector<Sprite::offset> run{
        {50,37},
        {100,37},
        {150,37},
        {200,37},
        {250,37},
        {300,37}
    };
    std::vector<Sprite::offset> jump{
        {100, 74},
        {150, 74},
        {200, 74},
        {250, 74},
        {300, 74},
        {0, 101},
        {50, 101},
        {100, 101}
    };
    mPlayer->setAction(static_cast<uint32_t>(PlayerAction::IDLE1),idle);
    mPlayer->setAction(static_cast<uint32_t>(PlayerAction::RUN), run);
    mPlayer->setAction(static_cast<uint32_t>(PlayerAction::JUMP), jump);
    mPlayerVelX = mPlayerVelY = 0;
    SDL_Rect initialTgt = mPlayer->tgt();
    initialTgt.y = 100;
    mPlayer->setTgt(initialTgt);
    std::cout << "Player loaded\n";
}

void Game::updatePlayerAction(PlayerAction action)
{
    uint32_t newAction = static_cast<uint32_t>(action);

    if(mPlayerCurrentAction != newAction){
        mPlayer->runAction(newAction);
        mPlayerCurrentAction = newAction;
    }
}

void Game::handleEvent()
{
    SDL_Event e;
    SDL_Rect playerTarget = mPlayer->tgt();


    while (SDL_PollEvent(&e) != 0) {
        switch (e.type) {
        case SDL_QUIT:
            mGameState = GameState::STOPPED;
            break;
        case SDL_KEYDOWN:
            if(e.key.repeat == 0 ){
                switch (e.key.keysym.sym) {
                case SDLK_RIGHT:
                    std::cout << "Prsng right\n";
                    mPlayerVelX += PLAYER_VELOCITY;
                    updatePlayerAction(PlayerAction::RUN);
                    if(mPlayerVelX > 0)
                    std::cout << "x: " << mPlayerVelX << "y: " << mPlayerVelY << "\n";
                    break;
                case SDLK_LEFT:
                    std::cout << "Prsng left\n";
                    mPlayerVelX -= PLAYER_VELOCITY;
                    updatePlayerAction(PlayerAction::RUN) ;
                    break;
                default:
                    break;
                }
            }
            break;
        case SDL_KEYUP:
            if(e.key.repeat == 0 ){
                switch (e.key.keysym.sym) {
                case SDLK_RIGHT:
                    std::cout << "Prsng right - \n";
                    mPlayerVelX -= PLAYER_VELOCITY;
                    break;
                case SDLK_LEFT:
                    std::cout << "Prsng left - \n";
                    mPlayerVelX += PLAYER_VELOCITY;
                    break;
                default:
                    break;
                }
            }
            break;
        default:
            break;

        };
    }


    if(mPlayerVelX == 0 && mPlayerVelY == 0){
        updatePlayerAction(PlayerAction::IDLE1);
    }
    playerTarget.x += mPlayerVelX;
    playerTarget.y += mPlayerVelY;

    mPlayer->setTgt(playerTarget);
}

void Game::loop() {
    uint32_t currentTime = 0;
    mPlayer->runAction(static_cast<uint32_t>(PlayerAction::IDLE1));
    mGameState = GameState::RUNNING;

    while (mGameState == GameState::RUNNING) {
        handleEvent();

        //clear screen
        SDL_RenderClear(mRenderer.get());

        //render texture
        //SDL_RenderCopy(mRenderer.get(),mTextureHolder["adventurer"].get(),NULL,NULL);
        currentTime = SDL_GetTicks();
        mPlayer->renderFrame(currentTime);

        //update frame rate
        showFPS(currentTime);

        //update screen
        SDL_RenderPresent(mRenderer.get());
    }
}

Game::~Game() = default;
