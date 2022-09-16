#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "SDL.h"

class Game {
public:
	Game();
	
	// Initialize the game
	bool Initialize();
	// Runs the game loop until the game is over
	void RunLoop();
	// Shutdown the game
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	SDL_Texture* GetTexture(const std::string& fileName);

private:
	// Helper functions for the game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	// Window created by SDL
	SDL_Window* mWindow;
	// 2D Renderer created by SDL
	SDL_Renderer* mRenderer;
	// Number of ticks since start of game
	Uint32 mTicksCount;
	// Game should continue to run
	bool mIsRunning;
	// Track if we're updating actors right now
	bool mUpdatingActors;

	// All actors in game
	std::vector<class Actor*> mActors;
	// Any pending actors
	std::vector<class Actor*> mPendingActors;

	// Map of textures loaded
	std::unordered_map<std::string, SDL_Texture*> mTextures;
	// All of the sprite components drawn
	std::vector<class SpriteComponent*> mSprites;

	// Game-specific
	class Ship* mShip; // Player ship
};