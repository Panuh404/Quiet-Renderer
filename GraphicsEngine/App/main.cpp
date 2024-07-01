#include "AppWindow.h"

int main(int argc, char* argv[])
{
	AppWindow app;
	if (app.Init())
	{
		while (app.IsRunning())
		{
			app.Broadcast();
		}
	}

	return 0;
}
