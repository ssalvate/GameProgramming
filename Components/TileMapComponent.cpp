#include "TileMapComponent.h"
#include "../Game.h"
#include "../GameActors/Actor.h"

#include <iostream>
#include <fstream>
#include <sstream>

TileMapComponent::TileMapComponent(Actor* owner, int drawOrder)
	: SpriteComponent(owner, drawOrder)
	, mTileHeight(32)
	, mTileWidth(32)
{
}

void TileMapComponent::Draw(SDL_Renderer* renderer)
{
	if (!mTileMap.empty())
	{
		SDL_Rect srcR;
		SDL_Rect destR;

		// Loop through the loaded tile map
		for (int y = 0; y < mTileMap.size(); y++)
		{
			for (int x = 0; x < mTileMap.at(0).size(); x++)
			{
				// Get number of tile from csv(map)
				int tileNum = mTileMap.at(y).at(x);
				// -1 means empty tile
				if (tileNum == -1) continue;

				// Scale width/height by owners width/height
				destR.h = static_cast<int>(mTileHeight * mOwner->GetScale());
				destR.w = static_cast<int>(mTileWidth * mOwner->GetScale());
				srcR.w = static_cast<int>(mTileWidth * mOwner->GetScale());
				srcR.h = static_cast<int>(mTileHeight * mOwner->GetScale());
				
				// Coordinates to draw tile at proper pos
				destR.x = static_cast<int>(x * 32);
				destR.y = static_cast<int>(y * 32);

				// Calculate which tile you are grabbing
				srcR.x = (tileNum % 8) * 32;
				srcR.y = (tileNum / 8) * 32;

				// Draw (have to convert angle from radians to degrees, and clockwise to counter)
				SDL_RenderCopyEx(renderer,
					mTexture,
					&srcR,
					&destR,
					-Math::ToDegrees(mOwner->GetRotation()),
					nullptr,
					SDL_FLIP_NONE);
			}
		}

	}
}

void TileMapComponent::LoadTileMap(const std::string& filename)
{
	std::vector<std::vector<int>> content;
	std::vector<int> row;
	std::string line, word;

	// Open csv file and read in contents line by line (rows)
	std::fstream file(filename, std::ios::in);
	if (file.is_open())
	{
		while (getline(file, line))
		{
			// Dummy row container(vector), clear it each loop through file row
			row.clear();
			// Row contents with value,value from file
			std::stringstream str(line); 

			// Loop through the row by ',', appending each word
			while (getline(str, word, ',')) 
			{
				row.push_back(stoi(word)); 
			}

			mTileMap.push_back(row);

		}
	}

}
