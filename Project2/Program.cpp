#include "Window.h"
#include <windows.h>

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	
	Window* pWindow = new Window();

	while (pWindow->ProcessMessages()) { Sleep(10); }

	delete pWindow;
	return 0;
}