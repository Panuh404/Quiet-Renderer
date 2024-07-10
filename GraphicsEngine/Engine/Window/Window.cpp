#include "Window.h"

#include <exception>


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CREATE:
		{
			// Event fired when the window is created
			// 
			break;
		}
	case WM_SETFOCUS:
		{
			// Event fired when the window is Focused
			Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			if (window) window->OnFocus();
			break;
		}
	case WM_KILLFOCUS:
		{
			// Event fired when the window is Unfocused
			Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			window->OnKillFocus();
			break;
		}
	case WM_DESTROY:
		{
			// Event fired when the window is destroyed
			Window * window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			window->OnDestroy();
			::PostQuitMessage(0);
			break;
		}
	default:
		return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return NULL;
}

Window::Window()
{
	WNDCLASSEX wc;
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = L"Window Class";
	wc.lpszMenuName = L"";
	wc.style = NULL;
	wc.lpfnWndProc = WndProc;

	if (!::RegisterClassEx(&wc)) // If the registration fail, return false
	{
		throw std::exception("ERROR::Window - Registration Failed");
	}

	// Creation of the window
	m_HWND = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
		L"Window Class",
		L"Application",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1024,
		768,
		NULL,
		NULL,
		NULL,
		NULL);
	// if the creation fail, return false
	if (!m_HWND)
	{
		throw std::exception("ERROR::Window - Creation Failed");
	}

	::ShowWindow(m_HWND, SW_SHOW);
	::UpdateWindow(m_HWND);

	m_IsRunning = true;
}

Window::~Window() {}

bool Window::Broadcast()
{
	MSG msg;

	if (!this->m_IsInit)
	{
		SetWindowLongPtr(m_HWND, GWLP_USERDATA, (LONG_PTR)this);
		this->OnCreate();
		this->m_IsInit = true;
	}

	this->OnUpdate();
	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Sleep(0);
	return true;
}

bool Window::IsRunning()
{
	if (m_IsRunning)
	{
		Broadcast();
	}
	return m_IsRunning;
}

RECT Window::GetClientWindowRect()
{
	RECT rc;
	::GetClientRect(this->m_HWND, &rc);
	return rc;
}

void Window::OnCreate()	{}
void Window::OnUpdate() {}
void Window::OnDestroy()
{
	m_IsRunning = false;
}

void Window::OnFocus() {}
void Window::OnKillFocus() {}
