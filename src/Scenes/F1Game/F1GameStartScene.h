/*
 * F1GameStartScene.h
 *
 *  Created on: 13 okt. 2025
 *      Author: QuintenDamsma
 */

#ifndef SCENES_F1GAME_F1GAMESTARTSCENE_H_
#define SCENES_F1GAME_F1GAMESTARTSCENE_H_

#include "ButtonOptionsScene.h"

class F1GameStartScene: public ButtonOptionsScene{
public:
	 F1GameStartScene();
	 void Init() override;
	 void Update(uint32_t dt) override;
	 void Draw(Screen& theScreen) override;
	 const std::string& GetSceneName() const override;
private:

};

#endif /* SCENES_F1GAME_F1GAMESTARTSCENE_H_ */
