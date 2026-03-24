/*
 * F1GameColorScene.cpp
 *
 *  Created on: 23 okt. 2025
 *      Author: QuintenDamsma
 */



#include "F1GameColorScene.h"
#include "App.h"
#include "GameController.h"

Color F1GameColorScene::sSelectedColor;

F1GameColorScene::F1GameColorScene()
    : ButtonOptionsScene({"Geel", "Rood", "Blauw", "Groen"}, Color::Cyan())
{
}

//Acties om kleur te bepalen van de auto
void F1GameColorScene::Init()
{

	std::vector<Button::ButtonAction> actions;

	actions.push_back([this](){
	    F1GameColorScene::sSelectedColor = Color::Yellow();
	    App::Singleton().PopScene();
	});

	actions.push_back([this](){
	    F1GameColorScene::sSelectedColor = Color::Red();
	    App::Singleton().PopScene();
	});

	actions.push_back([this](){
	    F1GameColorScene::sSelectedColor = Color::Blue();
	    App::Singleton().PopScene();
	});

	actions.push_back([this](){
	    F1GameColorScene::sSelectedColor = Color::Green();
	    App::Singleton().PopScene();
	});

    SetButtonActions(actions);
    ButtonOptionsScene::Init();

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

const std::string& F1GameColorScene::GetSceneName() const
{
    static std::string name = "Select Car Color";
    return name;
}

Color F1GameColorScene::GetSelectedColor()
{
	if (sSelectedColor.GetRed() == 0 &&  sSelectedColor.GetGreen() == 0 && sSelectedColor.GetBlue() == 0){
	      sSelectedColor = Color(255, 255, 0, 255);
	}
    return sSelectedColor;
}

void F1GameColorScene::SetSelectedColor(const Color& color)
{
    sSelectedColor = color;
}
