//Loading required stuff
#pragma comment( lib, "Winmm.lib" )
#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <time.h>

//Defining stuff
int screen_height = GetSystemMetrics(SM_CYSCREEN);
int screen_width  = GetSystemMetrics(SM_CXSCREEN);
HWND wndHandle;


// The Win32 C++ WINDOW

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

bool initWindow(HINSTANCE hInstance,LPCSTR name) // Basic window inizialization code
{
	// Setting screen height and width

	WNDCLASSEX wcex;
	ZeroMemory(&wcex,sizeof(WNDCLASSEX));

	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.hInstance		= hInstance;
    wcex.hIcon			= LoadIcon( hInstance, ( LPCTSTR )IDI_APPLICATION );
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= name;
	wcex.hIconSm		= 0;
	RegisterClassEx(&wcex);

	wndHandle = CreateWindowEx(WS_EX_CONTROLPARENT, name,
							 name, 
							 WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE,
							 CW_USEDEFAULT, 
							 CW_USEDEFAULT, 
							 screen_width, 
							 screen_height, 
							 NULL, 
							 NULL, 
							 hInstance, 
							 NULL);
   if (!wndHandle)
   {
	  MessageBox(NULL, "Failed  to create the window!", "Error", MB_OK);
      return false;
   }
   
   ShowWindow(wndHandle, SW_SHOW);
   UpdateWindow(wndHandle);

   return true;
}