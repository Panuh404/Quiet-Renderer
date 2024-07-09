#pragma once
#include "InputListener.h"
#include "Engine/Math/Point.h"
#include <unordered_set>

class InputSystem
{
public:
	InputSystem();
	~InputSystem();

	void Update();

	void AddListener(InputListener* listener);
	void RemoveListener(InputListener* listener);

	static InputSystem* Get();

private:
	std::unordered_set<InputListener*>  m_Listeners;
	unsigned char m_KeysState[256] = {};
	unsigned char m_OldKeysState[256] = {};

	Point m_OldMousePos;
	bool m_FirstUpdate = true;
};

