/*
 * MainMenuScene.h
 *
 *  Created on: 23 okt. 2025
 *      Author: QuintenDamsma
 */

#ifndef SCENES_MAINMENUSCENE_H_
#define SCENES_MAINMENUSCENE_H_

#pragma once
#include "ButtonOptionsScene.h"
#include <memory>

class MainMenuScene : public ButtonOptionsScene
{
public:
    MainMenuScene();
    virtual void Init() override;
    virtual const std::string& GetSceneName() const override;
private:
    std::unique_ptr<Scene> GetScene(int option);
};



#endif /* SCENES_MAINMENUSCENE_H_ */
