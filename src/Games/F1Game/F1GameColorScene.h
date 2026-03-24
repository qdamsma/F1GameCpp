/*
 * F1GameColorScene.h
 *
 *  Created on: 23 okt. 2025
 *      Author: QuintenDamsma
 */

#ifndef GAMES_F1GAME_F1GAMECOLORSCENE_H_
#define GAMES_F1GAME_F1GAMECOLORSCENE_H_


#include "ButtonOptionsScene.h"
#include "Color.h"

class F1GameColorScene : public ButtonOptionsScene
{
public:
    F1GameColorScene();
    virtual void Init() override;
    virtual const std::string& GetSceneName() const override;

    static Color GetSelectedColor();
    static void SetSelectedColor(const Color& color);

private:
    static Color sSelectedColor;
};


#endif /* GAMES_F1GAME_F1GAMECOLORSCENE_H_ */
