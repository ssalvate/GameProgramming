#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
	:mIsRunning(true), mWindow(nullptr)
{
}

bool Game::Initialize()
{
	// Initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	if (sdlResult != 0) {
		SDL_Log("Unable to Initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Create SDL window
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)", // Window title
		100,	// Top left x-coordinate of window
		100,	// Top left y-coordinate of window
		1024,	// Width of window
		768,	// Height of window
		0		// Flags (0 for no flags set)
	);

	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// Create SDL renderer
	mRenderer = SDL_CreateRenderer(
		mWindow,// window to create rendere for
		-1,     // Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		SDL_Log("Failed to create render: %s", SDL_GetError());
		return false;
	}

	// Init game objects
	mPaddlePos.x = 10.0f;
	mPaddlePos.y = 768.0f / 2.0f;
	mBallPos.x = 1024.0f / 2.0f;
	mBallPos.y = 768.0f / 2.0f;
	mBallVel = Vector2{ -30.0f, 33.0f };


	return true;
}

void Game::Shutdown()
{
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();
}

void Game::RunLoop() 
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}

}

void Game::ProcessInput()
{
	SDL_Event event;

	// While there are still events in the queue
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// Handle different event types here
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}
	// Get state of the keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if ( state[SDL_SCANCODE_ESCAPE] ) mIsRunning = false;

	// Update paddle direction based on W/S keys
	mPaddleDir = 0;
	//Adding to paddle dir so if you press both it cancels out
	if (state[SDL_SCANCODE_W]) mPaddleDir -= 1;
	if (state[SDL_SCANCODE_S]) mPaddleDir += 1;

}

void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame
	// Frame limiting to 60 fps
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;


	// Delta time is the difference in ticks from last frame conveted to sec
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 100.0f;
	// update tick count for next frame
	mTicksCount = SDL_GetTicks();

	// Clamp max deltaTime
	if (deltaTime > 0.5f) deltaTime = 0.5f;

	// --Update game objects as a function of deltaTime

	// Update paddle position based on direction
	if (mPaddleDir != 0) {
		mPaddlePos.y += mPaddleDir * 100.0f * deltaTime;

		// Make sure paddle doesn't move off screen!
		if (mPaddlePos.y < paddleH / 2.0f + thickness) {
			mPaddlePos.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddlePos.y > 768.0f - paddleH / 2.0f - thickness) {
			mPaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	// Update ball position based on ball velocity
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	//--Check for collision

	//Paddle / Horizontals
	float diff = mPaddlePos.y - mBallPos.y;
	// Take absolute value of difference
	diff = (diff > 0.0f) ? diff : -diff;
	if (
		// the y-difference is small enough
		diff <= paddleH / 2.0f &&
		// We are in the correct x-position + buffer space (i.e between 25-20)
		mBallPos.x <= mPaddlePos.x + thickness && mBallPos.x >= mPaddlePos.x + 10.0f &&
		// We are moving left
		mBallVel.x <= 0.0f
		) 
	{
		mBallVel.x *= -1.0f;
	}
	//Left out-of-bounds
	else if (mBallPos.x <= 0.0f) {
		mIsRunning = false;
	}
	//Right wall
	else if(mBallPos.x + thickness >= 1024.0f && mBallVel.x >= 0.0f)
	{
		mBallVel.x *= -1.0;
	}

	//-- Verticals
	
	//Top Wall
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f) {
		mBallVel.y *= -1;
	} 
	//Bottom Wall
	else if(mBallPos.y >= (768.0f - thickness) && mBallVel.y > 0.0f) {
		mBallVel.y *= -1;
	}

}

void Game::GenerateOutput()
{
	// 1.Set draw color to blue (Clear backbuffer)
	SDL_SetRenderDrawColor(
		mRenderer,
		0,    // R
		0,    // G
		255,  // B
		255   // A
	);
	SDL_RenderClear(mRenderer);

	// 2.Drawn entire Game screen 
	
	// Draw walls
	SDL_SetRenderDrawColor(mRenderer, 0, 255, 0, 255);
	SDL_Rect wall{0,0,1024, thickness};
	SDL_RenderFillRect(mRenderer, &wall);
	// Draw bottom wall
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);
	// Draw right wall
	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 1024;
	SDL_RenderFillRect(mRenderer, &wall);

	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	// Draw Ball
	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	// Draw Paddle
	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - paddleH/2.0f),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle);

	// 3.Swap front and back buffers
	SDL_RenderPresent(mRenderer);
}