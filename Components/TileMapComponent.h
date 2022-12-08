#pragma once

#include "SpriteComponent.h"

#include <string>
#include <vector>

class TileMapComponent : public SpriteComponent
{
public:
	TileMapComponent(class Actor* owner, int drawOrder);

	void Draw(SDL_Renderer* renderer) override;

	void LoadTileMap(const std::string& filename);

private:
	// 2d Vector to hold csv
	std::vector<std::vector<int>> mTileMap;
	int mTileWidth;
	int mTileHeight;
};