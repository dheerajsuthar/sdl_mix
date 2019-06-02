/*
 * Sprite.cpp
 *
 *  Created on: 01-Jun-2019
 *      Author: dheeraj
 */
#include "Sprite.h"
#include <iostream>
#include <vector>
#include <utility>

Sprite::Sprite(renderer renderer, texture texture, uint32_t width,
        uint32_t height,  uint32_t fps,
        bool loop, int32_t scale) :
		mRenderer(renderer), mTexture(std::move(texture)), mWidth(width), mHeight(
                height), mLoop(loop), mLastUpdated(0), mFPS(fps),
        mTgt({0,0,static_cast<int32_t>(mWidth),static_cast<int32_t>(mHeight)}),
        mScale(scale),
        mFlipped(false)
{
	//50,37

}

void Sprite::renderFrame( uint32_t srcX,
		uint32_t srcY) {

    mTgt.w = SCALE_FACTOR * mWidth;
    mTgt.h = SCALE_FACTOR * mHeight;
	SDL_Rect src;
	src.x = srcX;
	src.y = srcY;
	src.w = mWidth;
	src.h = mHeight;
    SDL_RendererFlip flip = mFlipped?SDL_FLIP_HORIZONTAL:SDL_FLIP_NONE;
    SDL_RenderCopyEx(mRenderer.get(), mTexture.get(), &src, &mTgt,0,NULL,flip);
}

void Sprite::setTexture(texture texture) {
	mTexture = std::move(texture);
}

void Sprite::setAction(uint32_t action, std::vector<Sprite::offset> offsets){
    mActionMap.insert(std::make_pair(action,offsets));
}

void Sprite::runAction(uint32_t action){
    mCurrentAction = action;
    mCurrentIndex = 0;
}

void Sprite::setScale(const int32_t &scale)
{
    mScale = scale;
}

void Sprite::setTgt(const SDL_Rect &tgt)
{
    mTgt = tgt;
}

SDL_Rect Sprite::tgt() const
{
    return mTgt;
}

uint32_t Sprite::currentAction() const
{
    return mCurrentAction;
}

bool Sprite::flipped() const
{
    return mFlipped;
}

void Sprite::setFlipped(bool flipped)
{
    mFlipped = flipped;
}


void Sprite::renderFrame(uint32_t time) {

    uint32_t delta = time - mLastUpdated;
    std::vector<offset> &actionReel = mActionMap[mCurrentAction];
    const offset &currentOffset = actionReel[mCurrentIndex];
    const uint64_t loopCount = actionReel.size();

//    std::cout << "Rendering: " << currentOffset.x << " " << currentOffset.y << "\n";

    renderFrame(currentOffset.x,
            currentOffset.y);

	if (mLoop && delta > (1000.f / mFPS)) {
		mLastUpdated = time;
        mCurrentIndex = ++mCurrentIndex % loopCount;
	}
}
