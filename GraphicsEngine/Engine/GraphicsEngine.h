#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
#include "Engine/Renderer/RenderSystem.h"
#include "Engine/Resources/TextureManager.h"

class GraphicsEngine
{
private:
	GraphicsEngine();	// Initialize GraphicsEngine and DirectX 11 Device
	~GraphicsEngine();	// Release all resources loaded

public:
	RenderSystem* GetRenderSystem();
	TextureManager* GetTextureManager();

	static GraphicsEngine* Get();
	static void Create();
	static void Release();

private:
	RenderSystem* m_RenderSystem = nullptr;
	TextureManager* m_TextureManager = nullptr;
	static GraphicsEngine* m_Engine;
};