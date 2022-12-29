#include <algorithm>

// Engine
#include "Game.h"
#include "Renderer.h"
// Actors
#include "GameActors/Actor.h"
#include "GameActors/PlaneActor.h"
#include "GameActors/CameraActor.h"
// Components
#include "Components/MeshComponent.h"
#include "Components/SpriteComponent.h"

Game::Game()
	: mRenderer(nullptr)
	, mIsRunning(true)
	, mUpdatingActors(false)
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

	// Create the Renderer
	mRenderer = new Renderer(this);
	if (!mRenderer->Initialize(1024.0f, 768.0f))
	{
		SDL_Log("Failed to initialize renderer");
		delete mRenderer;
		mRenderer = nullptr;
		return false;
	}

	// Init game objects
	LoadData();

	mTicksCount = SDL_GetTicks();
	
	SDL_Log("Game initialized!");
	return true;
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

void Game::Shutdown()
{
	UnloadData();
	if (mRenderer)
	{
		mRenderer->Shutdown();
	}
	SDL_Quit();
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

	for (auto actor : mActors)
	{
		actor->ProcessInput(keyState);
	}
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
	mRenderer->Draw();
}

void Game::LoadData()
{
	// ---- Game Specific Code ----
	
	// Cube
	Actor* a = new Actor(this);
	a->SetPosition(Vector3(200.0f, 75.0f, 0.0f));
	a->SetScale(100.0f);
	// Textured side is top when first loaded, with text facing away
	// Rotate around z by 180d + 45d, 
	// then rotate around z -90d
	Quaternion q(Vector3::UnitY, -Math::PiOver2); 
	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::Pi + Math::Pi / 4.0f));
	a->SetRotation(q);
	MeshComponent* mc = new MeshComponent(a);
	mc->SetMesh(mRenderer->GetMesh("Assets/Cube.gpmesh"));
	
	// Spehere
	a = new Actor(this);
	a->SetPosition(Vector3(200.0f, -75.0f, 0.0f));
	a->SetScale(3.0f);
	mc = new MeshComponent(a);
	mc->SetMesh(mRenderer->GetMesh("Assets/Sphere.gpmesh"));

	// Setup lights
	mRenderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = mRenderer->GetDirectionalLight();
	dir.mDirection = Vector3(0.0f, -0.707f, -0.707f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

	// Camera actor
	mCameraActor = new CameraActor(this);

	// -- Map tiles--
	// Ground
	const float start = -1250.0f;
	const float size = 250.0f;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			a = new PlaneActor(this);
			a->SetPosition(Vector3(start + i * size, start + j * size, -100.0f));
		}
	}
	// Left/Right walls
	q = Quaternion(Vector3::UnitX, Math::PiOver2); // 90d rotate around X
	for (int i = 0; i < 10; i++)
	{
		a = new PlaneActor(this); // L
		a->SetPosition(Vector3(start + i * size, start - size, 0.0f));
		a->SetRotation(q);

		a = new PlaneActor(this); // R
		a->SetPosition(Vector3(start + i * size, start + size * 10, 0.0f));
		a->SetRotation(q);
	}

	// Forward/Back Walls
	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::PiOver2)); // 90d around X then Z
	for (int i = 0; i < 10; i++)
	{
		a = new PlaneActor(this); // F
		a->SetPosition(Vector3(-start+size, start + size * i, 0.0f));
		a->SetRotation(q);
	}

	// -- UI elements --
	// Hp bar
	a = new Actor(this);
	a->SetPosition(Vector3(-350.0f, -350.0f, 0.0f));
	SpriteComponent* sc = new SpriteComponent(a);
	sc->SetTexture(mRenderer->GetTexture("Assets/HealthBar.png"));
	
	// Radar
	a = new Actor(this);
	a->SetPosition(Vector3(380.0f, -270.0f, 0.0f));
	a->SetScale(0.75f);
	sc = new SpriteComponent(a);
	sc->SetTexture(mRenderer->GetTexture("Assets/Radar.png"));
}

void Game::UnloadData()
{
	// Delete actors
	// beacuse ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	if (mRenderer)
	{
		mRenderer->UnloadData();
	}
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