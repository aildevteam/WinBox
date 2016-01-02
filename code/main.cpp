	// main.cpp - implements the managers
// Loading required stuff - the managers
#include "Win32Manager.h" // The Win32 C++ Manager header
#include "VNEngine.h"
#include "main.h" // The main.cpp header - defining stuff

// Getting our message
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) // Basic messages structure
{
	switch (message) 
	{
		case WM_DESTROY:
			PostQuitMessage(0); //Quiting
			break;

		case WM_KEYDOWN:
			{
				if(wParam == VK_ESCAPE){
					PostQuitMessage(0);
					break;
				}

				if(wParam == VK_RETURN){
					if(vne->gameState == 0)
						handleMenu();
					if(vne->gameState == 1)
						vne->next();
					else if(vne->gameState == 3)
						vne->setChoice();
				}

				if(vne->gameState == 0)
				{
					int menuLen = sizeof(Menu) / sizeof(Menu[0]);
					if(wParam == VK_DOWN)
						vne->choiceCursor(&vne->menu,menuLen,-1);
					else if(wParam == VK_UP)
						vne->choiceCursor(&vne->menu,menuLen,1);
				}

				if(vne->gameState == 3)
				{
					if(wParam == VK_UP)
						vne->choiceCursor(&vne->curChoice,vne->numChoices,1);
					if(wParam == VK_DOWN)
						vne->choiceCursor(&vne->curChoice,vne->numChoices,-1);
				}

				break;
			}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,  
    LPSTR lpCmdLine, int nCmdShow )									// Basic main function sturcture
{

	if (!initWindow(hInstance,"VisualNovel"))
		return false;

	if(!initEngine())
		return false;

	if(strlen(lpCmdLine)!=0)
	{
		vne->curLevel = atoi(lpCmdLine);
		vne->gameState = 1;
		vne->getGameText();
	}

	// Main message loop:
	// Enter the message loop
    MSG msg; 
    ZeroMemory( &msg, sizeof(msg) );

    while( msg.message!=WM_QUIT )
    {
		// Check for messages
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
        {
			// Handling messages
			TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
		else 		// this is called when no messages are pending
		{
				mScenes();// call our render function
			    // Calling our scene
		}
    }
	Shutdown();

  return (int) msg.wParam;
}

