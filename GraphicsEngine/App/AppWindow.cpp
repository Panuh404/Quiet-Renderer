#include "AppWindow.h"
#include "Engine/Graphics/DeviceContext.h"

struct VEC3
{
	float x, y, z;
};

struct vertex
{
	VEC3 position;
	VEC3 color;
};

AppWindow::AppWindow() {}

AppWindow::~AppWindow() {}

void AppWindow::OnCreate()
{
	Window::OnCreate();
	GraphicsEngine::Get()->Init();
	m_SwapChain = GraphicsEngine::Get()->CreateSwapChain();

	RECT rc = this->GetClientWindowRect();
	m_SwapChain->Init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	vertex list[] = {
	{-0.5f, -0.5f, 0.0f, 1, 0, 0},	// POS 1
	{-0.5f,  0.5f, 0.0f, 0, 1, 0},	// POS 2
	{ 0.5f, -0.5f, 0.0f, 0, 0, 1},	// POS 3
	{ 0.5f,  0.5f, 0.0f, 1, 1, 0},	// POS 4
	};

	m_VertexBuffer = GraphicsEngine::Get()->CreateVertexBuffer();
	UINT size_list = ARRAYSIZE(list);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	// Vertex Shader
	GraphicsEngine::Get()->CompileVertexShader(L"Shaders/VertexShader.hlsl", "main", &shader_byte_code, &size_shader);
	m_VertexShader = GraphicsEngine::Get()->CreateVertexShader(shader_byte_code, size_shader);
	m_VertexBuffer->Load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);
	GraphicsEngine::Get()->ReleaseCompiledShaders();

	// Pixel Shader
	GraphicsEngine::Get()->CompilePixelShader(L"Shaders/PixelShader.hlsl", "main", &shader_byte_code, &size_shader);
	m_PixelShader = GraphicsEngine::Get()->CreatePixelShader(shader_byte_code, size_shader);
	GraphicsEngine::Get()->ReleaseCompiledShaders();
}

void AppWindow::OnUpdate()
{
	Window::OnUpdate();
	// Clear Render Target
	GraphicsEngine::Get()->GetImmediateDeviceContext()->ClearRenderTargetColor(this->m_SwapChain, 0.3f, 0.4f, 0.7f, 1.0f);

	// Set Viewport
	RECT rc = this->GetClientWindowRect();
	GraphicsEngine::Get()->GetImmediateDeviceContext()->SetViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	// Set Shaders in the graphics pipeline
	GraphicsEngine::Get()->GetImmediateDeviceContext()->SetVertexShader(m_VertexShader);
	GraphicsEngine::Get()->GetImmediateDeviceContext()->SetVertexBuffer(m_VertexBuffer);
	GraphicsEngine::Get()->GetImmediateDeviceContext()->SetPixelShader(m_PixelShader);

	// Draw
	GraphicsEngine::Get()->GetImmediateDeviceContext()->DrawTriangleStrip(m_VertexBuffer->GetSizeVertexList(), 0);

	m_SwapChain->Present(true);
}

void AppWindow::OnDestroy()
{
	Window::OnDestroy();
	m_VertexBuffer->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
	m_SwapChain->Release();
	GraphicsEngine::Get()->Release();
}
