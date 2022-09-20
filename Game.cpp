#include <algorithm>

#include "SDL_image.h"
#include "Game.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "BGSpriteComponent.h"

#include "Ship.h"
#include "Skeleton.h"

Game::Game()
	: mWindow(nullptr)
	, mRenderer(nullptr)
	, mIsRunning(true)
	, mUpdatingActors(true)
{
}

bool Game::Initialize()
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Create SDL window
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 2)", // Window title
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
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	// Init image loader (SDL)
	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Failed to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	
	// Init game objects
	LoadData();

	mTicksCount = SDL_GetTicks();
	
	SDL_Log("Game initialized!");
	return true;
}

void Game::Shutdown()
{
	UnloadData();
	IMG_Quit();
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
		case SDL_QUIT: // Window click X
			mIsRunning = false;
			break;
		}
	}
	// Get state of the keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if ( state[SDL_SCANCODE_ESCAPE] ) mIsRunning = false;

	// Process ship input
	mShip->ProcessKeyboard(state);
	mSkel->ProcessKeyboard(state);

}

void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame
	// Frame limiting to 60 fps
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// Delta time is the difference in ticks from last frame conveted to sec
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 100.0f;
	// Clamp max deltaTime
	if (deltaTime > 0.05f) deltaTime = 0.05f;
	// update tick count for next frame
	mTicksCount = SDL_GetTicks();


	// --Update game objects(Actors) as a function of deltaTime
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	
	mUpdatingActors = false;
	// move any pending actors to mActors
	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// Add any dead actors to a temp vector
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	// Delete dead actors (which removes them from mActors)
	for (auto actor : deadActors)
	{
		delete actor;
	}

}

void Game::GenerateOutput()
{
	// 1.Set draw color to black  + (Clear backbuffer)
	SDL_SetRenderDrawColor(
		mRenderer,
		0,    // R
		0,    // G
		0,    // B
		255   // A
	);
	SDL_RenderClear(mRenderer);

	// 2.Drawn entire Game screen 
	
	// Draw all sprite components
	for (auto sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}

	// 3.Swap front and back buffers
	SDL_RenderPresent(mRenderer);
}

void Game::LoadData()
{
	// Create player ship
	mShip = new Ship(this);
	mShip->SetPosition(Vector2(100.0f, 384.0f));
	mShip->SetScale(1.5f);

	mSkel = new Skeleton(this);
	mSkel->SetPosition(Vector2(100.0f, 180.0f));

	// Create actor for the background (this doesn't need a subclass)
	Actor* temp = new Actor(this);
	temp->SetPosition(Vector2(512.0f, 384.0f));
	// Create the closer background
	BGSpriteComponent* bg = new BGSpriteComponent(temp,50);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	std::vector<SDL_Texture*> bgtexs = {
		GetTexture("Assets/Stars.png"),
		GetTexture("Assets/Stars.png")
		
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-200.0f);

	// Create the "far back" background
	bg = new BGSpriteComponent(temp);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	bgtexs = {
		GetTexture("Assets/Farback01.png"),
		GetTexture("Assets/Farback02.png")
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-100.0f);

}

void Game::UnloadData()
{
	// Delete actors
	// beacuse ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// Destroy textures
	for (auto i : mTextures)
	{
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();

}

void Game::AddActor(Actor* actor)
{

	// If still updating Actors, add to pending
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}

}

void Game::RemoveActor(Actor* actor)
{
	// Is it in pending actors?
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		// swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// Is it in actors?
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}

}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;
	// Is the texture already loaded?
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		// Load from file
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf) 
		{
			SDL_Log("Failed to load texture file %s", fileName.c_str());
			return nullptr;
		}

		// Create texture from surface
		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		SDL_FreeSurface(surf);
		if (!tex)
		{
			SDL_Log("Failed to convert surface to texture %s", fileName.c_str());
			return nullptr;
		}

		mTextures.emplace(fileName.c_str(), tex);
	}

	return tex;
}

void Game::AddSprite(SpriteComponent* sprite)
{
	// Find insert point in the sorted vector
	// (the first elem with a higher draw order than me)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (; iter != mSprites.begin(); ++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder()) break;

	}
	// Insert elem before position of iterator
		//printf("DrawOrder in Array\n");
	mSprites.insert(iter, sprite);
		/*for (auto i : mSprites) {
			printf(" %i \n", i->GetDrawOrder());
		}
		printf("---End Sprite Added \n\n");*/

}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	// We can't swap because it braks ordering
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}