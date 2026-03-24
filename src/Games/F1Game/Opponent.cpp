/*
 * Opponent.cpp
 *
 *  Created on: 21 okt. 2025
 *      Author: QuintenDamsma
 */

#include "Opponent.h"


Opponent::Opponent():mInitialPos(Vec2D::Zero)
{
}

void Opponent::Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Vec2D& initialPos, uint32_t movementSpeed, bool updateSpriteOnMovement, const Color&spriteColor)
{
	F1Actor::Init(spriteSheet, animationsPath, initialPos, movementSpeed, updateSpriteOnMovement, spriteColor);
	mInitialPos = initialPos;
	mInitialRotation = 270.0f;
	mInitialMovement = CAR_MOVEMENT_UP;
	ResetToFirstPosition();
}

void Opponent::Update(uint32_t dt)
{
	Vec2D position = Position();

	F1Actor::Update(dt);

	mCanChangeDirection = position != Position();

}


void Opponent::SetMovementDirection(CarMovement direction)
{
	F1Actor::SetMovementDirection(direction);

	CarMovement movementDir = GetMovementDirection();

	if(mState == OPP_STATE_ALIVE)
	{
		if(movementDir == CAR_MOVEMENT_RIGHT)
		{
			SetAnimation("opp_movement_right", true);
		}
		else if(movementDir == CAR_MOVEMENT_UP)
		{
			SetAnimation("opp_movement_up", true);
		}
		else if(movementDir == CAR_MOVEMENT_DOWN)
		{
			SetAnimation("opp_movement_down", true);
		}
		else if(movementDir == CAR_MOVEMENT_LEFT)
		{
			SetAnimation("opp_movement_left", true);
		}
	}
	else if(mState == OPP_STATE_DEAD)
	{
		if(movementDir == CAR_MOVEMENT_RIGHT)
		{
			SetAnimation("opp_dead_right", true);
		}
		else if(movementDir == CAR_MOVEMENT_UP)
		{
			SetAnimation("opp_dead_up", true);
		}
		else if(movementDir == CAR_MOVEMENT_DOWN)
		{
			SetAnimation("opp_dead_down", true);
		}
		else if(movementDir == CAR_MOVEMENT_LEFT)
		{
			SetAnimation("opp_dead_left", true);
		}
	}
}

void Opponent::Stop()
{
	SetMovementDirection(CAR_MOVEMENT_NONE);
}



void Opponent::ResetToFirstPosition()
{
    mSprite.SetPosition(mInitialPos);
    mRotation = mInitialRotation;
    mSprite.SetRotation(mInitialRotation);

    mMomentum = Vec2D::Zero;
    mMovementDirection = CAR_MOVEMENT_NONE;

    if (mState == OPP_STATE_ALIVE){
        SetAnimation("opp_movement_up", true);
    } else {
        SetAnimation("opp_dead_up", true);
    }
    mCanChangeDirection = true;
}

void Opponent::DriveToTarget(const Vec2D& target, uint32_t dt)
{
    Vec2D pos = GetBoundingBox().GetCenterPoint();
    Vec2D toTarget = target - pos;

    float desiredAngle = atan2f(toTarget.GetY(), toTarget.GetX()) * 180.0f / M_PI;
    float currentAngle = GetRotation();

    //Voorkom dat hij de lange hoek draait
    float angleDiff = desiredAngle - currentAngle;
    while (angleDiff > 180.0f){ angleDiff -= 360.0f;}
    while (angleDiff < -180.0f){ angleDiff += 360.0f;}

    // Settings AI
    const float steerSensitivity = 0.55f;
    const float speedFactor = 0.8f;
    const float alignmentToDrive = 18.0f;

    // Stuur naar het doel
    if (angleDiff > 1.0f){
    	SetSteer(+steerSensitivity);
    }
    else if (angleDiff < -1.0f){
        SetSteer(-steerSensitivity);
    } else {
        SetSteer(0.0f);
    }
    // Alleen rijden als auto uitgelijnd is
    if (fabs(angleDiff) <= alignmentToDrive)
    {
        SetMovementDirection(CAR_MOVEMENT_UP);
        SetMovementSpeed(static_cast<uint32_t>(OPP_MOVEMENT_SPEED * speedFactor));
    }
    else
    {
        // Draai de auto naar de angle
        SetMovementDirection(CAR_MOVEMENT_NONE);
        mMomentum *= 0.6f;
    }
}

