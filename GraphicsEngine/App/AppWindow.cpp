#include "AppWindow.h"

#include <cmath>

#include "Engine/Graphics/DeviceContext.h"
#include "Engine/Math/Matrix4x4.h"
#include "Engine/Math/Vector3D.h"
#include "Engine/Input/InputSystem.h"

struct vertex
{
	Vector3D position;
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
	m_DeltaScale += m_DeltaTime * 1.0f;

	cc.m_time = ::GetTickCount();
	//cc.m_world.SetScale(Vector3D::Lerp(Vector3D(0.5, 0.5, 0), Vector3D(1, 1, 0), (sin(m_DeltaScale) + 1.0f)/2.0f));
	//temp.SetTranslation(Vector3D::Lerp(Vector3D(-1.5,-1.5,0), Vector3D(1.5, 1.5, 0), m_DeltaPos));
	//cc.m_world *= temp;
	cc.m_world.SetScale(Vector3D(m_Scale, m_Scale, m_Scale));

	temp.SetIdentity();
	temp.setRotationZ(0.0f);
	cc.m_world *= temp;

	temp.SetIdentity();
	temp.setRotationY(m_RotationY);
	cc.m_world *= temp;

	temp.SetIdentity();
	temp.setRotationX(m_RotationX);
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

	InputSystem::Get()->AddListener(this);

	GraphicsEngine::Get()->Init();
	m_SwapChain = GraphicsEngine::Get()->CreateSwapChain();

	RECT rc = this->GetClientWindowRect();
	m_SwapChain->Init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	vertex vertex_list[] = 
	{
		//FRONT FACE
		{Vector3D(-0.5f,-0.5f,-0.5f),		Vector3D(0,0,0),		Vector3D(0,1,0) },
		{Vector3D(-0.5f,0.5f,-0.5f),		Vector3D(1,1,0),		Vector3D(0,1,1) },
		{ Vector3D(0.5f,0.5f,-0.5f),		Vector3D(0,0,1),		Vector3D(1,0,0) },
		{ Vector3D(0.5f,-0.5f,-0.5f),		Vector3D(1,1,1),		Vector3D(0,0,1) },

		//BACK FACE
		{ Vector3D(0.5f,-0.5f,0.5f),		Vector3D(0,0,0),		Vector3D(0,1,0) },
		{ Vector3D(0.5f,0.5f,0.5f),		Vector3D(1,1,0),		Vector3D(0,1,1) },
		{ Vector3D(-0.5f,0.5f,0.5f),		Vector3D(0,0,1),		Vector3D(1,0,0) },
		{ Vector3D(-0.5f,-0.5f,0.5f),		Vector3D(1,1,1),		Vector3D(0,0,0) }
	};
	unsigned int index_list[] =
	{
		//FRONT SIDE
		0, 1, 2,
		2, 3, 0,
		//BACK SIDE
		4, 5, 6,
		6, 7, 4,
		//TOP SIDE
		1, 6, 5,
		5, 2, 1,
		//BOTTOM SIDE
		7, 0, 3,
		3, 4, 7,
		//RIGHT SIDE
		3, 2, 5,
		5, 4, 3,
		//LEFT SIDE
		7, 6, 1,
		1, 0, 7
	};
	UINT size_vertex_list = ARRAYSIZE(vertex_list);
	UINT size_index_list = ARRAYSIZE(index_list);

	m_VertexBuffer = GraphicsEngine::Get()->CreateVertexBuffer();
	m_IndexBuffer = GraphicsEngine::Get()->CreateIndexBuffer();


	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	// Vertex Shader
	GraphicsEngine::Get()->CompileVertexShader(L"Shaders/VertexShader.hlsl", "main", &shader_byte_code, &size_shader);
	m_VertexShader = GraphicsEngine::Get()->CreateVertexShader(shader_byte_code, size_shader);
	m_VertexBuffer->Load(vertex_list, sizeof(vertex), size_vertex_list, shader_byte_code, size_shader);
	m_IndexBuffer->Load(index_list, size_index_list);
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

	InputSystem::Get()->Update();

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
	GraphicsEngine::Get()->GetImmediateDeviceContext()->SetPixelShader(m_PixelShader);

	GraphicsEngine::Get()->GetImmediateDeviceContext()->SetVertexBuffer(m_VertexBuffer);
	GraphicsEngine::Get()->GetImmediateDeviceContext()->SetIndexBuffer(m_IndexBuffer);

	// Draw
	GraphicsEngine::Get()->GetImmediateDeviceContext()->DrawIndexTriangleList(m_IndexBuffer->GetSizeIndexList(), 0, 0);

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
	m_IndexBuffer->Release();
	m_ConstantBuffer->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
	m_SwapChain->Release();
	GraphicsEngine::Get()->Release();
}

void AppWindow::OnFocus()
{
	InputSystem::Get()->AddListener(this);
}

void AppWindow::OnKillFocus()
{
	InputSystem::Get()->RemoveListener(this);
}

void AppWindow::OnKeyDown(int key)
{
	float rotationSpeed = 2.5f;
	if (key == 'W')
	{
		m_RotationX += rotationSpeed * m_DeltaTime;
	}
	else if (key == 'S')
	{
		m_RotationX -= rotationSpeed * m_DeltaTime;
	}
	else if (key == 'A')
	{
		m_RotationY += rotationSpeed * m_DeltaTime;
	}
	else if (key == 'D')
	{
		m_RotationY -= rotationSpeed * m_DeltaTime;
	}

}

void AppWindow::OnKeyUp(int key)
{
	
}

void AppWindow::OnMouseMove(const Point& delta_mouse_pos)
{
	m_RotationX -= delta_mouse_pos.m_y * m_DeltaTime;
	m_RotationY -= delta_mouse_pos.m_x * m_DeltaTime;
}

void AppWindow::OnLeftMouseDown(const Point& mouse_pos)
{
	m_Scale = 0.5f;
}

void AppWindow::OnLeftMouseUp(const Point& mouse_pos)
{
	m_Scale = 1.0f;
}

void AppWindow::OnRightMouseDown(const Point& mouse_pos)
{
	m_Scale = 2.0f;
}

void AppWindow::OnRightMouseUp(const Point& mouse_pos)
{
	m_Scale = 1.0f;
}
