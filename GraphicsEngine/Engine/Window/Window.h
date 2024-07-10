#pragma once
#include <Windows.h>

class Window
{
public:
	Window();			// Initialize Window
	virtual ~Window();	// Release Window

	bool IsRunning();

	RECT GetClientWindowRect();

	// Events
	virtual void OnCreate();
	virtual void OnUpdate();
	virtual void OnDestroy();
	virtual void OnFocus();
	virtual void OnKillFocus();

private:
	bool Broadcast();

protected:
	HWND m_HWND;
	bool m_IsRunning;
	bool m_IsInit = false;
};

