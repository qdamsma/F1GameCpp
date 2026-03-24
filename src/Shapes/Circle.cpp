/*
 * Circle.cpp
 *
 *  Created on: 1 okt. 2025
 *      Author: QuintenDamsma
 */

#include "Circle.h"
#include "Utils.h"

Circle::Circle(): Circle(Vec2D::Zero, 0)
{

}

Circle::Circle(const Vec2D& center, float radius):mRadius(radius)
{
	mPoints.push_back(center);
}

bool Circle::Intersects(const Circle& otherCircle) const
{
	return GetCenterPoint().Distance(otherCircle.GetCenterPoint()) < (mRadius + otherCircle.mRadius);
}

bool Circle::ContainsPoint(const Vec2D& point) const
{
	return IsLessThanOrEqual(GetCenterPoint().Distance(point), mRadius);
}
