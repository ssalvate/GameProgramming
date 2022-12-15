#include <algorithm>
#include <glew.h>

// Engine
#include "SDL_image.h"
#include "Game.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

// Components
#include "Components/SpriteComponent.h"

// Actors
#include "GameActors/Actor.h"
#include "GameActors/Ship.h"
#include "GameActors/Asteroid.h"

Game::Game()
	: mWindow(nullptr)
	, mIsRunning(true)
	, mUpdatingActors(true)
	, mSpriteShader(nullptr)
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

	// Set OpenGL attributes
	// Use the core OpenGL profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// Specify version 3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// Request a color buffer with 8-bits per RGBA channel
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	// Enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// Force OpenGL to use hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// Create OpenGL window
	mWindow = SDL_CreateWindow(
		"Game Programming in C++", // Window title
		100,	// Top left x-coordinate of window
		100,	// Top left y-coordinate of window
		1024,	// Width of window
		768,	// Height of window
		SDL_WINDOW_OPENGL
	);

	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// Create an OpenGL context
	mContext = SDL_GL_CreateContext(mWindow);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}

	// On some platforms, GLEW will emit a benign error code,
	// so clear it
	glGetError();

	// Make sure we can create / compile shaders
	if (!LoadShaders())
	{
		SDL_Log("Failed to load shaders");
		return false;
	}

	// Create quad for drawing sprites
	CreateSpriteVerts();

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
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
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
	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	if (keyState[SDL_SCANCODE_ESCAPE] ) mIsRunning = false;

	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->ProcessInput(keyState);
	}
	mUpdatingActors = false;
}

void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// Delta time is the difference in ticks from last frame conveted to sec
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
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
		pending->ComputeWorldTransform();
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
	// 1.Set clear color to gray
	glClearColor(0.86f, 0.86f, 0.86f, 1.0f);
	// Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	// 2.Draw the scene
	// Set sprite shader and vertex array objects to active
	mSpriteShader->SetActive();
	mSpriteVerts->SetActive();
	
	for (auto sprite : mSprites)
	{
		sprite->Draw(mSpriteShader);
	}

	// 3.Swap front and back buffers, which also displays the scene
	SDL_GL_SwapWindow(mWindow);
}

bool Game::LoadShaders()
{
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
	{
		return false;
	}
	mSpriteShader->SetActive();

	// Set the 'simple' view-projection matrix
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(1024.0f, 768.0f);
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);
	return true;
}

// Create rect polygon to place sprites on (a unit square)
void Game::CreateSpriteVerts()
{
	float vertexBuffer[] = {
		-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, // Vertex 0 with UV coords, top left
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,	// top right
		 0.5f, -0.5f, 0.0f,	1.0f, 1.0f,	// bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f	// bottom left
	};

	unsigned int indexBuffer[] = {
		0,1,2,
		2,3,0
	};

	mSpriteVerts = new VertexArray(vertexBuffer, 4, indexBuffer, 6);
}

void Game::LoadData()
{
	// Create player ship
	mShip = new Ship(this);
	mShip->SetPosition(Vector2::Zero);
	mShip->SetScale(1.0f);

	// Create asteroids
	const int numAsteroids = 20;
	for (int i = 0; i < numAsteroids; i++)
	{
		new Asteroid(this);
	}
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
		i.second->Unload();
		delete i.second;
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

Texture* Game::GetTexture(const std::string& fileName)
{
	Texture* tex = nullptr;
	// Is the texture already loaded?
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		// Create new texture 
		tex = new Texture();
		if (tex->Load(fileName))
		{
			mTextures.emplace(fileName, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}

	}
	return tex;
}

void Game::AddAsteroid(Asteroid* ast)
{
	mAsteroids.emplace_back(ast);
}

void Game::RemoveAsteroid(Asteroid* ast)
{
	auto iter = std::find(mAsteroids.begin(), mAsteroids.end(), ast);
	if (iter != mAsteroids.end())
	{
		mAsteroids.erase(iter);
	}
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
	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	// We can't swap because it braks ordering
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}