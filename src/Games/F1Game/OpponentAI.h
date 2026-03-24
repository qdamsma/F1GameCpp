/*
 * OpponentAI.h
 *
 *  Created on: 23 okt. 2025
 *      Author: QuintenDamsma
 */

#ifndef GAMES_F1GAME_OPPONENTAI_H_
#define GAMES_F1GAME_OPPONENTAI_H_

#include "Opponent.h"
#include "F1GameUtils.h"
#include <random>
#include <stdint.h>

class F1GameLevel;
class Screen;
class Car;

enum OppAIState{
	OPP_AI_STATE_START = 0,
	OPP_AI_STATE_CHASE,
	OPP_AI_STATE_SCATTER
};
class OpponentAI{
public:
	OpponentAI();
	void Init(Opponent& opp, uint32_t lookAheadDistance, const Vec2D& scatterTarget, OppName name);
	CarMovement Update(uint32_t dt, const Car& player, const F1GameLevel& level, const std::vector<Opponent>& opponents);
	void Draw(Screen& screen);
    void SetWaypoints(const std::vector<Vec2D>& waypoints);
    void Reset();

private:
	Vec2D mTarget;
	uint32_t mTimer;
	Opponent* mnoptrOpp;
	OppName mName;
    std::vector<Vec2D> mWaypoints;
    size_t mCurrentWaypointIndex = 0;

};



#endif /* GAMES_F1GAME_OPPONENTAI_H_ */
