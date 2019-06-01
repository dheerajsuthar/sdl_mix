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

class Sprite {
public:
	const int SCALE_FACTOR = 2;

	Sprite(renderer renderer, texture texture, uint32_t width, uint32_t height,uint32_t offsetX, uint32_t offsetY, uint32_t fps, bool loop = false,uint32_t loopCount = 0 );
	void renderFrame(uint32_t dstX, uint32_t dstY, uint32_t offsetX,
			uint32_t offsetY);
	void renderFrame(uint32_t dstX, uint32_t dstY, uint32_t currentTime);
	void setTexture(texture texture);
private:
	renderer mRenderer;
	texture mTexture;
	uint32_t mWidth;
	uint32_t mHeight;
	uint32_t mCurrentOffsetX;
	uint32_t mCurrentOffsetY;
	uint32_t mLoopCount;
	uint32_t mLastUpdated;
	uint32_t mFPS;
	bool mLoop;
};

#endif /* INCLUDE_SPRITE_H_ */
