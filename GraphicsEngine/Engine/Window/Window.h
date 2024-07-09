#pragma once
#include <Windows.h>

class Window
{
public:
	Window();
	virtual ~Window();

	bool Init();
	bool Broadcast();
	bool Release();
	bool IsRunning();

	RECT GetClientWindowRect();
	void SetHWND(HWND hwnd);

	// Events
	virtual void OnCreate();
	virtual void OnUpdate();
	virtual void OnDestroy();

	virtual void OnFocus();
	virtual void OnKillFocus();

protected:
	HWND m_hwnd;
	bool m_isRunning;
};

