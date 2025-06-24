#include <iostream>
#include "Window.h"

int main() {
	
	std::cout << "Creating window...\n";
	
	Window* pWindow = new Window();

	bool running = true;

	while (running) {
		
		if (!pWindow->ProcessMessages()) {
			std::cout << "Close Window\n";
			running = false;
		}
		// Render
		Sleep(10);
	}

	delete pWindow;
	return 0;
}