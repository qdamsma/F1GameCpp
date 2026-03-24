/*
 * F1GameLevel.cpp
 *
 *  Created on: 14 okt. 2025
 *      Author: QuintenDamsma
 */


#include "F1GameLevel.h"
#include "FileCommandLoader.h"
#include "Screen.h"
#include "App.h"
#include "Circle.h"
#include "Opponent.h"
#include "Car.h"
#include <cassert>

bool F1GameLevel::Init(const std::string& levelPath){
	mLevelPath = levelPath;
	mOppsSpawnPoints.resize(NUM_OPPS);
	bool levelLoaded = LoadLevel(levelPath);
	if(levelLoaded){
//		ResetLevel();
	}

	return levelLoaded;
}

bool F1GameLevel::Update(uint32_t dt, Car& car, std::vector<Opponent>& opps){
	for(auto& wall : mWalls){
		BoundaryEdge edge;

		if(wall.HasCollided(car.GetBoundingBox(), edge)){
			Vec2D offset = wall.GetCollisionOffset(car.GetBoundingBox());
			car.MoveBy(offset);
			car.Stop();
		}

		for(auto& opponent : opps){
			if(wall.HasCollided(opponent.GetBoundingBox(), edge)){
				Vec2D offset = wall.GetCollisionOffset(opponent.GetBoundingBox());
				opponent.MoveBy(offset);
				opponent.Stop();
			}
		}
	}

	for(auto & finish : mFinish){
	        if(car.GetEatingBoundingBox().Intersects(finish.mBBox)){
	            return true;
	        }
	    }
	 return false;
}

void F1GameLevel::Draw(Screen& screen){

	Sprite bgSprite;
	bgSprite.width = mBGImage.GetWidth();
	bgSprite.height = mBGImage.GetHeight();

	screen.Draw(mBGImage, bgSprite, Vec2D::Zero);

	//Debug voor muren
//	for(const auto& wall: mWalls){
//		screen.Draw(wall.GetAARectangle(), Color::Blue());
//	}

	// Tekenen van finishpunten (alleen de 'f' tiles)
//	    for (const auto& finish : mFinish)
//	    {
//	        if (!finish.eaten)
//	        {
//	            Color finishColor = Color::Yellow(); // duidelijk zichtbaar
//	            screen.Draw(finish.mBBox, finishColor);
//	        }
//	    }
}


bool F1GameLevel::WillCollide(const AARectangle& aBBox, CarMovement direction) const{
	AARectangle bbox = aBBox;

	bbox.MoveBy(GetMovementVector(direction));
	for( const auto& wall : mWalls){
		BoundaryEdge edge;

		if(wall.HasCollided(bbox, edge)){
			return true;
		}
	}
	return false;
}

void F1GameLevel::ResetLevel(){
	ResetFinish();

	LoadLevel(mLevelPath);
}

void F1GameLevel::ResetFinish()
{
    mFinish.clear();
}

bool F1GameLevel::LoadLevel(const std::string& levelPath){
	FileCommandLoader fileLoader;

	std::string bgImageName;

	Command bgImageCommand;
	bgImageCommand.command = "bg_image";
	bgImageCommand.parseFunc = [this, &bgImageName](ParseFuncParams params){
		bgImageName = FileCommandLoader::ReadString(params);
		bool loaded = mBGImage.Load(App::Singleton().GetBasePath() + std::string("Assets/F1Game_track.bmp"));
		assert(loaded && "Didn't load the bg image");
	};

	fileLoader.AddCommand(bgImageCommand);


	Command tileWidthCommand;
	tileWidthCommand.command = "tile_width";
	tileWidthCommand.parseFunc = [this](ParseFuncParams params){
		int width = FileCommandLoader::ReadInt(params); if (!mTiles.empty()) {
			mTiles.back().width = width; } else {
				for (auto& tile : mTiles) {
					tile.width = width;
				}
			}
			if (width > 0) {
				mTileWidth = width;
			}
	};

	fileLoader.AddCommand(tileWidthCommand);

	Command tileHeightCommand; tileHeightCommand.command = "tile_height";
	tileHeightCommand.parseFunc = [this](ParseFuncParams params){
		mTileHeight = FileCommandLoader::ReadInt(params);
	};

	fileLoader.AddCommand(tileHeightCommand);

	Command tileCommand; tileCommand.command = "tile";
	tileCommand.parseFunc = [this](ParseFuncParams params){
		mTiles.push_back(Tile()); }; fileLoader.AddCommand(tileCommand);
	Command tileSymbolCommand; tileSymbolCommand.command = "tile_symbol";
	tileSymbolCommand.parseFunc = [this](ParseFuncParams params){
		mTiles.back().symbol = FileCommandLoader::ReadChar(params);
	};

	fileLoader.AddCommand(tileSymbolCommand);

	Command tileCollisionCommand;
	tileCollisionCommand.command = "tile_collision";
	tileCollisionCommand.parseFunc = [this](ParseFuncParams params){
		mTiles.back().collidable = FileCommandLoader::ReadInt(params);
	};

	fileLoader.AddCommand(tileCollisionCommand);

	Vec2D layoutOffset;
	Command layoutOffsetCommand;
	layoutOffsetCommand.command = "layout_offset";
	layoutOffsetCommand.parseFunc = [&layoutOffset, this](ParseFuncParams params){
		mLayoutOffset = FileCommandLoader::ReadSize(params);
		layoutOffset = mLayoutOffset;
	};

	fileLoader.AddCommand(layoutOffsetCommand);

	Command tileExcludeFinishCommand;
	tileExcludeFinishCommand.command = "tile_exclude_finish";
	tileExcludeFinishCommand.parseFunc = [this](ParseFuncParams params)
			{
		mTiles.back().excludeFinishTile = FileCommandLoader::ReadInt(params);

		};

	fileLoader.AddCommand(tileExcludeFinishCommand);

	Command tileCarSpawnPointCommand;
	tileCarSpawnPointCommand.command = "tile_car_spawn_point";
	tileCarSpawnPointCommand.parseFunc = [this](ParseFuncParams params)
		{
			mTiles.back().carSpawnPoint = FileCommandLoader::ReadInt(params);
		};

	fileLoader.AddCommand(tileCarSpawnPointCommand);

	Command tileBronzeSpawnPointCommand;
	tileBronzeSpawnPointCommand.command = "tile_bronze_spawn_point";
	tileBronzeSpawnPointCommand.parseFunc = [this](ParseFuncParams params)
		{
			mTiles.back().bronzeSpawnPoint = FileCommandLoader::ReadInt(params);
		};

	fileLoader.AddCommand(tileBronzeSpawnPointCommand);


	Command layoutCommand;
	layoutCommand.command = "layout";
	layoutCommand.commandType = COMMAND_MULTI_LINE;
	layoutCommand.parseFunc = [&layoutOffset, this](ParseFuncParams params){
		int startingX = layoutOffset.GetX();

	    for(int c = 0; c < params.line.length(); ++c)
	    {
	        char symbol = params.line[c];
	        Tile* tile = GetTileForSymbol(symbol);

	        if(tile)
	        {
	            tile->position = Vec2D(startingX, layoutOffset.GetY());

	            // muren
	            if(tile->collidable > 0)
	            {
	                Excluder wall;
	                wall.Init(AARectangle(Vec2D(startingX, layoutOffset.GetY()),
	                                      tile->width, static_cast<int>(mTileHeight)));
	                mWalls.push_back(wall);
	            }

	            if(tile->carSpawnPoint > 0){
	            	mCarSpawnLocation = Vec2D(startingX + tile->offset.GetX(), layoutOffset.GetY() + tile->offset.GetY());
	            }
	            else if(tile->bronzeSpawnPoint > 0){
	            	mOppsSpawnPoints[BRONZE] = Vec2D(startingX + tile->offset.GetX(), layoutOffset.GetY() + tile->offset.GetY());
	            }

	            // finishpunten
	            if(symbol == 'f')
	            {
	                Finish f;
	                f.mBBox = AARectangle(Vec2D(startingX, layoutOffset.GetY()),
	                                      tile->width, static_cast<int>(mTileHeight));

	                mFinish.push_back(f);
	            }

	            startingX += tile->width;
	        }
	    }

	    layoutOffset += Vec2D(0, mTileHeight);
	};

	fileLoader.AddCommand(layoutCommand);
	return fileLoader.LoadFile(levelPath);
}


F1GameLevel::Tile* F1GameLevel::GetTileForSymbol(char symbol){
	for(size_t i = 0; i < mTiles.size(); i++){
		if(mTiles[i].symbol == symbol){
			return &mTiles[i];
		}
	} return nullptr;
}
