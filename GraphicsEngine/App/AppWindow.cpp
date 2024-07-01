#include "AppWindow.h"

#include "Engine/Graphics/DeviceContext.h"

AppWindow::AppWindow()
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::OnCreate()
{
	Window::OnCreate();
	Graphics::Get()->Init();
	m_SwapChain = Graphics::Get()->CreateSwapChain();

	RECT rc = this->GetClientWindowRect();
	m_SwapChain->Init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);
}

void AppWindow::OnUpdate()
{
	Window::OnUpdate();
	Graphics::Get()->GetImmediateDeviceContext()->ClearRenderTargetColor(this->m_SwapChain, 0.3f, 0.4f, 0.7f, 1.0f);
	m_SwapChain->Present(true);
}

void AppWindow::OnDestroy()
{
	Window::OnDestroy();
	m_SwapChain->Release();
	Graphics::Get()->Release();
}
