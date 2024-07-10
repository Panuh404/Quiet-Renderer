#include "AppWindow.h"
#include "Engine/Input/InputSystem.h"

int main(int argc, char* argv[])
{
	// Context Creation
	try
	{
		GraphicsEngine::Create();
		InputSystem::Create();
	}
	catch (...)
	{
		return -1;
	}

	// Window + Loop
	{
		try
		{
			AppWindow app;
			while (app.IsRunning());
		}
		catch (...)
		{
			// Cleanup
			GraphicsEngine::Release();
			InputSystem::Release();
			return -1;
		}
	}
	// Cleanup
	GraphicsEngine::Release();
	InputSystem::Release();
	return 0;
}
