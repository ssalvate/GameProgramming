#include "Asteroid.h"
#include "../Random.h"
#include "../Components/SpriteComponent.h"
#include "../Game.h"
#include "../Components/MoveComponent.h"
#include "../Components/CircleComponent.h"

Asteroid::Asteroid(Game* game)
	:Actor(game)
	,mCircle(nullptr)
{
	// Initialize random position/orientation
	Vector2 randPos = Random::GetVector(Vector2(-512.0f, -384.0f),
		Vector2(512.0f, 384.0f));;

	SetPosition(randPos);
	SetRotation(Random::GetFloatRange(0.0f, Math::TwoPi));

	// Create a sprite component, and set texture
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Asteroid.png"));

	//Create a move component, and set a forward speed
	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(150.0f);
	Vector2 randVel = Random::GetVector(Vector2(-50.0f, -50.0f), Vector2(50.0f, 50.0f));
	mc->SetVelocity(randVel);


	mCircle = new CircleComponent(this);
	mCircle->SetRadius(40.0f);

	// Add to mAsteroids in game
	game->AddAsteroid(this);
}

Asteroid::~Asteroid()
{
	GetGame()->RemoveAsteroid(this);
}