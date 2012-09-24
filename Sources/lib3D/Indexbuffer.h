#pragma once
#ifdef WIN32
#include <Windows.h>
#endif
#include <GLES2/gl2.h>

namespace lib3D
{
	class INDEXBUFFER
	{
	public:
		INDEXBUFFER();
		INDEXBUFFER(unsigned short* iBuffer,int iElements,int iStride);
		virtual ~INDEXBUFFER();

		void Draw();
	private:
		GLuint IndexBufferId;
		GLuint m_uiElements;
	};
}
