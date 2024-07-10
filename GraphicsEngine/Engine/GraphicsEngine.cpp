#include "GraphicsEngine.h"
#include "Engine/Renderer/RenderSystem.h"

GraphicsEngine* GraphicsEngine::m_Engine = nullptr;

GraphicsEngine::GraphicsEngine()
{
	try
	{
		m_RenderSystem = new RenderSystem();
	}
	catch (...)
	{
		throw std::exception("ERROR::GraphicsEngine not created successfully");
	}
}

GraphicsEngine::~GraphicsEngine()
{
	m_Engine = nullptr;
	delete m_RenderSystem;
}

RenderSystem* GraphicsEngine::GetRenderSystem()
{
	return m_RenderSystem;
}

void GraphicsEngine::Create()
{
	if (m_Engine) throw std::exception("ERROR::GraphicsEngine already created");
	m_Engine = new GraphicsEngine();
}

void GraphicsEngine::Release()
{
	if (!m_Engine) return;
	delete m_Engine;
}

GraphicsEngine* GraphicsEngine::Get()
{
	return m_Engine;
}

