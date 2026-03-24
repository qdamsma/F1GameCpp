/*
 * OpponentAI.cpp
 *
 *  Created on: 23 okt. 2025
 *      Author: QuintenDamsma
 */


#include "OpponentAI.h"
#include "Screen.h"
#include "F1GameLevel.h"
#include "Car.h"
#include <cassert>
#include <algorithm>
#include "Color.h"
#include "Circle.h"

OpponentAI::OpponentAI():mnoptrOpp(nullptr)
{

}

void OpponentAI::Init(Opponent& opp, uint32_t lookAheadDistance, const Vec2D& scatterTarget, OppName name)
{
    mnoptrOpp = &opp;
    mName = name;
    mTimer = 0;


    // Waypoints instellen
    mWaypoints = {
        {100, 220},
        {300, 220},
        {500, 240},
        {700, 260},
        {900, 280},
        {100, 200}
    };

    mCurrentWaypointIndex = 0;
    mTarget = mWaypoints[mCurrentWaypointIndex];

    // Direct naar eerste target sturen
//    mnoptrOpp->DriveToTarget(mTarget, 0);

}

void OpponentAI::SetWaypoints(const std::vector<Vec2D>& waypoints)
{
    mWaypoints = waypoints;
    mCurrentWaypointIndex = 0;
}

CarMovement OpponentAI::Update(uint32_t dt, const Car& player, const F1GameLevel& level, const std::vector<Opponent>& opponents)
{
    if (!mnoptrOpp || mWaypoints.empty())
        return CAR_MOVEMENT_NONE;

    Vec2D pos = mnoptrOpp->GetBoundingBox().GetCenterPoint();
    Vec2D target = mWaypoints[mCurrentWaypointIndex];
    float dist = (target - pos).Length();

    const float waypointThreshold = 15.0f;

    // Wissel van waypoint zodra de AI dichtbij is
    if (dist < waypointThreshold && mTimer > 200)
    {
        mCurrentWaypointIndex = (mCurrentWaypointIndex + 1) % mWaypoints.size();
        target = mWaypoints[mCurrentWaypointIndex];
    }

    mTarget = target;
    mnoptrOpp->DriveToTarget(target, dt);

    // Korte wachttijd tussen updates vermijden
    mTimer += dt;
    if (mTimer > 5000){ mTimer = 0;}

    return mnoptrOpp->GetMovementDirection();
}
void OpponentAI::Draw(Screen& screen)
{

//	//De waypoint van de AI
//	for (const auto& wp : mWaypoints)
//	{
//	    Circle c(wp, 5);
//	    screen.Draw(c, Color::White(), true, Color::White());
//	}
//
//    if (mnoptrOpp)
//    {
//         Circle targetCircle = Circle(mTarget, 4);
//         screen.Draw(targetCircle, mnoptrOpp->GetSpriteColor(), true, mnoptrOpp->GetSpriteColor());
//    }
}

void OpponentAI::Reset() {
	// Zet AI en opponent terug naar startpositie
    if (mnoptrOpp) {
        mnoptrOpp->ResetToFirstPosition();
        mnoptrOpp->SetMovementDirection(CAR_MOVEMENT_UP);
    }
    mCurrentWaypointIndex = 0;
    if (!mWaypoints.empty()) {
        mTarget = mWaypoints[mCurrentWaypointIndex];
    }
}


