#include "AnimatedSpriteComponent.h"
#include "Math.h"

AnimatedSpriteComponent::AnimatedSpriteComponent(Actor* owner, int drawOrder)
	: SpriteComponent(owner, drawOrder)
	, mCurrFrame(0.0f)
	, mAnimFPS(24.0f)
{
}

void AnimatedSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);
	
	// Grab current animation, [start, end, loop]
	std::array<int,3> index = mAnims[mCurrAnim];


	if (mAnimTextures.size() > 0)
	{
		// Update the current frame based on frame rate and deltaTime
		mCurrFrame += mAnimFPS * deltaTime;
			
		// Wrap current frame if needed
		while (mCurrFrame > index[1])
		{	
			if (index[2])mCurrFrame -= index[1] - index[0]; // Looping
			else mCurrFrame = index[1];
		}

	}
	// Set the current texture
	SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
}

void AnimatedSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures)
{
	mAnimTextures = textures;
	if (mAnimTextures.size() > 0)
	{
		// Set the actice texture to the first frame
		mCurrFrame = 0.0f;
		SetTexture(mAnimTextures[0]);
	}
}

void AnimatedSpriteComponent::SetAnimNames(const std::string& name, const std::array<int, 3>& index)
{
	mAnims.insert({ name, index });
	mCurrAnim = name;
	mCurrFrame = index[0];
}

void AnimatedSpriteComponent::SetCurrentAnimation(const std::string& name)
{
	mCurrAnim = name;
	mCurrFrame = mAnims[name][0];
}
