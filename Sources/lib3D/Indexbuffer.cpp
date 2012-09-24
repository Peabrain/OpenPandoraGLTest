#include "Indexbuffer.h"
#include <malloc.h>
#include <memory>

using namespace lib3D;

INDEXBUFFER::INDEXBUFFER()
{
	IndexBufferId = 0;
}

INDEXBUFFER::~INDEXBUFFER()
{
	glDeleteBuffers(1, &IndexBufferId);
}

INDEXBUFFER::INDEXBUFFER(unsigned short* iBuffer,int iElements,int iStride)
{
	m_uiElements = iElements;
	glGenBuffers(1, &IndexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iStride * m_uiElements, iBuffer,GL_STATIC_DRAW);
}

void INDEXBUFFER::Draw()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId);
	glDrawElements(GL_TRIANGLES, m_uiElements, GL_UNSIGNED_SHORT, NULL);
	GLenum err = glGetError();
	int i = 0;
}
