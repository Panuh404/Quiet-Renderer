#include "AppWindow.h"
#include "Engine/Graphics/DeviceContext.h"

struct VEC3
{
	float x, y, z;
};

struct vertex
{
	VEC3 position;
};

AppWindow::AppWindow()
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::OnCreate()
{
	Window::OnCreate();
	GraphicsEngine::Get()->Init();
	m_SwapChain = GraphicsEngine::Get()->CreateSwapChain();

	RECT rc = this->GetClientWindowRect();
	m_SwapChain->Init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	vertex list[] = {
		{-0.5f, -0.5f, 0.0f},	// Vert 1
		{ 0.0f,  0.5f, 0.0f},	// Vert 2
		{ 0.5f, -0.5f, 0.0f}	// Vert 3
	};

	m_VertexBuffer = GraphicsEngine::Get()->CreateVertexBuffer();
	UINT size_list = ARRAYSIZE(list);

	// TEMP
	GraphicsEngine::Get()->CreateShaders();

	void* shader_byte_code = nullptr;
	UINT size_shader = 0;
	GraphicsEngine::Get()->GetShaderBufferAndSize(&shader_byte_code, &size_shader);

	m_VertexBuffer->Load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);
}

void AppWindow::OnUpdate()
{
	Window::OnUpdate();
	GraphicsEngine::Get()->GetImmediateDeviceContext()->ClearRenderTargetColor(this->m_SwapChain, 0.3f, 0.4f, 0.7f, 1.0f);

	RECT rc = this->GetClientWindowRect();
	GraphicsEngine::Get()->GetImmediateDeviceContext()->SetViewportSize(rc.right - rc.left, rc.bottom - rc.top);
	GraphicsEngine::Get()->SetShaders();

	GraphicsEngine::Get()->GetImmediateDeviceContext()->SetVertexBuffer(m_VertexBuffer);
	GraphicsEngine::Get()->GetImmediateDeviceContext()->DrawTriangleList(m_VertexBuffer->GetSizeVertexList(), 0);


	m_SwapChain->Present(true);
}

void AppWindow::OnDestroy()
{
	Window::OnDestroy();
	m_VertexBuffer->Release();
	m_SwapChain->Release();
	GraphicsEngine::Get()->Release();
}
