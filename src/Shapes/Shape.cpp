/*
 * Shape.cpp
 *
 *  Created on: 1 okt. 2025
 *      Author: QuintenDamsma
 */

#include "Shape.h"

void Shape::MoveBy(const Vec2D& deltaOffset)
{
	for(Vec2D& point : mPoints)
	{
		point = point + deltaOffset;
	}
}
