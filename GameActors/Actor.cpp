#include <algorithm>

#include "Actor.h"
#include "../Game.h"
#include "../Components/Component.h"

Actor::Actor(Game* game)
	: mState(EActive)
	, mPosition(Vector3::Zero)
	, mScale(1.0f)
	, mRotation(Quaternion::Identity)
	, mGame(game)
	, mRecomputeWorldTransform(true)
{
	mGame->AddActor(this);
}

Actor::~Actor()
{
	mGame->RemoveActor(this);
	// Need to delete components
	// ~Component calls Actor::RemoveComponent, need a different style loop
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}
}

void Actor::Update(float deltaTime)
{
	if (mState == EActive)
	{
		ComputeWorldTransform();

		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
		// Update transform twice in case it changes in the interim
		ComputeWorldTransform();
	}
}

void Actor::UpdateComponents(float deltaTime)
{
	for (auto comp : mComponents)
	{
		comp->Update(deltaTime);
	}
}

void Actor::UpdateActor(float deltaTime)
{
}

void Actor::ProcessInput(const uint8_t* keyState)
{
	if (mState == EActive)
	{
		for (auto comp : mComponents)
		{
			comp->ProcessInput(keyState);
		}
		ActorInput(keyState);
	}
}

void Actor::ActorInput(const uint8_t* keyState)
{
}

void Actor::ComputeWorldTransform()
{
	if (mRecomputeWorldTransform)
	{
		mRecomputeWorldTransform = false;
		// Scale, then rotate, then translate
		mWorldTransform  = Matrix4::CreateScale(mScale);
		mWorldTransform *= Matrix4::CreateFromQuaternion(mRotation);
		mWorldTransform *= Matrix4::CreateTranslation(mPosition);

		// Inform componentss world transform updated
		for (auto comp : mComponents)
		{
			comp->OnUpdateWorldTransform();
		}
	}
}


void Actor::AddComponent(Component* component)
{
	// Find the insertion point in the sorted vector
	// (The first element with a order higher than me)
	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (; iter != mComponents.end(); ++iter)
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}
	// Inserts element before position of iterator
	mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component* component)
{
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}