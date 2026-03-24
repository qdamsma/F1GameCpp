/*
 * F1GameHighScoreScene.cpp
 *
 *  Created on: 29 okt. 2025
 *      Author: QuintenDamsma
 */




#include "F1GameHighScoreScene.h"
#include "App.h"
#include "GameController.h"

F1GameHighScoresScene::F1GameHighScoresScene(const std::vector<float>& lapTimes)
    : mLapTimes(lapTimes), mTextColor(Color::White())
{
}

void F1GameHighScoresScene::Init()
{
    ButtonAction backAction;
    backAction.key = GameController::CancelKey();
    backAction.action = [](uint32_t dt, InputState state)
    {
        if(GameController::IsPressed(state))
        {
            App::Singleton().PopScene();
        }
    };
    mGameController.AddInputActionForKey(backAction);
}

void F1GameHighScoresScene::Update(uint32_t dt)
{
}

void F1GameHighScoresScene::Draw(Screen& screen)
{
    const auto& font = App::Singleton().GetFont();
    int y = 50;

    screen.Draw(font, "F1 Game High Scores", Vec2D(30, y));
    y += 40;

    for(size_t i = 0; i < mLapTimes.size(); ++i)
    {
        std::string lapStr = "Lap " + std::to_string(i+1) + ": " + std::to_string(mLapTimes[i]).substr(0,5) + "s";
        screen.Draw(font, lapStr, Vec2D(50, y), mTextColor);
        y += 30;
    }
}

const std::string& F1GameHighScoresScene::GetSceneName() const
{
    static std::string name = "F1Game High Scores";
    return name;
}
