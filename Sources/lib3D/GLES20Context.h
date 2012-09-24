#pragma once
#include <EGL/egl.h>
#include "EngWindow.h"

class GLES20Context
{
public:
	GLES20Context();
	virtual ~GLES20Context();

	bool Init(EngWindow* pWindow);
	void Swap();
private:
	bool TestEGLError(const char* pszLocation);

	EGLint ai32ContextAttribs[3];
	// EGL variables
	EGLDisplay			eglDisplay;
	EGLConfig			eglConfig;
	EGLSurface			eglSurface;
	EGLContext			eglContext;
};
