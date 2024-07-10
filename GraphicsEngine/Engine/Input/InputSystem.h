#pragma once
#include "InputListener.h"
#include "Engine/Math/Point.h"
#include <unordered_set>

class InputSystem
{
private:
	InputSystem();
	~InputSystem();

public:
	void Update();

	void AddListener(InputListener* listener);
	void RemoveListener(InputListener* listener);

	void SetCursorPosition(const Point& pos);
	void ShowCursor(bool show);

	static InputSystem* Get();
	static void Create();
	static void Release();

private:
	std::unordered_set<InputListener*>  m_Listeners;
	unsigned char m_KeysState[256] = {};
	unsigned char m_OldKeysState[256] = {};

	Point m_OldMousePos;
	bool m_FirstUpdate = true;

	static InputSystem* m_System;
};

