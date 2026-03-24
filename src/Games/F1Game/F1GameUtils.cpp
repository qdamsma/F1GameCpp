/*
 * F1GameUtils.cpp
 *
 *  Created on: 15 okt. 2025
 *      Author: QuintenDamsma
 */


#include "F1GameUtils.h"

Vec2D GetMovementVector(CarMovement direction)
{
	if(direction == CAR_MOVEMENT_LEFT)
	{
		return Vec2D(-1, 0);
	}
	else if(direction == CAR_MOVEMENT_DOWN)
	{
		return Vec2D(0, 1);
	}
	else if(direction == CAR_MOVEMENT_RIGHT)
	{
		return Vec2D(1, 0);
	}
	else if(direction == CAR_MOVEMENT_UP)
	{
		return Vec2D(0, -1);
	}

	return Vec2D::Zero;
}

CarMovement GetOppositeDirection(CarMovement direction)
{
	if(direction == CAR_MOVEMENT_LEFT)
	{
		return CAR_MOVEMENT_RIGHT;
	}
	else if(direction == CAR_MOVEMENT_DOWN)
	{
		return CAR_MOVEMENT_UP;
	}
	else if(direction == CAR_MOVEMENT_RIGHT)
	{
		return CAR_MOVEMENT_LEFT;
	}
	else if(direction == CAR_MOVEMENT_UP)
	{
		return CAR_MOVEMENT_DOWN;
	}

	return CAR_MOVEMENT_NONE;
}

std::vector<CarMovement> GetPerpendicularMovements(CarMovement direction)
{
	std::vector<CarMovement> perpendicularDirections;

	if(direction == CAR_MOVEMENT_DOWN || direction == CAR_MOVEMENT_UP)
	{
		perpendicularDirections.push_back(CAR_MOVEMENT_LEFT);
		perpendicularDirections.push_back(CAR_MOVEMENT_RIGHT);
	}
	else if(direction == CAR_MOVEMENT_RIGHT || direction == CAR_MOVEMENT_LEFT)
	{
		perpendicularDirections.push_back(CAR_MOVEMENT_UP);
		perpendicularDirections.push_back(CAR_MOVEMENT_DOWN);
	}
	else if(direction == CAR_MOVEMENT_NONE)
	{
		perpendicularDirections.push_back(CAR_MOVEMENT_UP);
		perpendicularDirections.push_back(CAR_MOVEMENT_LEFT);
		perpendicularDirections.push_back(CAR_MOVEMENT_DOWN);
		perpendicularDirections.push_back(CAR_MOVEMENT_RIGHT);
	}

	return perpendicularDirections;
}

std::vector<CarMovement> GetOtherDirections(CarMovement direction)
{
	std::vector<CarMovement> directions;

	for(int dir = CarMovement::CAR_MOVEMENT_NONE + 1; dir <= CAR_MOVEMENT_RIGHT; ++dir)
	{
		if(dir != direction)
		{
			directions.push_back(static_cast<CarMovement>(dir));
		}
	}

	return directions;

}

