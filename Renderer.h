#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "Math.h"
#include "SDL.h"

class Renderer
{
public:
	Renderer(class Game* game);
	~Renderer();
	// initialize and shutdown renderer
	bool Initialize(float screenWidth, float screenHeight);
	void Shutdown();
	// Unload all textures/meshes
	void UnloadData();
	// Draw the frame
	void Draw();

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);
	class Texture* GetTexture(const std::string& fileName);
	class Mesh* GetMesh(const std::string& fileName);
private:
	bool LoadShaders();
	void CreateSpriteVerts();

	// Map of textures loaded
	std::unordered_map<std::string, class Texture*> mTextures;
	// Map of meshes loaded
	std::unordered_map<std::string, class Mesh*> mMeshes;

	// All the sprite components drawn
	std::vector<class SpriteComponent*> mSprites;

	// All mesh components drawn
	

	// Game
	class Game* mGame;

	// Sprite shader
	class Shader* mSpriteShader;
	// Sprite vertex array
	class VertexArray* mSpriteVerts;

	// Mesh shader
	class Shader* mMeshShader;

	// View/projection for 3D shaders
	

	// Width/height of screen
	float mScreenWidth;
	float mScreenHeight;

	// Lighting
	

	// Window
	SDL_Window* mWindow;
	// OpenGL context
	SDL_GLContext mContext;
};

