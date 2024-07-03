#include "AppWindow.h"

#include <cmath>

#include "Engine/Graphics/DeviceContext.h"
#include "Engine/Math/Matrix4x4.h"
#include "Engine/Math/Vector3D.h"

struct vertex
{
	Vector3D position;
	Vector3D position1;
	Vector3D color;
	Vector3D color1;
};

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	unsigned int m_time;
};

AppWindow::AppWindow() {}

AppWindow::~AppWindow() {}

void AppWindow::UpdateQuadPosition()
{
	constant cc;
	Matrix4x4 temp;
	m_DeltaPos += m_DeltaTime / 6.0f;
	if (m_DeltaPos > 1.0f)
	{
		m_DeltaPos = 0;
	}
	m_DeltaScale += m_DeltaTime * 6.0f;

	cc.m_time = ::GetTickCount();

	cc.m_world.SetScale(Vector3D::Lerp(Vector3D(0.5, 0.5, 0), Vector3D(1, 1, 0), (sin(m_DeltaScale) + 1.0f)/2.0f));
	temp.SetTranslation(Vector3D::Lerp(Vector3D(-1.5,-1.5,0), Vector3D(1.5, 1.5, 0), m_DeltaPos));
	cc.m_world *= temp;
	cc.m_view.SetIdentity();
	cc.m_proj.SetOrthoLH
	(
		(this->GetClientWindowRect().right - this->GetClientWindowRect().left) / 400.0f,
		(this->GetClientWindowRect().bottom - this->GetClientWindowRect().top) / 400.0f,
		-4.0f,
		4.0f
	);

	m_ConstantBuffer->Update(GraphicsEngine::Get()->GetImmediateDeviceContext(), &cc);
}

void AppWindow::OnCreate()
{
	Window::OnCreate();
	GraphicsEngine::Get()->Init();
	m_SwapChain = GraphicsEngine::Get()->CreateSwapChain();

	RECT rc = this->GetClientWindowRect();
	m_SwapChain->Init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	vertex list[] = {
		{Vector3D(-0.5f, -0.5f, 0.0f),Vector3D(-0.32f,-0.11f,0.0f), Vector3D(1, 0, 0), Vector3D(0, 1, 1)},	// POS 1
		{Vector3D(-0.5f,  0.5f, 0.0f),Vector3D(-0.11f, 0.78f,0.0f), Vector3D(0, 1, 0), Vector3D(1, 0, 1)},	// POS 2
		{Vector3D(0.5f,  -0.5f, 0.0f),Vector3D( 0.75f,-0.73f,0.0f), Vector3D(0, 0, 1), Vector3D(1, 1, 0)},	// POS 3
		{Vector3D(0.5f,   0.5f, 0.0f),Vector3D( 0.88f, 0.5f, 0.0f), Vector3D(1, 1, 0), Vector3D(0, 0, 1)},	// POS 4
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

	constant cc;
	cc.m_time = 0;

	m_ConstantBuffer = GraphicsEngine::Get()->CreateConstantBuffer();
	m_ConstantBuffer->Load(&cc, sizeof(constant));
}

void AppWindow::OnUpdate()
{
	Window::OnUpdate();
	// Clear Render Target
	GraphicsEngine::Get()->GetImmediateDeviceContext()->ClearRenderTargetColor(this->m_SwapChain, 0.3f, 0.4f, 0.7f, 1.0f);

	// Set Viewport
	RECT rc = this->GetClientWindowRect();
	GraphicsEngine::Get()->GetImmediateDeviceContext()->SetViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	// Constant Buffer
	UpdateQuadPosition();

	GraphicsEngine::Get()->GetImmediateDeviceContext()->SetConstantBuffer(m_VertexShader, m_ConstantBuffer);
	GraphicsEngine::Get()->GetImmediateDeviceContext()->SetConstantBuffer(m_PixelShader, m_ConstantBuffer);

	// Set Shaders in the graphics pipeline
	GraphicsEngine::Get()->GetImmediateDeviceContext()->SetVertexShader(m_VertexShader);
	GraphicsEngine::Get()->GetImmediateDeviceContext()->SetVertexBuffer(m_VertexBuffer);
	GraphicsEngine::Get()->GetImmediateDeviceContext()->SetPixelShader(m_PixelShader);

	// Draw
	GraphicsEngine::Get()->GetImmediateDeviceContext()->DrawTriangleStrip(m_VertexBuffer->GetSizeVertexList(), 0);

	m_SwapChain->Present(true);

	// Calculate DeltaTime
	m_OldDelta = m_NewDelta;
	m_NewDelta = ::GetTickCount();
	m_DeltaTime = (m_OldDelta) ? (m_NewDelta - m_OldDelta) / 1000.0f : 0;
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
