#include "Laser.h"
#include "SpriteComponent.h"
#include "CircleComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Asteroid.h"

Laser::Laser(Game* game)
	:Actor(game)
	,mDeathTimer(1.0f)
{
	// Create sprite component
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Laser.png"));

	// Create new move component + set forward speed
	mMc = new MoveComponent(this);
	mMc->SetForwardSpeed(800.0f);

	// Create circle component for collision
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(11.0f);

}

void Laser::UpdateActor(float deltaTime)
{
	// If we run out of time, laser is dead
	mDeathTimer -= deltaTime;
	if (mDeathTimer <= 0.0f)
	{
		SetState(EDead);
	}
	else
	{
		// Does laser intersect with an asteroid
		for (auto ast : GetGame()->GetAsteroids())
		{
			if (Intersect(*mCircle, *(ast->GetCircle())))
			{
				// If this laser intersects with an asteroid
				// set this(ourself) and asteroid to dead
				SetState(EDead);
				ast->SetState(EDead);
				break;
			}
		}
	}

}
