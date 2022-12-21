#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "SDL.h"
#include "Math.h"

class Game {
public:
	Game();
	
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	class Renderer* GetRenderer() { return mRenderer; }

private:
	// Helper functions for the game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	// Number of ticks since start of game
	Uint32 mTicksCount;
	// Game should continue to run
	bool mIsRunning;

	// All actors in game
	std::vector<class Actor*> mActors;
	// Any pending actors
	std::vector<class Actor*> mPendingActors;
	// Track if we're updating actors right now
	bool mUpdatingActors;

	class Renderer* mRenderer;

	// Game-specific
};