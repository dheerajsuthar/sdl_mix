/*
 * Sprite.cpp
 *
 *  Created on: 01-Jun-2019
 *      Author: dheeraj
 */
#include "Sprite.h"
#include <iostream>

Sprite::Sprite(renderer renderer, texture texture, uint32_t width,
		uint32_t height, uint32_t offsetX, uint32_t offsetY, uint32_t fps,
		bool loop, uint32_t loopCount) :
		mRenderer(renderer), mTexture(std::move(texture)), mWidth(width), mHeight(
				height), mCurrentOffsetX(offsetX), mCurrentOffsetY(offsetY), mLoopCount(
				loopCount), mLoop(loop), mLastUpdated(0), mFPS(fps) {
}

void Sprite::renderFrame(uint32_t dstX, uint32_t dstY, uint32_t srcX,
		uint32_t srcY) {
	SDL_Rect dst;
	dst.x = dstX;
	dst.y = dstY;
	dst.w = SCALE_FACTOR * mWidth;
	dst.h = SCALE_FACTOR * mHeight;
	SDL_Rect src;
	src.x = srcX;
	src.y = srcY;
	src.w = mWidth;
	src.h = mHeight;
	SDL_RenderCopy(mRenderer.get(), mTexture.get(), &src, &dst);
}

void Sprite::setTexture(texture texture) {
	mTexture = std::move(texture);
}

void Sprite::renderFrame(uint32_t dstX, uint32_t dstY, uint32_t time) {

	uint32_t delta = time - mLastUpdated;
	std::cout << 1000.f / mFPS << "\n";
	std::cout << delta << "\n";
	std::cout << "looping: " << mLoop << " mCurrentOffsetX: " << mCurrentOffsetX
			<< " mCurrentOffsetY:" << mCurrentOffsetY << "\n";
	renderFrame(dstX, dstY, mCurrentOffsetX * mWidth,
			mCurrentOffsetY * mHeight);

	if (mLoop && delta > (1000.f / mFPS)) {
		mLastUpdated = time;
		mCurrentOffsetX = ++mCurrentOffsetX % mLoopCount;
	}
}
