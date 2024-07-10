#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
#include "Engine/Renderer/RenderSystem.h"


class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();

	bool Init();		// Initialize GraphicsEngine and DirectX 11 Device
	bool Release();		// Release all resources loaded

	RenderSystem* GetRenderSystem();
	static GraphicsEngine* Get();

private:
	RenderSystem* m_RenderSystem = nullptr;
};