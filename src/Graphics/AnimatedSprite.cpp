/*
 * AnimatedSprite.cpp
 *
 *  Created on: 10 okt. 2025
 *      Author: QuintenDamsma
 */

#include "AnimatedSprite.h"
#include "AARectangle.h"
#include "Screen.h"
#include "SpriteSheet.h"

AnimatedSprite::AnimatedSprite():mPosition(Vec2D::Zero), mnoptrSpriteSheet(nullptr)
{

}

void AnimatedSprite::Init(const std::string& animationsPath, const SpriteSheet& spriteSheet, const Color& color)
{
	mAnimationPlayer.Init(animationsPath);
	mnoptrSpriteSheet = &spriteSheet;
	mColor = color;
}

void AnimatedSprite::Update(uint32_t dt)
{
	mAnimationPlayer.Update(dt);
}

void AnimatedSprite::Draw(Screen& theScreen)
{
    AnimationFrame frame = mAnimationPlayer.GetCurrentAnimationFrame();

    Color frameColor = frame.frameColor;
    if(!frame.frameColorSet)
        frameColor = mColor;
    Sprite spriteFrame = mnoptrSpriteSheet->GetSprite(frame.frame);
    theScreen.Draw(mnoptrSpriteSheet->GetBMPImage(), spriteFrame, mPosition + frame.offset, frameColor, mRotation);

    if(frame.overlay.size() > 0)
    {
    	Sprite overlaySprite = mnoptrSpriteSheet->GetSprite(frame.overlay);
    	theScreen.Draw(mnoptrSpriteSheet->GetBMPImage(), overlaySprite, mPosition, frame.overlayColor, mRotation);
    }
}

void AnimatedSprite::SetAnimation(const std::string& animationName, bool looped)
{
	mAnimationPlayer.Play(animationName, looped);
}

Vec2D AnimatedSprite::Size() const
{
	return mAnimationPlayer.GetCurrentAnimationFrame().size;
}

void AnimatedSprite::Stop()
{
	mAnimationPlayer.Stop();
}

const AARectangle AnimatedSprite::GetBoundingBox() const
{
	AARectangle bbox = {mPosition, static_cast<unsigned int>(Size().GetX()), static_cast<unsigned int>(Size().GetY())};
	return bbox;
}




