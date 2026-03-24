/*
 * F1Game.cpp
 *
 *  Created on: 13 okt. 2025
 *      Author: QuintenDamsma
 */
#include "F1Game.h"
#include "App.h"
#include "F1GameUtils.h"
#include "F1GameColorScene.h"
#include "OpponentAI.h"

namespace
{
const std::string SCORE_STR = "Score ";
const std::string CAR_LIFE_SPRITE_NAME = "car_left_idle";
const size_t MAX_NUM_LIVES = 2;
const std::string READY_STR = "Ready!";
const std::string GAME_OVER_STR = "Game Over";
const uint32_t LEVEL_STARTING_TIME = 3000;
}
void F1Game::Init(GameController& controller){

	mCarSpriteSheet.Load("F1GameSprites");

	mCar.Init(mCarSpriteSheet, App::Singleton().GetBasePath() + "Assets/Car_animations.txt", Vec2D(60, 44), CAR_MOVEMENT_SPEED, false, F1GameColorScene::GetSelectedColor());

	mCarInitialPosition = mCar.GetBoundingBox().GetTopLeftPoint();

	mLevel.Init(App::Singleton().GetBasePath() + "Assets/F1Game_level.txt");

	mStringRect = AARectangle(Vec2D(0, 180), App::Singleton().Width(), mCar.GetBoundingBox().GetHeight());

	SetupOpps();
	ResetGame();

	ButtonAction leftAction;
	leftAction.key = GameController::LeftKey();
	leftAction.action = [this](uint32_t dt, InputState state){
		HandleGameControllerState(dt, state, CAR_MOVEMENT_LEFT);
	};
	controller.AddInputActionForKey(leftAction);

	ButtonAction rightAction;
	rightAction.key = GameController::RightKey();
	rightAction.action = [this](uint32_t dt, InputState state)
		{
			HandleGameControllerState(dt, state, CAR_MOVEMENT_RIGHT);
		};
	controller.AddInputActionForKey(rightAction);

	ButtonAction upAction;
	upAction.key = GameController::UpKey();
	upAction.action = [this](uint32_t dt, InputState state)
		{
			HandleGameControllerState(dt, state, CAR_MOVEMENT_UP);
		};
	controller.AddInputActionForKey(upAction);

	ButtonAction downAction;
	downAction.key = GameController::DownKey();
	downAction.action = [this](uint32_t dt, InputState state)
		{
			HandleGameControllerState(dt, state, CAR_MOVEMENT_DOWN);
		};
	controller.AddInputActionForKey(downAction);

	ButtonAction backAction;
	backAction.key = GameController::CancelKey();
	backAction.action = [this](uint32_t dt, InputState state)
		{
			if(mGameState == GAME_OVER && GameController::IsPressed(state))
			{
				App::Singleton().PopScene();
			}
		};

	controller.AddInputActionForKey(backAction);


}

void F1Game::Update(uint32_t dt){

	if(mGameState == LEVEL_STARTING){
		mLevelStartingTimer += dt;

		if(mLevelStartingTimer >= LEVEL_STARTING_TIME){
			mLevelStartingTimer = 0;
			mGameState = PLAY_GAME;
		}
	} else if (mGameState == PLAY_GAME){
		UpdateCarMovement();
			mCar.Update(dt);

			for(size_t i = 0; i < NUM_OPPS; ++i){

				if(static_cast<OppName>(i) == BRONZE){
					OpponentAI& oppAI = mOppsAI[i];

					auto direction = oppAI.Update(dt, mCar, mLevel, mOpps);
				}
				mOpps[i].Update(dt);
			}

			mLevel.Update(dt, mCar, mOpps);

		    bool crossedFinish = mLevel.Update(dt, mCar, mOpps);
		    float seconds = dt / 1000.0f;

		    if (mFinishCooldown > 0.0f){
		            mFinishCooldown -= seconds;
		    }

		    //Over de finish logica
		    if (crossedFinish && !mWasOnFinish)
		    {
		        if (!mTimerRunning)
		        {
		            mTimerRunning = true;
		            mElapsedTime = 0.0f;
		        }
		        else
		        {
		        	mLapTimes.push_back(mElapsedTime);
		        	mCurrentLap++;


		            if (mCurrentLap > mTotalLaps)
		            {
		                mCurrentLap = 1;
		                mCurrentTrack++;

		                if (mCurrentTrack > mTotalTracks)
		                {
		                    mGameState = GAME_OVER;
		                    std::cout << "Alle tracks voltooid!" << std::endl;
		                }
		                else
		                {
		                    ResetLevelForNewTrack();
		                }
		            }

		            mFinishCooldown = 1.5f;
		        }
		    }

		    mWasOnFinish = crossedFinish;

		    if (mTimerRunning)
		    {
		        mElapsedTime += dt / 1000.0f;
		    }

		    for (size_t i = 0; i < NUM_OPPS; ++i) {
		    	if (mCar.GetEatingBoundingBox().Intersects(mOpps[i].GetBoundingBox())) {
		    	    mNumLives--;
		    	    mGameState = LOST_LIFE;
		    	    mCar.GotByOpp();

		    	    break;
		    	}
		    }

	} else if (mGameState == LOST_LIFE){
	    mCar.Update(dt);

	    if(!mCar.IsDying()){
	        ResetLevelFull();
	    }


		if(mCar.IsFinishedAnimation()){
			if (mNumLives >= 0){
//				ResetLevel();
			} else {
				mGameState = GAME_OVER;
			}
		}
	}
}

void F1Game::ResetLevelFull() {
    // Reset spelerauto
    mCar.SetMovementDirection(CAR_MOVEMENT_NONE);
    mCar.ResetToFirstAnimation();
    mCar.ResetToFirstAnimation();
    mCar.SetPosition(mCarInitialPosition);
    mCar.SetRotation(0);

    // Reset AI
    for (auto& opponent : mOpps) {
        opponent.ResetToFirstPosition();
    }
    for (auto& oppAI : mOppsAI) {
        oppAI.Reset();
    }

    // Reset timer
    mElapsedTime = 0.0f;
    mTimerRunning = false;
    mWasOnFinish = false;
    mFinishCooldown = 0.0f;

    mGameState = LEVEL_STARTING;
    mLevelStartingTimer = 0;
}

void F1Game::Draw(Screen& screen){
	mLevel.Draw(screen);
	mCar.Draw(screen);

	for(auto& opponent: mOpps){
		opponent.Draw(screen);
	}

	for(auto& oppAI: mOppsAI){
		oppAI.Draw(screen);
	}

    Vec2D levelOffset = mLevel.GetLayoutOffset();
    AARectangle infoRect = AARectangle(Vec2D(0, 4), App::Singleton().Width(), levelOffset.GetY());
    const auto& font = App::Singleton().GetFont();


    std::string timeStr = "Tijd: " + std::to_string(mElapsedTime).substr(0, 5) + "s";
    Vec2D timePos = font.GetDrawPosition(timeStr, infoRect, BFXA_CENTER, BFYA_CENTER);
    screen.Draw(font, timeStr, timePos);


    std::string trackStr = "T " + std::to_string(mCurrentTrack) + "." + std::to_string(mTotalTracks);
    Vec2D trackPos = font.GetDrawPosition(trackStr, infoRect, BFXA_LEFT, BFYA_CENTER);
    screen.Draw(font, trackStr, trackPos);


    std::string lapStr = "L " + std::to_string(mCurrentLap) + "." + std::to_string(mTotalLaps);
    Vec2D lapPos = font.GetDrawPosition(lapStr, infoRect, BFXA_RIGHT, BFYA_CENTER);
    screen.Draw(font, lapStr, lapPos);

    Vec2D lapTimesPos(50, App::Singleton().Height() - 13);
    std::string lapTimesStr = "Time: ";
    for(size_t i = 0; i < mLapTimes.size(); ++i){
        int seconds = static_cast<int>(mLapTimes[i]);
        lapTimesStr += std::to_string(seconds) + "s ";
    }
    screen.Draw(font, lapTimesStr, lapTimesPos);

    if(mGameState == LEVEL_STARTING){
    	std::string readyStr = "Ready!";
        Vec2D ready = font.GetDrawPosition(READY_STR, mStringRect, BFXA_CENTER, BFYA_CENTER);
        screen.Draw(font, readyStr, ready);
    } else if (mGameState == GAME_OVER){
    	std::string gameoverStr = "Game over";
        Vec2D gameover = font.GetDrawPosition(GAME_OVER_STR, mStringRect, BFXA_CENTER, BFYA_CENTER);
        screen.Draw(font, gameoverStr, gameover);
    }
    DrawLives(screen);
}
void F1Game::DrawLives(Screen& screen) {
	const uint32_t X_PAD = 1;
	auto sprite = mCarSpriteSheet.GetSprite(CAR_LIFE_SPRITE_NAME);

	uint32_t xPos = X_PAD;
	for(int i = 0; i < mNumLives; ++i){
		screen.Draw(mCarSpriteSheet, CAR_LIFE_SPRITE_NAME, Vec2D(xPos, App::Singleton().Height() - sprite.height));
		xPos += X_PAD + sprite.width;
	}
}

const std::string& F1Game::GetName()const {
	static std::string name = "F1Game";
	return name;
}

void F1Game::ResetGame()
{
	mNumLives = MAX_NUM_LIVES;
	mPressedDirection = CAR_MOVEMENT_NONE;
	mCar.ResetScore();
}

void F1Game::ResetLevel(){
	mGameState = LEVEL_STARTING;
}


void F1Game::UpdateCarMovement()
{
	if(mPressedDirection != CAR_MOVEMENT_NONE)
	{
		if(!mLevel.WillCollide(mCar.GetBoundingBox(), mPressedDirection)){
			mCar.SetMovementDirection(mPressedDirection);
		}
	}
}

// Controls voor de game
void F1Game::HandleGameControllerState(uint32_t dt, InputState state, CarMovement direction)
{
    if (direction == CAR_MOVEMENT_LEFT)
    {
        if (GameController::IsPressed(state))
            mCar.SetSteer(-1.0f);
        else if (GameController::IsReleased(state))
            mCar.SetSteer(0.0f);
    }

    else if (direction == CAR_MOVEMENT_RIGHT)
    {
        if (GameController::IsPressed(state))
            mCar.SetSteer(+1.0f);
        else if (GameController::IsReleased(state))
            mCar.SetSteer(0.0f);
    }

    else if (direction == CAR_MOVEMENT_UP)
    {
        if (GameController::IsPressed(state))
            mCar.SetMovementDirection(CAR_MOVEMENT_UP);
        else if (GameController::IsReleased(state))
            mCar.SetMovementDirection(CAR_MOVEMENT_NONE);
    }

    else if (direction == CAR_MOVEMENT_DOWN)
    {
        if (GameController::IsPressed(state))
            mCar.SetMovementDirection(CAR_MOVEMENT_DOWN);
        else if (GameController::IsReleased(state))
            mCar.SetMovementDirection(CAR_MOVEMENT_NONE);
    }
}

//Instellen van de opponents
void F1Game::SetupOpps(){
    const Vec2D BRONZE_SCATTER_POS = Vec2D(App::Singleton().Width() - 24, 0);

    mOpps.resize(NUM_OPPS);
    mOppsAI.resize(1);

    Opponent bronze;
    bronze.Init(mCarSpriteSheet, App::Singleton().GetBasePath() + "Assets/Opp_animations.txt",
                mLevel.GetOppsSpawnPoints()[BRONZE], OPP_MOVEMENT_SPEED, true, Color::Red());
    bronze.SetMovementDirection(CAR_MOVEMENT_LEFT);
    mOpps[BRONZE] = bronze;
    mOpps[BRONZE].ResetToFirstPosition();

    OpponentAI bronzeAI;
    bronzeAI.Init(mOpps[BRONZE], bronze.GetBoundingBox().GetWidth(), BRONZE_SCATTER_POS, BRONZE);

    std::vector<Vec2D> waypoints = {
        {40, 40},
		{180, 50},
		{180, 100},
		{100, 100},
		{100, 150},
		{190, 140},
		{180, 250},
		{30, 250},

    };

    bronzeAI.SetWaypoints(waypoints);
    mOppsAI[BRONZE] = bronzeAI;


}

void F1Game::ResetLevelForNewTrack()
{
    mCar.SetMovementDirection(CAR_MOVEMENT_NONE);
    mCar.ResetToFirstAnimation();
    mCar.SetPosition(mCarInitialPosition);
    mCar.SetRotation(0);

    for (auto& opponent : mOpps) {
        opponent.ResetToFirstPosition();
    }
    for (auto& oppAI : mOppsAI) {
        oppAI.Reset();
    }

    mElapsedTime = 0.0f;
    mTimerRunning = false;
    mWasOnFinish = false;
    mFinishCooldown = 0.0f;

    mGameState = LEVEL_STARTING;
    mLevelStartingTimer = 0;

    mLevel.ResetLevel();
}

const std::vector<float>& F1Game::GetRecentLapTimes() const {
    return mLapTimes;
}
