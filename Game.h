#pragma once

#include <vector>

#include "SDL.h"

struct Vector2
{
	float x;
	float y;
};

struct Ball
{
	Vector2 pos;
	Vector2 vel;
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
	Vector2 mPaddle2Pos;
	// Direction of paddle
	int mPaddleDir;
	int mPaddle2Dir;
	
	//Vector of balls
	std::vector<Ball> mBalls;
	// Ball pos
		//Vector2 mBallPos;
	// Ball velocity
		//Vector2 mBallVel;
};