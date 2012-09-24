#include "EngWindow.h"
#include "libEngine.h"
#include "Input.h"
#include <stdio.h>

// Max width and height of the window
#ifdef WIN32
#define WINDOW_WIDTH	800 * 2
#define WINDOW_HEIGHT	480 * 2
#else
#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	480
#endif

#ifdef WIN32
#ifndef NO_GDI
#define	WINDOW_CLASS _T("PVRShellClass")
#endif
#endif

/*!****************************************************************************
 @Function		WndProc
 @Input			hWnd		Handle to the window
 @Input			message		Specifies the message
 @Input			wParam		Additional message information
 @Input			lParam		Additional message information
 @Return		LRESULT		result code to OS
 @Description	Processes messages for the main window
******************************************************************************/
EngWindow *g_MyWindow = 0;
#ifdef WIN32
#ifndef NO_GDI
// Variable set in the message handler to finish the demo
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		/*
			Here we are handling 2 system messages: screen saving and monitor power.
			They are especially relevent on mobile devices.
		*/
#ifndef UNDER_CE
		case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
				case SC_SCREENSAVE:					// Screensaver trying to start ?
				case SC_MONITORPOWER:				// Monitor trying to enter powersave ?
				return 0;							// Prevent this from happening
			}
			break;
		}
#endif
		case WM_KEYDOWN:
			{
				unsigned short c = tolower(wParam);
				lib3D::LibraryEngine.GetInputManager()->KeyboardDown(c);
			}break;
		case WM_KEYUP:
			{
				unsigned short c = tolower(wParam);
				lib3D::LibraryEngine.GetInputManager()->KeyboardUp(c);
			}break;
		// Handles the close message when a user clicks the quit icon of the window
		case WM_CLOSE:
			if(g_MyWindow) g_MyWindow->SetEnd(true);
			PostQuitMessage(0);
			return 1;

		default:
			break;
	}

	// Calls the default window procedure for messages we did not handle
	return DefWindowProc(hWnd, message, wParam, lParam);
}
#endif
#endif

EngWindow::EngWindow()
{
	g_MyWindow = this;
	bDemoDone = false;
#ifdef WIN32
	// Windows variables
	hWnd	= 0;
	hDC		= 0;
	eglWindow	= 0;
#else
	// X11 variables
	x11Window	= 0;
	x11Display	= 0;
	x11Screen	= 0;
	x11Visual	= 0;
	x11Colormap	= 0;
#endif
}
EngWindow::~EngWindow()
{
#ifdef WIN32
#ifndef NO_GDI
	// Release the device context
	if (hDC) ReleaseDC(hWnd, hDC);

	// Destroy the eglWindow
	if (hWnd) DestroyWindow(hWnd);
#endif
#else
	if (x11Window) XDestroyWindow(x11Display, x11Window);
    if (x11Colormap) XFreeColormap( x11Display, x11Colormap );
	if (x11Display) XCloseDisplay(x11Display);

    delete x11Visual;
#endif
}
void EngWindow::SetEnd(bool bEnd)
{
	bDemoDone = bEnd;
}
bool EngWindow::GetEnd()
{
	return bDemoDone;
}
bool EngWindow::Create()
{
#ifdef WIN32
	/*
		Step 0 - Create a EGLNativeWindowType that we can use for OpenGL ES output
	*/
#ifndef NO_GDI
	// Register the windows class
	char title[500];  // to hold title
	GetConsoleTitleA( title, 500 );
	HWND hwndConsole = FindWindowA( NULL, title );
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hwndConsole, GWL_HINSTANCE);
	
	WNDCLASS sWC;
    sWC.style = CS_HREDRAW | CS_VREDRAW;
	sWC.lpfnWndProc = WndProc;
    sWC.cbClsExtra = 0;
    sWC.cbWndExtra = 0;
    sWC.hInstance = hInstance;
    sWC.hIcon = 0;
    sWC.hCursor = 0;
    sWC.lpszMenuName = 0;
	sWC.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    sWC.lpszClassName = WINDOW_CLASS;
	unsigned int nWidth = WINDOW_WIDTH;
	unsigned int nHeight = WINDOW_HEIGHT;

	ATOM registerClass = RegisterClass(&sWC);
	if (!registerClass)
	{
		MessageBox(0, _T("Failed to register the window class"), _T("Error"), MB_OK | MB_ICONEXCLAMATION);
	}
#if defined(UNDER_CE)
	// Get the monitor dimensions
	{
		HMONITOR	hMonitor;
		BOOL		bRet;
		POINT		p;
		MONITORINFO sMInfo;

		p.x			= 0;
		p.y			= 0;
		hMonitor	= MonitorFromPoint(p, MONITOR_DEFAULTTOPRIMARY);
		sMInfo.cbSize = sizeof(sMInfo);
		bRet = GetMonitorInfo(hMonitor, &sMInfo);
		if (!bRet)
		{
			MessageBox(0, _T("Failed to get monitor info"), _T("Error"), MB_OK|MB_ICONEXCLAMATION);
			goto cleanup;
		}

		nWidth = sMInfo.rcMonitor.right - sMInfo.rcMonitor.left;
		nHeight = sMInfo.rcMonitor.bottom - sMInfo.rcMonitor.top;
	}
#endif
	// Create the eglWindow
	RECT	sRect;
	SetRect(&sRect, 0, 0, nWidth, nHeight);
	AdjustWindowRectEx(&sRect, WS_CAPTION | WS_SYSMENU, false, 0);
	hWnd = CreateWindow( WINDOW_CLASS, _T("HelloTriangle"), WS_VISIBLE | WS_SYSMENU,
						 0, 0, nWidth, nHeight, NULL, NULL, hInstance, NULL);
	eglWindow = hWnd;

	// Get the associated device context
	hDC = GetDC(hWnd);
	if (!hDC)
	{
		MessageBox(0, _T("Failed to create the device context"), _T("Error"), MB_OK|MB_ICONEXCLAMATION);
		return false;
	}
	m_iWidth = nWidth;
	m_iHeight = nHeight;
#endif
	/*
		Step 1 - Get the default display.
		EGL uses the concept of a "display" which in most environments
		corresponds to a single physical screen. Since we usually want
		to draw to the main screen or only have a single screen to begin
		with, we let EGL pick the default display.
		Querying other displays is platform specific.
	*/
#else
	/*
		Step 0 - Create a NativeWindowType that we can use it for OpenGL ES output
	*/
	Window					sRootWindow;
    XSetWindowAttributes	sWA;
	unsigned int			ui32Mask;
	int						i32Depth;
	int 					i32Width, i32Height;

	// Initializes the display and screen
	x11Display = XOpenDisplay( 0 );
	if (!x11Display)
	{
		printf("Error: Unable to open X display\n");
		return false;
	}
	x11Screen = XDefaultScreen( x11Display );

	// Gets the window parameters
	sRootWindow = RootWindow(x11Display, x11Screen);
	i32Depth = DefaultDepth(x11Display, x11Screen);
	x11Visual = new XVisualInfo;
	XMatchVisualInfo( x11Display, x11Screen, i32Depth, TrueColor, x11Visual);
	if (!x11Visual)
	{
		printf("Error: Unable to acquire visual\n");
		return false;
	}
    x11Colormap = XCreateColormap( x11Display, sRootWindow, x11Visual->visual, AllocNone );
    sWA.colormap = x11Colormap;

    
    // Add to these for handling other events
    sWA.event_mask = StructureNotifyMask | ExposureMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask;
    sWA.override_redirect = 1;
    ui32Mask = CWBackPixel | CWBorderPixel | CWEventMask | CWColormap | CWOverrideRedirect;

	i32Width  = WINDOW_WIDTH  < XDisplayWidth(x11Display, x11Screen) ? WINDOW_WIDTH : XDisplayWidth(x11Display, x11Screen);
	i32Height = WINDOW_HEIGHT < XDisplayHeight(x11Display,x11Screen) ? WINDOW_HEIGHT: XDisplayHeight(x11Display,x11Screen);

	// Creates the X11 window
    x11Window = XCreateWindow( x11Display, RootWindow(x11Display, x11Screen), 0, 0, i32Width, i32Height,
								 0, CopyFromParent, InputOutput, CopyFromParent, ui32Mask, &sWA);
	XMapWindow(x11Display, x11Window);

	XWarpPointer(x11Display, None, x11Window, 0, 0, 0, 0, 0, 0);               
        XMapRaised(x11Display, x11Window);                                 
        XGrabKeyboard(x11Display, x11Window, True, GrabModeAsync,GrabModeAsync, CurrentTime);                                     
        XGrabPointer(x11Display, x11Window, True, ButtonPressMask,GrabModeAsync, GrabModeAsync, x11Window, None, CurrentTime);  


	XFlush(x11Display);

	/*
		Step 1 - Get the default display.
		EGL uses the concept of a "display" which in most environments
		corresponds to a single physical screen. Since we usually want
		to draw to the main screen or only have a single screen to begin
		with, we let EGL pick the default display.
		Querying other displays is platform specific.
	*/
	m_iWidth = i32Width;
	m_iHeight = i32Height;
#endif
	return true;
}
#ifdef WIN32
HDC EngWindow::GetDisplay()
{
	return hDC;
}
#else
EGLNativeDisplayType EngWindow::GetDisplay()
{
	return (EGLNativeDisplayType)x11Display;
}
#endif
EGLNativeWindowType EngWindow::GetWindow()
{
#ifdef WIN32
	return eglWindow;
#else
	return (EGLNativeWindowType)x11Window;
#endif
}
void EngWindow::Process()
{
#ifdef WIN32
#ifndef NO_GDI
	// Managing the window messages
	MSG msg;
	PeekMessage(&msg, hWnd, NULL, NULL, PM_REMOVE);
	TranslateMessage(&msg);
	DispatchMessage(&msg);
#endif
#else
	// Managing the X11 messages
	int i32NumMessages = XPending( x11Display );
	for( int i = 0; i < i32NumMessages; i++ )
	{
		XEvent	event;
		XNextEvent( x11Display, &event );

		switch( event.type )
		{
		// Exit on mouse click
//			case ButtonPress:
		case ButtonPress:
			{
				unsigned int uiButton = event.xbutton.button;
				printf("Press Button: %i\n",uiButton);
			}break;
		case ButtonRelease:
			{
				unsigned int uiButton = event.xbutton.button;
				printf("Unpress Button: %i\n",uiButton);
			}break;
		case KeyPress:
			{
				KeySym key;
				char text[255];
				if(XLookupString(&event.xkey,text,255,&key,0)==1) 
				{
					unsigned short c = tolower(text[0]);
					lib3D::LibraryEngine.GetInputManager()->KeyboardDown(c);
				}
			}break;
		case  KeyRelease:
			{
				KeySym key;
				char text[255];
				if(XLookupString(&event.xkey,text,255,&key,0)==1) 
				{
					unsigned short c = tolower(text[0]);
					lib3D::LibraryEngine.GetInputManager()->KeyboardUp(c);
				}
			}break;
		default:
			break;
		}
	}
#endif
}
int EngWindow::GetWidth()
{
	return m_iWidth;
}
int EngWindow::GetHeight()
{
	return m_iHeight;
}
