/*
 * F1GameLevel.h
 *
 *  Created on: 14 okt. 2025
 *      Author: QuintenDamsma
 */

#ifndef GAMES_F1GAME_F1GAMELEVEL_H_
#define GAMES_F1GAME_F1GAMELEVEL_H_

#include "Vec2D.h"
#include "../BreakOut/Excluder.h"
#include <string>
#include <vector>
#include "F1GameUtils.h"
#include "BMPImage.h"

class Screen;
class Car;
class Opponent;

class F1GameLevel{
public:

	bool Init(const std::string& levelPath);
	bool Update(uint32_t dt, Car& car, std::vector<Opponent>& opps);
	void Draw(Screen& screen);

	inline Vec2D GetLayoutOffset() const {return mLayoutOffset;}
	void ResetLevel();

	bool WillCollide(const AARectangle& bbox, CarMovement direction) const;
	inline Vec2D GetCarSpawnLocation() const{return mCarSpawnLocation;}
	bool IsLevelOver() const;
	void IncreaseLevel();
	void ResetToFirstLevel();
	inline const std::vector<Vec2D>& GetOppsSpawnPoints(){return mOppsSpawnPoints;}



private:
	struct Tile{
		Vec2D position = Vec2D::Zero;
		Vec2D offset = Vec2D::Zero;
		int width = 0;
		int collidable = 0;
		int excludeFinishTile = 0;
		int carSpawnPoint = 0;
		int bronzeSpawnPoint = 0;
		char symbol = 'g';
	};

	struct Finish{
		uint32_t score = 0;
		AARectangle mBBox;
		int powerPellet = 0;
		int eaten = 0;
		char symbol = 'f';
	};

	bool LoadLevel(const std::string& levelPath);
	Tile* GetTileForSymbol(char symbol);
	void ResetFinish();
	bool HasCrossedFinish() const;

	BMPImage mBGImage;

	std::vector<Vec2D> mOppsSpawnPoints;

	std::vector<Excluder> mWalls;
	std::vector<Tile> mTiles;
	std::vector<Tile> mExclusionTiles;
	std::vector<Tile> mLevelTiles;
	std::vector<Finish> mFinish;

	Vec2D mCarSpawnLocation;
	Vec2D mLayoutOffset;
	size_t mTileHeight;
	size_t mTileWidth;
	int mCurrentLevel;

	std::string mLevelPath;

};


#endif /* GAMES_F1GAME_F1GAMELEVEL_H_ */
