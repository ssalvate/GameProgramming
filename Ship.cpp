#include "Ship.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "InputComponent.h"
#include "Laser.h"
#include "CircleComponent.h"
#include "Asteroid.h"

Ship::Ship(Game* game)
	: Actor(game)
	,mLaserCooldown(0.0f)
	,mRespawnCooldown(0.0f)
	,mSc(nullptr)
{
	// Create sprite component
	mSc = new SpriteComponent(this, 150);
	mSc->SetTexture(game->GetTexture("Assets/Ship.png"));

	// Create input component and set keys/speed
	InputComponent* ic = new InputComponent(this);
	ic->SetForwardKey(SDL_SCANCODE_W);
	ic->SetBackKey(SDL_SCANCODE_S);
	ic->SetClockwiseKey(SDL_SCANCODE_A);
	ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
	ic->SetMaxForwardSpeed(300.0f);
	ic->SetMaxAngularSpeed(Math::TwoPi);

	// Create circle component for collision
	mCc = new CircleComponent(this);
	mCc->SetRadius(32.0f);
}

void Ship::UpdateActor(float deltaTime)
{
	mLaserCooldown -= deltaTime;
	mRespawnCooldown -= deltaTime;

	if (mRespawning && mRespawnCooldown < 0)
	{
		printf("Respawn");
		SetState(EActive);
		SetPosition(Vector2(512.0f, 384.0f));
		SetRotation(0.0f);
		mRespawning = false;
	}
	else if (!mRespawning && mRespawnCooldown < 0)
	{
		// Does ship intersect with an asteroid?
		for (auto ast : GetGame()->GetAsteroids())
		{
			if (Intersect(*mCc, *(ast->GetCircle())))
			{
				// If this ship intesects with an asteroid
				// set this(ourself) and asteroid to dead
				mSc->SetTexture(GetGame()->GetTexture("Assets/Ship.png"));
				SetState(EPaused);
				mRespawnCooldown= 2.0f;
				mRespawning = true;
				ast->SetState(EDead);
				break;
			}
		}
	}
}

void Ship::ActorInput(const uint8_t* keyState)
{
	if (keyState[SDL_SCANCODE_W])
	{
		mSc->SetTexture(GetGame()->GetTexture("Assets/ShipWithThrust.png"));
	}
	else
	{
		mSc->SetTexture(GetGame()->GetTexture("Assets/Ship.png"));
	}
	if (keyState[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f)
	{
		// Create a laser and set its position/rotation to this
		Laser* laser = new Laser(GetGame());
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());

		// Reset laser cooldown (half second)
		mLaserCooldown = 0.5f;
	}
}

