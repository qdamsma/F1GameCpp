/*
 * F1GameStartScene.cpp
 *
 *  Created on: 13 okt. 2025
 *      Author: QuintenDamsma
 */


#include "F1GameStartScene.h"
#include "App.h"
#include "NotImplementedScene.h"
#include "GameScene.h"
#include "../Games/F1Game/F1Game.h"
#include <vector>
#include <memory>
#include "../Games/F1Game/F1GameColorScene.h"
#include "../Games/F1Game/F1GameHighScoreScene.h"

F1GameStartScene::F1GameStartScene():ButtonOptionsScene({"Play Game", "High Scores", "Select Car Color"}, Color::Blue() ){

}

void F1GameStartScene::Init() {
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

	std::vector<Button::ButtonAction> actions;

	actions.push_back([this](){
		auto f1Game = std::make_unique<F1Game>();
		App::Singleton().PushScene(std::make_unique<GameScene>(std::move(f1Game)));
	});

	actions.push_back([this](){
	    F1Game tempGame;
	    auto lapTimes = tempGame.GetRecentLapTimes();
	    App::Singleton().PushScene(std::make_unique<F1GameHighScoresScene>(lapTimes));
	});

	actions.push_back([this](){
	    App::Singleton().PushScene(std::make_unique<F1GameColorScene>());
	});

	SetButtonActions(actions);

	ButtonOptionsScene::Init();
}

void F1GameStartScene::Update(uint32_t dt) {

}

void F1GameStartScene::Draw(Screen& theScreen) {
	ButtonOptionsScene::Draw(theScreen);
}

const std::string& F1GameStartScene::GetSceneName() const {
		static std::string name = "F1Game";
		return name;
}
