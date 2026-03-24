/*
 * Actor.h
 *
 *  Created on: 15 okt. 2025
 *      Author: QuintenDamsma
 */

#ifndef GAMES_F1GAME_F1ACTOR_H_
#define GAMES_F1GAME_F1ACTOR_H_

#include "AnimatedSprite.h"
#include "Vec2D.h"
#include "F1GameUtils.h"
#include "AARectangle.h"
#include "Color.h"
#include <string>
#include <stdint.h>
#include "Utils.h"
class Screen;
class SpriteSheet;

class F1Actor
{
public:

	virtual ~F1Actor() {}
	virtual void Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Vec2D& initialPos, uint32_t movementSpeed, bool updateSpriteOnMovement, const Color&spriteColor = Color::White());
	virtual void Update(uint32_t dt);
	virtual void Draw(Screen& screen);

	virtual void Stop();
	AARectangle GetEatingBoundingBox() const;

	inline bool IsFinishedAnimation() const {return mSprite.IsFinishedPlayingAnimation();}
	inline const AARectangle GetBoundingBox() const {return mSprite.GetBoundingBox();}
	inline void MoveBy(const Vec2D& delta) {mSprite.MoveBy(delta);}
	inline void MoveTo(const Vec2D& position) {mSprite.SetPosition(position);}
	inline Vec2D Position() const {return mSprite.Position();}
	inline CarMovement GetMovementDirection() const {return mMovementDirection;}
	virtual inline void SetMovementDirection(CarMovement direction) {mMovementDirection = direction;}
	inline const Color& GetSpriteColor() const {return mSprite.GetColor();}

    float GetRotation() const { return mRotation; }
    void SetRotation(float r) { mRotation = r; }
	inline void SetSteer(float s) { mSteer = Clamp(s, -1.0f, 1.0f); }
	inline float GetSteer() const { return mSteer; }
	void SetPosition(const Vec2D& pos) {
	    mSprite.SetPosition(pos);
	}
	CarMovement mMovementDirection;

protected:

	void SetAnimation(const std::string& animationName, bool looped);
	inline void ResetDelta() {mDelta = Vec2D::Zero;}
	inline void SetMovementSpeed(uint32_t movementSpeed) {mMovementSpeed = movementSpeed;}

	AnimatedSprite mSprite;
	Vec2D mMomentum = Vec2D::Zero;
    float mRotation = 0.0f;
private:
	Vec2D mDelta;

	uint32_t mMovementSpeed;
	bool mUpdateSpriteOnUpdate;
	float mSpeedFactor = 1.0f;
	float mBrakeFactor = 0.8f;

    CarMovement mLastDirection = CAR_MOVEMENT_NONE;

    float mSpriteRotation = 0.0f;
    float mSteer = 0.0f;
    float mSteerSpeed = 180.0f;
    float mRotationSmooth = 0.2f;
};

#endif /* GAMES_F1GAME_F1ACTOR_H_ */
