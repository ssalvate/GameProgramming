#include "Asteroid.h"
#include "Random.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "MoveComponent.h"

Asteroid::Asteroid(Game* game)
	:Actor(game)
{
	// Initialize random position/orientation
	Vector2 randPos = Random::GetVector(Vector2::Zero, Vector2(1024.0f, 768.0f));

	SetPosition(randPos);
	SetRotation(Random::GetFloatRange(0.0f, Math::TwoPi));

	// Create a sprite component, and set texture
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Asteroid.png"));

	//Create a move component, and set a forward speed
	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(150.0f);
}
