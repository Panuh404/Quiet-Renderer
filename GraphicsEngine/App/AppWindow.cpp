#include "AppWindow.h"

#include <cmath>

#include "Engine/Graphics/DeviceContext.h"
#include "Engine/Math/Matrix4x4.h"
#include "Engine/Math/Vector3D.h"
#include "Engine/Math/Vector2D.h"
#include "Engine/Input/InputSystem.h"

struct vertex
{
	Vector3D position;
	Vector2D texcoord;
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

void AppWindow::Update()
{
	constant cc;
	Matrix4x4 temp;
	cc.m_world.SetIdentity();
	cc.m_time = ::GetTickCount();

	Matrix4x4 world_cam;
	world_cam.SetIdentity();

	temp.SetIdentity();
	temp.setRotationX(m_RotationX);
	world_cam *= temp;

	temp.SetIdentity();
	temp.setRotationY(m_RotationY);
	world_cam *= temp;

	// Move Forward
	Vector3D new_pos = m_WorldCam.GetTranslation() + world_cam.GetZDirection() * (m_Forward * 5.0f * m_DeltaTime);
	// Move Right
	new_pos  = new_pos + world_cam.GetXDirection() * (m_Right * 5.0f * m_DeltaTime);

	world_cam.SetTranslation(new_pos);
	m_WorldCam = world_cam;
	world_cam.Inverse();

	cc.m_view = world_cam;
	/*cc.m_proj.SetOrthoLH
	(
		(this->GetClientWindowRect().right - this->GetClientWindowRect().left) / 400.0f,
		(this->GetClientWindowRect().bottom - this->GetClientWindowRect().top) / 400.0f,
		-4.0f,
		4.0f
	);*/

	int width = this->GetClientWindowRect().right - this->GetClientWindowRect().left;
	int height = this->GetClientWindowRect().bottom - this->GetClientWindowRect().top;

	cc.m_proj.SetPerspectiveFOVLH(1.57f, ((float)width / (float)height), 0.1f, 100.0f);

	m_ConstantBuffer->Update(GraphicsEngine::Get()->GetRenderSystem()->GetImmediateDeviceContext(), &cc);
}

void AppWindow::OnCreate()
{
	Window::OnCreate();

	InputSystem::Get()->AddListener(this);
	InputSystem::Get()->ShowCursor(false);

	m_WoodTex = GraphicsEngine::Get()->GetTextureManager()->CreateTextureFromFile(L"Textures/wood.jpg");

	RECT rc = this->GetClientWindowRect();
	m_SwapChain = GraphicsEngine::Get()->GetRenderSystem()->CreateSwapChain(this->m_HWND, rc.right - rc.left, rc.bottom - rc.top);

	m_WorldCam.SetTranslation(Vector3D(0, 0, -2));

	Vector3D position_list[] =
	{
		{ Vector3D(-0.5f,-0.5f,-0.5f)},
		{ Vector3D(-0.5f,0.5f,-0.5f) },
		{ Vector3D(0.5f, 0.5f,-0.5f) },
		{ Vector3D(0.5f,-0.5f,-0.5f)},

		{ Vector3D(0.5f,-0.5f,0.5f) },
		{ Vector3D(0.5f,0.5f,0.5f) },
		{ Vector3D(-0.5f,0.5f,0.5f)},
		{ Vector3D(-0.5f,-0.5f,0.5f) },
	};

	Vector2D texcoord_list[] =
	{
		{ Vector2D(0.0f,0.0f) },
		{ Vector2D(0.0f,1.0f) },
		{ Vector2D(1.0f,0.0f) },
		{ Vector2D(1.0f,1.0f) },
	};

	vertex vertex_list[] =
	{
		//X - Y - Z
		//FRONT FACE
		{ position_list[0],texcoord_list[1] },
		{ position_list[1],texcoord_list[0] },
		{ position_list[2],texcoord_list[2] },
		{ position_list[3],texcoord_list[3] },

		{ position_list[4],texcoord_list[1] },
		{ position_list[5],texcoord_list[0] },
		{ position_list[6],texcoord_list[2] },
		{ position_list[7],texcoord_list[3] },

		{ position_list[1],texcoord_list[1] },
		{ position_list[6],texcoord_list[0] },
		{ position_list[5],texcoord_list[2] },
		{ position_list[2],texcoord_list[3] },

		{ position_list[7],texcoord_list[1] },
		{ position_list[0],texcoord_list[0] },
		{ position_list[3],texcoord_list[2] },
		{ position_list[4],texcoord_list[3] },

		{ position_list[3],texcoord_list[1] },
		{ position_list[2],texcoord_list[0] },
		{ position_list[5],texcoord_list[2] },
		{ position_list[4],texcoord_list[3] },

		{ position_list[7],texcoord_list[1] },
		{ position_list[6],texcoord_list[0] },
		{ position_list[1],texcoord_list[2] },
		{ position_list[0],texcoord_list[3] },
	};

	unsigned int index_list[] =
	{
		//FRONT SIDE
		0,1,2,
		2,3,0,
		//BACK SIDE
		4,5,6,
		6,7,4,
		//TOP SIDE
		8,9,10,
		10,11,8,
		//BOTTOM SIDE
		12,13,14,
		14,15,12,
		//RIGHT SIDE
		16,17,18,
		18,19,16,
		//LEFT SIDE
		20,21,22,
		22,23,20
	};
	UINT size_vertex_list = ARRAYSIZE(vertex_list);
	UINT size_index_list = ARRAYSIZE(index_list);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	// Vertex Shader
	GraphicsEngine::Get()->GetRenderSystem()->CompileVertexShader(L"Shaders/VertexShader.hlsl", "main", &shader_byte_code, &size_shader);
	m_VertexShader = GraphicsEngine::Get()->GetRenderSystem()->CreateVertexShader(shader_byte_code, size_shader);
	m_VertexBuffer = GraphicsEngine::Get()->GetRenderSystem()->CreateVertexBuffer(vertex_list, sizeof(vertex), size_vertex_list, shader_byte_code, size_shader);
	m_IndexBuffer = GraphicsEngine::Get()->GetRenderSystem()->CreateIndexBuffer(index_list, size_index_list);
	GraphicsEngine::Get()->GetRenderSystem()->ReleaseCompiledShaders();

	// Pixel Shader
	GraphicsEngine::Get()->GetRenderSystem()->CompilePixelShader(L"Shaders/PixelShader.hlsl", "main", &shader_byte_code, &size_shader);
	m_PixelShader = GraphicsEngine::Get()->GetRenderSystem()->CreatePixelShader(shader_byte_code, size_shader);
	GraphicsEngine::Get()->GetRenderSystem()->ReleaseCompiledShaders();

	constant cc;
	cc.m_time = 0;

	m_ConstantBuffer = GraphicsEngine::Get()->GetRenderSystem()->CreateConstantBuffer(&cc, sizeof(constant));
}

void AppWindow::OnUpdate()
{
	Window::OnUpdate();

	InputSystem::Get()->Update();

	// Clear Render Target
	GraphicsEngine::Get()->GetRenderSystem()->GetImmediateDeviceContext()->ClearRenderTargetColor(this->m_SwapChain, 0.3f, 0.4f, 0.7f, 1.0f);

	// Set Viewport
	RECT rc = this->GetClientWindowRect();
	GraphicsEngine::Get()->GetRenderSystem()->GetImmediateDeviceContext()->SetViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	Update();

	GraphicsEngine::Get()->GetRenderSystem()->GetImmediateDeviceContext()->SetConstantBuffer(m_VertexShader, m_ConstantBuffer);
	GraphicsEngine::Get()->GetRenderSystem()->GetImmediateDeviceContext()->SetConstantBuffer(m_PixelShader, m_ConstantBuffer);

	GraphicsEngine::Get()->GetRenderSystem()->GetImmediateDeviceContext()->SetTexture(m_PixelShader, m_WoodTex);

	// Set Shaders in the graphics pipeline
	GraphicsEngine::Get()->GetRenderSystem()->GetImmediateDeviceContext()->SetVertexShader(m_VertexShader);
	GraphicsEngine::Get()->GetRenderSystem()->GetImmediateDeviceContext()->SetPixelShader(m_PixelShader);

	GraphicsEngine::Get()->GetRenderSystem()->GetImmediateDeviceContext()->SetVertexBuffer(m_VertexBuffer);
	GraphicsEngine::Get()->GetRenderSystem()->GetImmediateDeviceContext()->SetIndexBuffer(m_IndexBuffer);

	// Draw
	GraphicsEngine::Get()->GetRenderSystem()->GetImmediateDeviceContext()->DrawIndexTriangleList(m_IndexBuffer->GetSizeIndexList(), 0, 0);

	m_SwapChain->Present(true);

	// Calculate DeltaTime
	m_OldDelta = m_NewDelta;
	m_NewDelta = ::GetTickCount();
	m_DeltaTime = (m_OldDelta) ? (m_NewDelta - m_OldDelta) / 1000.0f : 0;
}

void AppWindow::OnDestroy()
{
	Window::OnDestroy();
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
		m_Forward = 1.0f;
	}
	else if (key == 'S')
	{
		m_Forward = -1.0f;
	}
	else if (key == 'D')
	{
		m_Right = 1.0f;
	}
	else if (key == 'A')
	{
		m_Right = -1.0f;
	}

}

void AppWindow::OnKeyUp(int key)
{
	m_Forward = 0.0f;
	m_Right = 0.0f;
}

void AppWindow::OnMouseMove(const Point& mouse_pos)
{
	int width = this->GetClientWindowRect().right - this->GetClientWindowRect().left;
	int height = this->GetClientWindowRect().bottom - this->GetClientWindowRect().top;

	m_RotationX += (mouse_pos.m_y - (height / 2.0f)) * m_DeltaTime * 0.4f;
	m_RotationY += (mouse_pos.m_x - (width / 2.0f)) * m_DeltaTime * 0.4f;

	InputSystem::Get()->SetCursorPosition(Point(width / 2.0f, height / 2.0f));
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
