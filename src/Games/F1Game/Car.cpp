/*
 * Car.cpp
 *
 *  Created on: 13 okt. 2025
 *      Author: QuintenDamsma
 */

#include "Car.h"
#include "Utils.h"
#include "Color.h"

void Car::Init(const SpriteSheet& spriteSheet, const std::string& animationPath, const Vec2D& initialPos, uint32_t movementSpeed, bool updateSpriteOnMovement, const Color&spriteColor)
{
	F1Actor::Init(spriteSheet, animationPath, initialPos, movementSpeed, false, spriteColor);

	ResetToFirstAnimation();
	ResetScore();
	mIsDying = false;

}
void Car::Update(uint32_t dt)
{
	if(mIsDying)
	{
		mSprite.Update(dt);
		mIsDying = !IsFinishedAnimation();
		return;
	}

	F1Actor::Update(dt);
}

void Car::SetMovementDirection(CarMovement movementDir)
{
    CarMovement currentDirection = GetMovementDirection();

    // Alleen voor vooruit/achteruit animaties
    if (movementDir == CAR_MOVEMENT_DOWN && currentDirection != CAR_MOVEMENT_DOWN)
    {
        ResetDelta();
    }
    else if (movementDir == CAR_MOVEMENT_UP && currentDirection != CAR_MOVEMENT_UP)
    {
        SetAnimation("move_right", true);
        ResetDelta();
    }

    F1Actor::SetMovementDirection(movementDir);
}

void Car::ResetToFirstAnimation()
{
	SetAnimation("move_right", true);
	Stop();
}


void Car::ResetScore()
{
	mScore = 0;
}

void Car::GotByOpp()
{
	SetAnimation("death", false);
	mIsDying = true;

}


