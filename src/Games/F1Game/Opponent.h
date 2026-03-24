/*
 * Opponent.h
 *
 *  Created on: 21 okt. 2025
 *      Author: QuintenDamsma
 */

#ifndef GAMES_F1GAME_OPPONENT_H_
#define GAMES_F1GAME_OPPONENT_H_
#define _USE_MATH_DEFINES
#include <cmath>
#include "F1Actor.h"
#include "Color.h"
#include "F1GameUtils.h"
#include "AARectangle.h"

enum OppName{
	BRONZE = 0,
	NUM_OPPS
};

enum OppState
{
	OPP_STATE_ALIVE = 0,
	OPP_STATE_VULNERABLE,
	OPP_STATE_VULNERABLE_ENDING,
	OPP_STATE_DEAD
};

class Opponent: public F1Actor
{
public:
	static const uint32_t VULNERABILITY_TIME = 6000;
	static const uint32_t VULNERABILITY_ENDING_TIME = 4000;

	Opponent();
	virtual void Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Vec2D& initialPos, uint32_t movementSpeed, bool updateSpriteOnMovement, const Color&spriteColor = Color::White()) override;
	virtual void Update(uint32_t dt) override;

	void DriveToTarget(const Vec2D& target, uint32_t dt);
	virtual void SetMovementDirection(CarMovement direction) override;
	virtual void Stop() override;

	void ResetToFirstPosition();


private:
	OppState mState;
	bool mCanChangeDirection;
	Vec2D mInitialPos;
    float mInitialRotation;
    CarMovement mInitialMovement;
};

#endif /* GAMES_F1GAME_OPPONENT_H_ */
