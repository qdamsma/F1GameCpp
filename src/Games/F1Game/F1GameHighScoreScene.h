/*
 * F1GameHighScoreScene.h
 *
 *  Created on: 29 okt. 2025
 *      Author: QuintenDamsma
 */

#ifndef GAMES_F1GAME_F1GAMEHIGHSCORESCENE_H_
#define GAMES_F1GAME_F1GAMEHIGHSCORESCENE_H_

#include "Scene.h"
#include "Screen.h"
#include "Color.h"
#include "Vec2D.h"
#include <vector>
#include <string>

class F1GameHighScoresScene : public Scene
{
public:
    F1GameHighScoresScene(const std::vector<float>& lapTimes);

    void Init() override;
    void Update(uint32_t dt) override;
    void Draw(Screen& screen) override;
    const std::string& GetSceneName() const override;

private:
    std::vector<float> mLapTimes;
    Color mTextColor;
};



#endif /* GAMES_F1GAME_F1GAMEHIGHSCORESCENE_H_ */
