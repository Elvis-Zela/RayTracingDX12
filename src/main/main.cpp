#include "Constants.h"
#include "../Window/Window.h"

WCHAR				WindowName[MAX_STRING_VALUE];

void InitWnd() {
	/* - Initialising Global variables - */
	wcscpy_s(WindowName, TEXT("RayTracer"));

	/* - Creating the Window - */
	HINSTANCE instance = hInstance();
	HWND window_handle;
	Window::Create(WINDOW_WIDTH, WINDOW_HEIGHT, instance, window_handle, WindowName);
}

int main()
{
	InitWnd();
}

