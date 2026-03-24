/*
 * ArcadeScene.cpp
 *
 *  Created on: 6 okt. 2025
 *      Author: QuintenDamsma
 */

#include "ArcadeScene.h"
#include "Screen.h"
#include "GameController.h"
#include <iostream>
#include "App.h"
#include "GameScene.h"
#include "../Games/BreakOut/BreakOut.h"
#include "NotImplementedScene.h"
#include "Pacman/PacmanStartScene.h"
#include "F1Game/F1GameStartScene.h"
#include "MainMenuScene.h"

ArcadeScene::ArcadeScene():ButtonOptionsScene({"Break Out!", "Asteroids", "Tetris", "Pac-man"}, Color::Cyan())
{

}

void ArcadeScene::Init()
{

	std::vector<Button::ButtonAction> actions;

	actions.push_back([this]{
		App::Singleton().PushScene(GetScene(TETRIS));
	});

	actions.push_back([this]{
		App::Singleton().PushScene(GetScene(BREAK_OUT));
	});

	actions.push_back([this]{
		App::Singleton().PushScene(GetScene(ASTEROIDS));
	});

	actions.push_back([this]{
		App::Singleton().PushScene(GetScene(PACMAN));
	});

    ButtonAction backToMenu;
    backToMenu.key = 's';
    backToMenu.action = [this](uint32_t dt, InputState state)
    {
        if(GameController::IsPressed(state))
        {

            App::Singleton().PopScene();
        }
    };

    mGameController.AddInputActionForKey(backToMenu);

	SetButtonActions(actions);

	ButtonOptionsScene::Init();
}

void ArcadeScene::Update(uint32_t dt)
{
}

void ArcadeScene::Draw(Screen& theScreen)
{
	ButtonOptionsScene::Draw(theScreen);
}

const std::string& ArcadeScene::GetSceneName() const
{
	static std::string sceneName = "Arcade";
	return sceneName;
}


std::unique_ptr<Scene> ArcadeScene::GetScene(eGame game)
{
	switch(game)
	{
		case TETRIS:
		{

		}
		break;

		case BREAK_OUT:
		{
			std::unique_ptr<BreakOut> breakoutGame = std::make_unique<BreakOut>();

			std::unique_ptr<GameScene> breakoutScene = std::make_unique<GameScene>(std::move(breakoutGame));

			return breakoutScene;
		}
		break;

		case ASTEROIDS:
		{

		}
		break;

		case PACMAN:
		{
			return std::make_unique<PacmanStartScene>();
		}
		break;

	}

	std::unique_ptr<Scene> notImplementedScene = std::make_unique<NotImplementedScene>();

	return notImplementedScene;
}


