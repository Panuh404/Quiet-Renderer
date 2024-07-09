#pragma once
#include "Engine/Window/Window.h"
#include "Engine/Graphics/GraphicsEngine.h"
#include "Engine/Graphics/SwapChain.h"
#include "Engine/Graphics/VertexBuffer.h"
#include "Engine/Graphics/IndexBuffer.h"
#include "Engine/Graphics/ConstantBuffer.h"
#include "Engine/Graphics/VertexShader.h"
#include "Engine/Graphics/PixelShader.h"
#include "Engine/Input/InputListener.h"

class  AppWindow : public Window, public InputListener
{
public:
	AppWindow();
	~AppWindow();

	void UpdateQuadPosition();

	// Inherited via window
	void OnCreate() override;
	void OnUpdate() override;
	void OnDestroy() override;

	void OnFocus() override;
	void OnKillFocus() override;

	// Inherited via InputListener
	void OnKeyDown(int key) override;
	void OnKeyUp(int key) override;
	void OnMouseMove(const Point& delta_mouse_pos) override;
	void OnLeftMouseDown(const Point& mouse_pos) override;
	void OnLeftMouseUp(const Point& mouse_pos) override;
	void OnRightMouseDown(const Point& mouse_pos) override;
	void OnRightMouseUp(const Point& mouse_pos) override;

private:
	SwapChain*		m_SwapChain;
	VertexBuffer*	m_VertexBuffer;
	IndexBuffer*	m_IndexBuffer;
	ConstantBuffer* m_ConstantBuffer;
	VertexShader*	m_VertexShader;
	PixelShader*	m_PixelShader;

	float m_OldDelta;
	float m_NewDelta;
	float m_DeltaTime;

	float m_DeltaPos;
	float m_DeltaScale;

	float m_RotationX = 0.0f;
	float m_RotationY = 0.0f;

	float m_Scale = 1.0f;
};

