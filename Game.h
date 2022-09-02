#pragma once

#include "SDL.h"

struct Vector2
{
	float x;
	float y;
};

struct Game {
	Game();
	
	// Initialize the game
	bool Initialize();
	// Runs the game loop until the game is over
	void RunLoop();
	// Shutdown the game
	void Shutdown();

private:
	// Helper functions for the game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	// Window created by SDL
	SDL_Window* mWindow;
	// 2D Renderer created by SDL
	SDL_Renderer* mRenderer;
	// Number of ticks since start of game
	Uint32 mTicksCount;
	// Game should continue to run
	bool mIsRunning;

	// Pong specific
	// Paddle pos
	Vector2 mPaddlePos;
	// Direction of paddle
	int mPaddleDir;
	// Ball pos
	Vector2 mBallPos;
	// Ball velocity
	Vector2 mBallVel;
};