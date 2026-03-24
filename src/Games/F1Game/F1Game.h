/*
 * F1Game.h
 *
 *  Created on: 13 okt. 2025
 *      Author: QuintenDamsma
 */

#ifndef GAMES_F1GAME_F1GAME_H_
#define GAMES_F1GAME_F1GAME_H_

#include "Game.h"
#include "F1GameLevel.h"
#include "Car.h"
#include "SpriteSheet.h"
#include "InputAction.h"
#include "F1Actor.h"
#include "Opponent.h"
#include "OpponentAI.h"

enum F1GameState
{
	LEVEL_STARTING = 0,
	PLAY_GAME,
	LOST_LIFE,
	GAME_OVER
};


class F1Game : public Game{

public:
	virtual void Init(GameController& controller) override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Screen& screen) override;
	virtual const std::string& GetName() const override;
	const std::vector<float>& GetRecentLapTimes() const;
	void ResetCarToSpawn();
	void ResetLevelFull();
	Vec2D mCarInitialPosition;
private:

	void ResetGame();
	void UpdateCarMovement();
	void ResetLevelForNewTrack();
	void HandleGameControllerState(uint32_t dt, InputState state, CarMovement direction);
	void MoveCarOneStep(CarMovement direction);
	void ResetLevel();
	void DrawLives(Screen& screen);
	void SetupOpps();

	CarMovement mPressedDirection;
	SpriteSheet mCarSpriteSheet;
	Car mCar;
	F1GameLevel mLevel;
	int mNumLives;
	F1GameState mGameState;
	uint32_t mLevelStartingTimer;
	AARectangle mStringRect;

	std::vector<Opponent> mOpps;
	std::vector<OpponentAI> mOppsAI;
	std::vector<float> mLapTimes;

	bool mTimerRunning = false;
	float mElapsedTime = 0.0f;
	bool mWasOnFinish = false;

	float mFinishCooldown = 0.0f;

    int mCurrentTrack = 1;
    int mTotalTracks = 2;
    int mCurrentLap = 1;
    int mTotalLaps = 2;
};





#endif /* GAMES_F1GAME_F1GAME_H_ */
