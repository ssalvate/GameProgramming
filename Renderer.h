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

	void AddMeshComp(class MeshComponent* mesh);
	void RemoveMeshComp(class MeshComponent* mesh);

	class Texture* GetTexture(const std::string& fileName);
	class Mesh* GetMesh(const std::string& fileName);
	
	void SetViewMatrix(const Matrix4& view) { mView = view; }

	float GetScreenWidth() const { return mScreenWidth; }
	float GetScreenHeight() const { return mScreenHeight; }
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
	std::vector<class MeshComponent*> mMeshComps;

	// Game
	class Game* mGame;

	// Sprite shader
	class Shader* mSpriteShader;
	// Sprite vertex array
	class VertexArray* mSpriteVerts;

	// Mesh shader
	class Shader* mMeshShader;

	// View/projection for 3D shaders
	Matrix4 mView;
	Matrix4 mProjection;
	// Width/height of screen
	float mScreenWidth;
	float mScreenHeight;

	// Lighting
	

	// Window
	SDL_Window* mWindow;
	// OpenGL context
	SDL_GLContext mContext;
};
