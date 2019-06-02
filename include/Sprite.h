/*
 * Sprite.h
 *
 *  Created on: 01-Jun-2019
 *      Author: dheeraj
 */

#ifndef INCLUDE_SPRITE_H_
#define INCLUDE_SPRITE_H_
#include "SDL.h"
#include <cstdint>
#include <map>
#include <vector>

class Sprite {
public:
	const int SCALE_FACTOR = 2;

    struct offset{
        uint32_t x;
        uint32_t y;
    };

    Sprite(renderer renderer, texture texture,
           uint32_t width, uint32_t height, uint32_t fps,
           int32_t scale = 1);
    void renderFrame(uint32_t offsetX,
			uint32_t offsetY);
    void renderFrame(uint32_t currentTime);
	void setTexture(texture texture);
    void setAction(uint32_t action,std::vector<offset> offsets );
    void runAction(uint32_t action);
    void setScale(const int32_t &scale);
    void setTgt(const SDL_Rect &tgt);

    SDL_Rect tgt() const;

    uint32_t currentAction() const;

    bool flipped() const;
    void setFlipped(bool flipped);

    bool runOnce() const;
    void setRunOnce(bool runOnce);

    bool halted() const;
    void setHalted(bool halted);

private:
    renderer mRenderer;
    texture mTexture;
	uint32_t mWidth;
	uint32_t mHeight;
	uint32_t mLastUpdated;
	uint32_t mFPS;
    uint32_t mCurrentAction;
    uint32_t mCurrentIndex;
    SDL_Rect mTgt;
    int32_t mScale;


	std::map<uint32_t, std::vector<offset>> mActionMap;
    bool mFlipped;
    bool mRunOnce;
    bool mHalted;
};

#endif /* INCLUDE_SPRITE_H_ */
