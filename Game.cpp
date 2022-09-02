#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
	:mWindow(nullptr)
	, mRenderer(nullptr)
	, mTicksCount(0)
	, mIsRunning(true)
	, mPaddleDir(0)
	, mPaddle2Dir(0)
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
	mPaddle2Pos.x = 1024.0f - (10.0f+ thickness);
	mPaddle2Pos.y = 768.0f / 2.0f;
	Ball a{};
	a.pos.x = 1024.0f / 2.0f;
	a.pos.y = 768.0f / 2.0f;
	a.vel.x = 33.0f ;
	a.vel.y =  33.0f;
	mBalls.push_back(a);
	Ball b{};
	b.pos.x = 1024.0f / 2.0f;
	b.pos.y = 768.0f / 2.0f;
	b.vel.x = -13.0f ;
	b.vel.y =  -13.0f;
	mBalls.push_back(b);

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
	mPaddle2Dir = 0;
	//Adding to paddle dir so if you press both it cancels out
	if (state[SDL_SCANCODE_W]) mPaddleDir -= 1;
	if (state[SDL_SCANCODE_S]) mPaddleDir += 1;
	if (state[SDL_SCANCODE_UP]) mPaddle2Dir -= 1;
	if (state[SDL_SCANCODE_DOWN]) mPaddle2Dir += 1;

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
	// Update paddle2 position based on direction
	if (mPaddle2Dir != 0) {
		mPaddle2Pos.y += mPaddle2Dir * 100.0f * deltaTime;

		// Make sure paddle doesn't move off screen!
		if (mPaddle2Pos.y < paddleH / 2.0f + thickness) {
			mPaddle2Pos.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddle2Pos.y > 768.0f - paddleH / 2.0f - thickness) {
			mPaddle2Pos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	for (Ball &b : mBalls) {

		// Update ball position based on ball velocity
		b.pos.x += b.vel.x * deltaTime;
		b.pos.y += b.vel.y * deltaTime;

		//--Check for collision

		//Paddles
		float diff = mPaddlePos.y - b.pos.y;
		// Take absolute value of difference
		diff = (diff > 0.0f) ? diff : -diff;
		if (
			// the y-difference is small enough
			diff <= paddleH / 2.0f &&
			// We are in the correct x-position + buffer space (i.e between 25-20)
			b.pos.x <= mPaddlePos.x + thickness && b.pos.x >= mPaddlePos.x + 10.0f &&
			// We are moving left
			b.vel.x <= 0.0f
			)
		{
			b.vel.x *= -1.0f;
		}
		float diff2 = mPaddle2Pos.y - b.pos.y;
		// Take absolute value of difference
		diff2 = (diff2 > 0.0f) ? diff2 : -diff2;
		if (
			// the y-difference is small enough
			diff2 <= paddleH / 2.0f &&
			// We are in the correct x-position + buffer space (i.e between 25-20)
			b.pos.x + thickness >= mPaddle2Pos.x + 10.0f && b.pos.x <= mPaddle2Pos.x &&
			// We are moving left
			b.vel.x >= 0.0f
			)
		{
			b.vel.x *= -1.0f;
		}
		//Left or Right out-of-bounds
		else if (b.pos.x <= 0.0f || b.pos.x >= 1024.0f) {
			mIsRunning = false;
		}

		//-- Verticals

		//Top Wall
		if (b.pos.y <= thickness && b.vel.y < 0.0f) {
			b.vel.y *= -1;
		}
		//Bottom Wall
		else if (b.pos.y >= (768.0f - thickness) && b.vel.y > 0.0f) {
			b.vel.y *= -1;
		}
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

	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	// Draw Balls
	for (Ball &b : mBalls) {
		SDL_Rect ball{
			static_cast<int>(b.pos.x - thickness / 2),
			static_cast<int>(b.pos.y - thickness / 2),
			thickness,
			thickness
		};
		SDL_RenderFillRect(mRenderer, &ball);
	}

	// Draw Paddles
	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - paddleH/2.0f),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle);
	SDL_Rect paddle2{
		static_cast<int>(mPaddle2Pos.x),
		static_cast<int>(mPaddle2Pos.y - paddleH / 2.0f),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle2);

	// 3.Swap front and back buffers
	SDL_RenderPresent(mRenderer);
}