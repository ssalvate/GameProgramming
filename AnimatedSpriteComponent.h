#pragma once

#include <vector>

#include "SpriteComponent.h"

class AnimatedSpriteComponent : public SpriteComponent
{
public:
	AnimatedSpriteComponent(class Actor* owner, int drawOrder = 100);
	// Update animation ever frame (overriden from component)
	void Update(float deltaTime) override;
	// Set the Textures used for animation
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
	// Set/get the animation FPS
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }
private:
	// All textures in the animation
	std::vector<SDL_Texture*> mAnimTextures;
	// Curent frame displayed
	float mCurrFrame;
	// Animation frame rate
	float mAnimFPS;
};