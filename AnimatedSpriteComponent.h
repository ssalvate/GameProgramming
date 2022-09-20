#pragma once

#include <vector>
#include <string>
#include <array>
#include <unordered_map>

#include "SpriteComponent.h"

class AnimatedSpriteComponent : public SpriteComponent
{
public:
	AnimatedSpriteComponent(class Actor* owner, int drawOrder = 100);
	// Update animation ever frame (overriden from component)
	void Update(float deltaTime) override;
	// Set the Textures used for all animations
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
	// Set names of animations
	void SetAnimNames(const std::string& name,const std::array<int,3> &index);
	void SetCurrentAnimation(const std::string& name);

	// Set/get the animation FPS
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }
private:
	// All textures in the animation
	std::vector<SDL_Texture*> mAnimTextures;
	// Map of animations <string, array[start index, end index, looping(0/1)] >
	std::unordered_map< std::string, std::array<int,3> > mAnims;
	// Curent frame displayed
	float mCurrFrame;
	// Animation frame rate
	float mAnimFPS;
	// Current animation to play
	std::string mCurrAnim;
};