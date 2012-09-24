#pragma once
#ifdef WIN32
#include <windows.h>
#include <TCHAR.h>
#else
#include "X11/Xlib.h"
#include "X11/Xutil.h"
#endif
#include <EGL/egl.h>

class EngWindow
{
public:
	EngWindow();
	virtual ~EngWindow();

	bool Create();
	void SetEnd(bool bEnd);
	bool GetEnd();

	int GetWidth();
	int GetHeight();

#ifdef WIN32
	HDC GetDisplay();
#else
	EGLNativeDisplayType GetDisplay();
#endif
	EGLNativeWindowType GetWindow();

	void Process();
private:
#ifdef WIN32
	// Windows variables
	HWND				hWnd;
	HDC					hDC;
	EGLNativeWindowType	eglWindow;
#else
	// X11 variables
	Window				x11Window;
	Display*			x11Display;
	long				x11Screen;
	XVisualInfo*		x11Visual;
	Colormap			x11Colormap;
#endif
	bool bDemoDone;
	int m_iWidth;
	int m_iHeight;
};
extern EngWindow *g_MyWindow;
