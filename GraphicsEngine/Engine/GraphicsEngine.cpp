#include "GraphicsEngine.h"
#include "Engine/Renderer/RenderSystem.h"

GraphicsEngine* GraphicsEngine::m_Engine = nullptr;

GraphicsEngine::GraphicsEngine()
{
	// Create Render System
	try
	{
		m_RenderSystem = new RenderSystem();
	}
	catch (...)
	{
		throw std::exception("ERROR::GraphicsEngine not created successfully");
	}

	// Create Texture Manager
	try
	{
		m_TextureManager = new TextureManager();
	}
	catch (...)
	{
		throw std::exception("ERROR::Texture Manager not created successfully");
	}
}

GraphicsEngine::~GraphicsEngine()
{
	m_Engine = nullptr;
	delete m_TextureManager;
	delete m_RenderSystem;
}

RenderSystem* GraphicsEngine::GetRenderSystem()
{
	return m_RenderSystem;
}

TextureManager* GraphicsEngine::GetTextureManager()
{
	return m_TextureManager;
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

