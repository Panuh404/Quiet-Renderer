#pragma once
#include "Engine/Window/Window.h"
#include "Engine/GraphicsEngine.h"
#include "Engine/Graphics/SwapChain.h"
#include "Engine/Graphics/VertexBuffer.h"
#include "Engine/Graphics/IndexBuffer.h"
#include "Engine/Graphics/ConstantBuffer.h"
#include "Engine/Graphics/VertexShader.h"
#include "Engine/Graphics/PixelShader.h"
#include "Engine/Input/InputListener.h"
#include "Engine/Math/Matrix4x4.h"

class  AppWindow : public Window, public InputListener
{
public:
	AppWindow();
	~AppWindow();

	void Update();

	// Inherited via window
	void OnCreate() override;
	void OnUpdate() override;
	void OnDestroy() override;

	void OnFocus() override;
	void OnKillFocus() override;

	// Inherited via InputListener
	void OnKeyDown(int key) override;
	void OnKeyUp(int key) override;
	void OnMouseMove(const Point& mouse_pos) override;
	void OnLeftMouseDown(const Point& mouse_pos) override;
	void OnLeftMouseUp(const Point& mouse_pos) override;
	void OnRightMouseDown(const Point& mouse_pos) override;
	void OnRightMouseUp(const Point& mouse_pos) override;

private:
	SwapChainPtr		m_SwapChain;
	VertexBufferPtr		m_VertexBuffer;
	IndexBufferPtr		m_IndexBuffer;
	ConstantBufferPtr	m_ConstantBuffer;
	VertexShaderPtr		m_VertexShader;
	PixelShaderPtr		m_PixelShader;

	float m_OldDelta;
	float m_NewDelta;
	float m_DeltaTime;

	float m_RotationX = 0.0f;
	float m_RotationY = 0.0f;
	float m_Scale = 1.0f;

	float m_Forward = 0.0f;
	float m_Right = 0.0f;
	Matrix4x4 m_WorldCam;
};

