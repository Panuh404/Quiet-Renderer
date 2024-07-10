#include "InputSystem.h"
#include <Windows.h>

InputSystem* InputSystem::m_System = nullptr;

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
	m_System = nullptr;
}

void InputSystem::Update()
{
	POINT current_mouse_pos = {};
	::GetCursorPos(&current_mouse_pos);

	if (m_FirstUpdate)
	{
		m_OldMousePos = Point(current_mouse_pos.x, current_mouse_pos.y);
		m_FirstUpdate = false;
	}

	if (current_mouse_pos.x != m_OldMousePos.m_x || current_mouse_pos.y != m_OldMousePos.m_y)
	{
		// Mouse move event
		std::unordered_set<InputListener*>::iterator it = m_Listeners.begin();
		while (it != m_Listeners.end())
		{
			(*it)->OnMouseMove(Point(current_mouse_pos.x, current_mouse_pos.y));
			++it;
		}
	}
	m_OldMousePos = Point(current_mouse_pos.x, current_mouse_pos.y);

	if (::GetKeyboardState(m_KeysState))
	{
		for (unsigned int i = 0; i < 256; ++i)
		{
			// Key is Down
			if (m_KeysState[i] & 0x80)
			{
				std::unordered_set<InputListener*>::iterator it = m_Listeners.begin();
				while (it != m_Listeners.end())
				{
					if (i == VK_LBUTTON)
					{
						if (m_OldKeysState[i] != m_KeysState[i])
							(*it)->OnLeftMouseDown(Point(current_mouse_pos.x, current_mouse_pos.y));
					}
					else if (i == VK_RBUTTON)
					{
						if (m_OldKeysState[i] != m_KeysState[i])
							(*it)->OnRightMouseDown(Point(current_mouse_pos.x, current_mouse_pos.y));
					}
					else
					{
						(*it)->OnKeyDown(i);
					}
					++it;
				}
			}
			// Key is Up
			else
			{
				if (m_OldKeysState[i] != m_KeysState[i])
				{
					std::unordered_set<InputListener*>::iterator it = m_Listeners.begin();
					while (it != m_Listeners.end())
					{
						if (i == VK_LBUTTON)
						{
							(*it)->OnLeftMouseUp(Point(current_mouse_pos.x, current_mouse_pos.y));
						}
						else if (i == VK_RBUTTON)
						{
							(*it)->OnRightMouseUp(Point(current_mouse_pos.x, current_mouse_pos.y));
						}
						else
						{
							(*it)->OnKeyUp(i);
						}
						++it;
					}
				}
			}
		}
		// Store current keys state to old keys buffer
		::memcpy(m_OldKeysState, m_KeysState, sizeof(unsigned char) * 256);
	}
}

void InputSystem::AddListener(InputListener* listener)
{
	m_Listeners.insert(listener);
}

void InputSystem::RemoveListener(InputListener* listener)
{
	m_Listeners.erase(listener);
}

void InputSystem::SetCursorPosition(const Point& pos)
{
	::SetCursorPos(pos.m_x, pos.m_y);
}

void InputSystem::ShowCursor(bool show)
{
	::ShowCursor(show);
}

InputSystem* InputSystem::Get()
{
	static InputSystem system;
	return &system;
}

void InputSystem::Create()
{
	if (m_System) throw std::exception("ERROR::Input System already created");
	m_System = new InputSystem();	
}

void InputSystem::Release()
{
	if (m_System) return;
	delete m_System;
}
