#include "Renderer.h"
#include "glew.h"
#include "Texture.h"
#include "Mesh.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Components/MeshComponent.h"
#include "Components/SpriteComponent.h"

Renderer::Renderer(Game* game)
	: mGame(game)
	, mSpriteShader(nullptr)
	, mMeshShader(nullptr)
{
}

Renderer::~Renderer()
{
}

bool Renderer::Initialize(float screenWidth, float screenHeight)
{
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	// Set OpenGL attributes
	// Use the core OpenGL profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// Specify version 3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// Request a color buffer with 8-bits per RGBA channel
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// Enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// Force OpenGL to use hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// Create OpenGL window
	mWindow = SDL_CreateWindow(
		"Game Programming in C++", // Window title
		100,	// Top left x-coordinate of window
		100,	// Top left y-coordinate of window
		static_cast<int>(mScreenWidth),	// Width of window
		static_cast<int>(mScreenHeight),// Height of window
		SDL_WINDOW_OPENGL
	);
	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// Create an OpenGL context
	mContext = SDL_GL_CreateContext(mWindow);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}

	// On some platforms, GLEW will emit a benign error code,
	// so clear it
	glGetError();

	// Make sure we can create / compile shaders
	if (!LoadShaders())
	{
		SDL_Log("Failed to load shaders");
		return false;
	}

	// Create quad for drawing sprites onto
	CreateSpriteVerts();

	return true;
}

void Renderer::Shutdown()
{
	delete mSpriteVerts;
	mSpriteShader->Unload();
	delete mSpriteShader;
	mMeshShader->Unload();
	delete mMeshShader;
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
}

void Renderer::UnloadData()
{
	// Destroy textures
	for (auto i : mTextures)
	{
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();

	// Destroy meshes
	for (auto i : mMeshes)
	{
		i.second->Unload();
		delete i.second;
	}
	mMeshes.clear();
}

void Renderer::Draw()
{
	// 1.Set clear color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Clear color buffer + depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 2.Draw the scene
	
	//	a.Draw mesh components
	// Enable depth buffering/disable alpha blend
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	// Set the mesh shader active
	mMeshShader->SetActive();
	// Update the view-proj matrix
	mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);
	// Update lighting uniforms
	SetLightUniforms(mMeshShader);
	for (auto mc : mMeshComps)
	{
		mc->Draw(mMeshShader);
	}

	//	b.Draw all sprite components
	// Disable depth buffering
	glDisable(GL_DEPTH_TEST);
	//Enable alpha blending on the color buffer
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	// Set sprite shader and vertex array objects to active
	mSpriteShader->SetActive();
	mSpriteVerts->SetActive();
	for (auto sprite : mSprites)
	{
		sprite->Draw(mSpriteShader);
	}

	// 3.Swap front and back buffers, which also displays the scene
	SDL_GL_SwapWindow(mWindow);
}

void Renderer::AddSprite(SpriteComponent* sprite)
{
	// Find insert point in the sorted vector
	// (the first elem with a higher draw order than me)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (; iter != mSprites.begin(); ++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder()) break;

	}
	// Insert elem before position of iterator
	mSprites.insert(iter, sprite);
}

void Renderer::RemoveSprite(SpriteComponent* sprite)
{
	// We can't swap because it breaks ordering
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

void Renderer::AddMeshComp(MeshComponent* mesh)
{
	mMeshComps.emplace_back(mesh);
}

void Renderer::RemoveMeshComp(MeshComponent* mesh)
{
	auto iter = std::find(mMeshComps.begin(), mMeshComps.end(), mesh);
	mMeshComps.erase(iter);
}

Texture* Renderer::GetTexture(const std::string& fileName)
{
	Texture* tex = nullptr;
	// Is the texture already loaded?
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		// Create new texture 
		tex = new Texture();
		if (tex->Load(fileName))
		{
			mTextures.emplace(fileName, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}

	}
	return tex;
}

Mesh* Renderer::GetMesh(const std::string& fileName)
{
	Mesh* m = nullptr;
	auto iter = mMeshes.find(fileName);
	if (iter != mMeshes.end())
	{
		m = iter->second;
	}
	else
	{
		m = new Mesh();
		if (m->Load(fileName, this))
		{
			mMeshes.emplace(fileName, m);
		}
		else
		{
			delete m;
			m = nullptr;
		}
	}
	return m;
}

bool Renderer::LoadShaders()
{
	// Create sprite shader
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
	{
		return false;
	}
	mSpriteShader->SetActive();
	// Set the 'simple' view-projection matrix
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(mScreenWidth,mScreenHeight);
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);
	
	// Create basic mesh shader
	mMeshShader = new Shader();
	if (!mMeshShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag"))
	{
		return false;
	}

	mMeshShader->SetActive();
	// Set the view-projection matrix
	mView = Matrix4::CreateLookAt(
		Vector3::Zero,	// Camera Position
		Vector3::UnitX, // Target Position
		Vector3::UnitZ	// Up
	);
	mProjection = Matrix4::CreatePerspectiveFOV(
		Math::ToRadians(70.0f), // Horizontal FOV
		mScreenWidth,			// Width of view
		mScreenHeight,			// Height of view
		25.0f,					// Near plane distance
		1000.0f					// Far plane distance
	);
	mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);
	return true;
}

// Create rect polygon to place sprites on (a unit square)
void Renderer::CreateSpriteVerts()
{
	float vertexBuffer[] = {
		-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // top left
		 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom left
	};

	unsigned int indexBuffer[] = {
		0,1,2,
		2,3,0
	};

	mSpriteVerts = new VertexArray(vertexBuffer, 4, indexBuffer, 6);
}

void Renderer::SetLightUniforms(Shader* shader)
{
	// Camera position is from inverted view
	Matrix4 invView = mView;
	invView.Invert();
	shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
	// Ambient light
	shader->SetVectorUniform("uAmbientLight", mAmbientLight);
	// Directional light
	shader->SetVectorUniform("uDirLight.mDirection", mDirLight.mDirection);
	shader->SetVectorUniform("uDirLight.mDiffuseColor",mDirLight.mDiffuseColor);
	shader->SetVectorUniform("uDirLight.mSpecColor", mDirLight.mSpecColor);
}
