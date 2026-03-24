/*
 * Actor.cpp
 *
 *  Created on: 15 okt. 2025
 *      Author: QuintenDamsma
 */

#include <F1Game/F1Actor.h>
#include "Utils.h"
#include <cmath>


void F1Actor::Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Vec2D& initialPos, uint32_t movementSpeed, bool updateSpriteOnMovement, const Color&spriteColor)
{
	mMovementDirection = CAR_MOVEMENT_NONE;
	mSprite.Init(animationsPath, spriteSheet, spriteColor);
	mUpdateSpriteOnUpdate = updateSpriteOnMovement;
	mMovementSpeed = movementSpeed;
	mDelta = Vec2D::Zero;
	mSprite.SetPosition(initialPos);
}

void F1Actor::Update(uint32_t dt)
{
    float seconds = MillisecondsToSeconds(dt);

    // Rotatie
    if (fabsf(mSteer) > 0.001f)
    {
        mRotation += mSteer * mSteerSpeed * seconds;
        if (mRotation >= 360.0f) mRotation -= 360.0f;
        else if (mRotation < 0.0f) mRotation += 360.0f;
    }

    // Beweging
    float rad = mRotation * 3.14159265f / 180.0f;
    Vec2D forward(std::cos(rad), std::sin(rad));

    if (mMovementDirection == CAR_MOVEMENT_UP)
    {
        Vec2D accel = forward * static_cast<float>(mMovementSpeed) * seconds;
        mMomentum += accel;

        float maxSpeed = static_cast<float>(mMovementSpeed) * 2.5f;
        if (mMomentum.Length() > maxSpeed)
            mMomentum = mMomentum.GetUnitVec() * maxSpeed;
    }
    else if (mMovementDirection == CAR_MOVEMENT_DOWN)
    {
        mMomentum *= 0.85f;
    }
    else
    {
        mMomentum *= 0.98f;
    }

    Vec2D delta = mMomentum * seconds;
    mSprite.MoveBy(delta);

    // Sprite rotatie
    // Hard gekoppeld aan auto-rotatie
    float currentSpriteRot = mSprite.GetRotation();
    float targetSpriteRot = mRotation;

    float diff = targetSpriteRot - currentSpriteRot;
    if (diff > 180.0f) diff -= 360.0f;
    else if (diff < -180.0f) diff += 360.0f;

    float followStrength = 1.0f;
    float newRot = currentSpriteRot + diff * followStrength;

    mSprite.SetRotation(newRot);

    mSprite.Update(dt);
}

void F1Actor::Draw(Screen& screen)
{
	mSprite.Draw(screen);
}

void F1Actor::Stop()
{
	SetMovementDirection(CAR_MOVEMENT_NONE);
	mSprite.Stop();
}

AARectangle F1Actor::GetEatingBoundingBox() const
{
	return AARectangle::Inset(GetBoundingBox(), Vec2D(3,3));
}

void F1Actor::SetAnimation(const std::string& animationName, bool looped)
{
	mSprite.SetAnimation(animationName, looped);
}
