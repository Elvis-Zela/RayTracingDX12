#include "Window.h"

WCHAR			WindowClass[MAX_STRING_VALUE];

INT				WindowWidth;
INT				WindowHeight;

/* - Window message loop - */

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	switch (message)
	{
	case WM_PAINT:													/* - Paint the window - */
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYUP:													/* - Checks Key pressed, if ESC then closes window - */
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		break;
	case WM_DESTROY:												/* - If window is being destroyed - */
		PostQuitMessage(0);											/* - Indicate to system that a request has been made to terminate  - */
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);		/* - Default window procedures - */
	}
	return 0;
}

namespace Window
{
	/* - Creating a new window - */
	HRESULT Create(LONG width, LONG height, HINSTANCE& instance, HWND& window, LPCWSTR title)
	{
		/* - Initialising Global Variables - */
		wcscpy_s(WindowClass, TEXT("WindowClass"));

		RECT rc = { 0, 0, width, height };

		WindowWidth = (rc.right - rc.left);
		WindowHeight = (rc.bottom - rc.top);


		/* - register the window class - */
		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;							/* - Window Procedure to most features and behaviours of the window, defined above */
		wcex.cbClsExtra = 0;								/* - Draw in extra memory at run time, set to 0 since uneeded - */
		wcex.cbWndExtra = 0;								/* - Draw in extra memory at run time, set to 0 since uneeded - */

		wcex.hInstance = hInstance();						/* - Instance of the running program - */
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);			/* - Simple Arrow Cursor*/
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	/* - Start up background colour of the window, base white - */

		wcex.lpszMenuName = NULL;							/* - Menu names, not necessary - */
		wcex.lpszClassName = WindowClass;

		wcex.hIcon = nullptr;								/* - Window Top Left Icon - */
		wcex.hIconSm = nullptr;								/* - Taskbar Window Icon*/

		if (!RegisterClassEx(&wcex))
		{
			throw std::runtime_error("ERROR: Failed to register window!");
		}

		/* - Getting desktop resolution - */
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);

		int x = (desktop.right - width) / 2;

		/* - Creating the window - */
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
		window = CreateWindow(wcex.lpszClassName, title, WS_OVERLAPPEDWINDOW, x, 0,
			WindowWidth, WindowHeight, NULL, NULL, instance, NULL);
		if (!window) return E_FAIL;

		/* - Set window icon - */
		HANDLE hIcon = LoadImageA(hInstance(), "RTIco", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
		SendMessage(window, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

		/* - Show Window - */
		ShowWindow(window, SW_SHOWDEFAULT);
		UpdateWindow(window);


		/* - TEMPORARY MSG LOOP TO KEEP WINDOW OPEN - */
		MSG msg{ 0 };

		while (msg.message != WM_QUIT)
		{
			if ( PeekMessage(&msg, 0, 0, 0, PM_REMOVE) )
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		return S_OK;
	}

}