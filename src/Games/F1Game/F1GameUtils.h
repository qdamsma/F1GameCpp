/*
 * F1GameUtils.h
 *
 *  Created on: 15 okt. 2025
 *      Author: QuintenDamsma
 */

#ifndef GAMES_F1GAME_F1GAMEUTILS_H_
#define GAMES_F1GAME_F1GAMEUTILS_H_


#include "Vec2D.h"
#include <vector>

static const int CAR_MOVEMENT_SPEED = 50; //per second
static const int OPP_MOVEMENT_SPEED = 45;
static const int OPP_VULNERABLE_MOVEMENT_SPEED = 25;
static const int OPP_BACK_TO_PEN_SPEED = 100;


enum CarMovement
{
	CAR_MOVEMENT_NONE = 0,
	CAR_MOVEMENT_UP,
	CAR_MOVEMENT_LEFT,
	CAR_MOVEMENT_DOWN,
	CAR_MOVEMENT_RIGHT
};

Vec2D GetMovementVector(CarMovement direction);
CarMovement GetOppositeDirection(CarMovement direction);
std::vector<CarMovement> GetPerpendicularMovements(CarMovement direction);
std::vector<CarMovement> GetOtherDirections(CarMovement direction);



#endif /* GAMES_F1GAME_F1GAMEUTILS_H_ */
