/*
 * Car.h
 *
 *  Created on: 13 okt. 2025
 *      Author: QuintenDamsma
 */

#ifndef GAMES_F1GAME_CAR_H_
#define GAMES_F1GAME_CAR_H_

#include "F1Actor.h"
#include <stdint.h>

class Screen;
class SpriteSheet;

class Car: public F1Actor
{
public:
	virtual void Init(const SpriteSheet& spriteSheet, const std::string& animationPath, const Vec2D& initialPos, uint32_t movementSpeed, bool updateSpriteOnMovement, const Color&spriteColor = Color::White()) override;
	virtual void Update(uint32_t dt) override;
	virtual void SetMovementDirection(CarMovement movementDir) override;

	void ResetToFirstAnimation();
	void ResetScore();
	void GotByOpp();
	bool IsDying() const {
	    return mIsDying;
	}

	inline uint32_t Score() const {return mScore;}

private:
	void AddToScore(uint32_t value);

	uint32_t mScore;
	uint32_t mGhostMultiplier;
	bool mIsDying;//dying animation
};



#endif /* GAMES_F1GAME_CAR_H_ */
