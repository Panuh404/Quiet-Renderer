#pragma once
#include "Engine/Window/Window.h"
#include "Engine/Graphics/Graphics.h"
#include "Engine/Graphics/SwapChain.h"

class AppWindow : public Window
{
public:
	AppWindow();
	~AppWindow();

	// Inherited via window
	void OnCreate() override;
	void OnUpdate() override;
	void OnDestroy() override;

private:
	SwapChain* m_SwapChain;
};

