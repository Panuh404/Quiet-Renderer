#include "GraphicsEngine.h"
#include "Engine/Renderer/RenderSystem.h"

GraphicsEngine::GraphicsEngine() {}

GraphicsEngine::~GraphicsEngine() {}

bool GraphicsEngine::Init()
{
	m_RenderSystem = new RenderSystem();
	m_RenderSystem->Init();
	return true;
}

bool GraphicsEngine::Release()
{
	m_RenderSystem->Release();
	delete m_RenderSystem;
	return true;
}

RenderSystem* GraphicsEngine::GetRenderSystem()
{
	return m_RenderSystem;
}

GraphicsEngine* GraphicsEngine::Get()
{
	static GraphicsEngine engine;
	return &engine;
}
