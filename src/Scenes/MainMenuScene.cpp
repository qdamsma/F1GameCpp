/*
 * MainMenuScene.cpp
 *
 *  Created on: 23 okt. 2025
 *      Author: QuintenDamsma
 */

#include "MainMenuScene.h"
#include "App.h"
#include "ArcadeScene.h"
#include "F1Game/F1GameStartScene.h"

MainMenuScene::MainMenuScene()
    : ButtonOptionsScene({"F1 Game", "Tutorial Games"}, Color::Cyan())
{
}

void MainMenuScene::Init()
{
    std::vector<Button::ButtonAction> actions;

    actions.push_back([this]{
        App::Singleton().PushScene(std::make_unique<F1GameStartScene>());
    });

    actions.push_back([this]{
        App::Singleton().PushScene(std::make_unique<ArcadeScene>());
    });

    SetButtonActions(actions);
    ButtonOptionsScene::Init();
}

const std::string& MainMenuScene::GetSceneName() const
{
    static std::string sceneName = "Main Menu";
    return sceneName;
}


