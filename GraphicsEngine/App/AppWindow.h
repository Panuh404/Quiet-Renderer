#pragma once
#include "Engine/Window/Window.h"
#include "Engine/Graphics/GraphicsEngine.h"
#include "Engine/Graphics/SwapChain.h"
#include "Engine/Graphics/VertexBuffer.h"
#include "Engine/Graphics/IndexBuffer.h"
#include "Engine/Graphics/ConstantBuffer.h"
#include "Engine/Graphics/VertexShader.h"
#include "Engine/Graphics/PixelShader.h"

class AppWindow : public Window
{
public:
	AppWindow();
	~AppWindow();

	void UpdateQuadPosition();

	// Inherited via window
	void OnCreate() override;
	void OnUpdate() override;
	void OnDestroy() override;

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
};

