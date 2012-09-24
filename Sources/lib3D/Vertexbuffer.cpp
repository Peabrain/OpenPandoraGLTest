#include "Vertexbuffer.h"
#include <malloc.h>
#include <memory>
#ifdef WIN32
#include <Windows.h>
#endif

using namespace lib3D;

VERTEXBUFFER::VERTEXBUFFER()
{
	m_Type = NONE;
//	m_fBuffer = 0;
	m_iStride = 0;
	m_iElements = 0;
	m_pBox = 0;
	ui32Vbo = 0;
}

VERTEXBUFFER::~VERTEXBUFFER()
{
//	if(m_fBuffer)
//		free(m_fBuffer);
//	m_fBuffer = 0;
	if(m_pBox) delete m_pBox;
}

VERTEXBUFFER::VERTEXBUFFER(float* fBuffer,int iElements,int iStride,TYPE Type)
{
	ui32Vbo = 0;
	m_pBox = 0;
	m_iStride = iStride;
	m_iElements = iElements;
	m_Type = Type;
	m_iComponets = iStride / sizeof(float);
//	m_fBuffer = (float*)malloc(m_iStride * m_iElements);
//	memcpy(m_fBuffer,fBuffer,m_iStride * m_iElements);
	if(Type == VERTEXBUFFER::VERTEX)
	{
		m_pBox = new BOUNDBOX();
		m_pBox->vMax = mathHelper::VEC3(-10000000000000.0f,-10000000000000.0f,-10000000000000.0f);
		m_pBox->vMin = mathHelper::VEC3(10000000000000.0f,10000000000000.0f,10000000000000.0f);
		for(int i = 0;i < iElements;i++)
		{
			mathHelper::VEC3 v;
			v.x = fBuffer[i * iStride / sizeof(float)];
			v.y = fBuffer[i * iStride / sizeof(float) + 1];
			v.z = fBuffer[i * iStride / sizeof(float) + 2];
			if(v.x < m_pBox->vMin.x) m_pBox->vMin.x = v.x;
			if(v.y < m_pBox->vMin.y) m_pBox->vMin.y = v.y;
			if(v.z < m_pBox->vMin.z) m_pBox->vMin.z = v.z;
			if(v.x > m_pBox->vMax.x) m_pBox->vMax.x = v.x;
			if(v.y > m_pBox->vMax.y) m_pBox->vMax.y = v.y;
			if(v.z > m_pBox->vMax.z) m_pBox->vMax.z = v.z;
		}
	}
	glGenBuffers(1, &ui32Vbo);
	Bind();
	glBufferData(GL_ARRAY_BUFFER, m_iStride * m_iElements, fBuffer, GL_STATIC_DRAW);
}
VERTEXBUFFER::VERTEXBUFFER(mathHelper::VEC3* fBuffer,int iElements,TYPE Type)
{
	ui32Vbo = 0;
	m_pBox = 0;
	m_iStride = 3 * sizeof(float);
	m_iElements = iElements;
	m_Type = Type;
	m_iComponets = 3;
//	m_fBuffer = (float*)malloc(m_iStride * m_iElements);
//	memcpy(m_fBuffer,fBuffer,m_iStride * m_iElements);
	if(Type == VERTEXBUFFER::VERTEX)
	{
		m_pBox = new BOUNDBOX();
		m_pBox->vMax = mathHelper::VEC3(-10000000000000.0f,-10000000000000.0f,-10000000000000.0f);
		m_pBox->vMin = mathHelper::VEC3(10000000000000.0f,10000000000000.0f,10000000000000.0f);
		for(int i = 0;i < iElements;i++)
		{
			mathHelper::VEC3 v = fBuffer[i];
			if(v.x < m_pBox->vMin.x) m_pBox->vMin.x = v.x;
			if(v.y < m_pBox->vMin.y) m_pBox->vMin.y = v.y;
			if(v.z < m_pBox->vMin.z) m_pBox->vMin.z = v.z;
			if(v.x > m_pBox->vMax.x) m_pBox->vMax.x = v.x;
			if(v.y > m_pBox->vMax.y) m_pBox->vMax.y = v.y;
			if(v.z > m_pBox->vMax.z) m_pBox->vMax.z = v.z;
		}
	}
	glGenBuffers(1, &ui32Vbo);
	Bind();
	glBufferData(GL_ARRAY_BUFFER, m_iStride * m_iElements, fBuffer, GL_STATIC_DRAW);
}

VERTEXBUFFER::VERTEXBUFFER(mathHelper::VEC2* fBuffer,int iElements,TYPE Type)
{
	ui32Vbo = 0;
	m_pBox = 0;
	m_iStride = 2 * sizeof(float);
	m_iElements = iElements;
	m_Type = Type;
	m_iComponets = 2;
//	m_fBuffer = (float*)malloc(m_iStride * m_iElements);
//	memcpy(m_fBuffer,fBuffer,m_iStride * m_iElements);

	glGenBuffers(1, &ui32Vbo);
	Bind();
	glBufferData(GL_ARRAY_BUFFER, m_iStride * m_iElements, fBuffer, GL_STATIC_DRAW);
}

void VERTEXBUFFER::Activate(GLuint uiLoc)
{
	m_iLoc = uiLoc;
	if(m_iLoc != (GLuint)-1)
	{
		Bind();
		glEnableVertexAttribArray(uiLoc);
		glVertexAttribPointer(m_iLoc, m_iComponets, GL_FLOAT, GL_FALSE, 0, 0);
	}

/*	switch(m_Type)
	{
	case VERTEX: glEnableClientState(GL_VERTEX_ARRAY);glVertexPointer(3, GL_FLOAT, m_iStride, m_fBuffer);break;
	case NORMAL: glEnableClientState(GL_NORMAL_ARRAY);glNormalPointer(GL_FLOAT, m_iStride, m_fBuffer);break;
	case COLOR: glEnableClientState(GL_COLOR_ARRAY);glColorPointer(3, GL_FLOAT, m_iStride, m_fBuffer);break;
	case TEXTURE0: glEnableClientState(GL_TEXTURE_COORD_ARRAY);glTexCoordPointer(2,GL_FLOAT, m_iStride, m_fBuffer);break;
	}
*/
/*	switch(m_Type)
	{
	case VERTEX: glEnableClientState(GL_VERTEX_ARRAY);glVertexPointer(3, GL_FLOAT, m_iStride, m_fBuffer);break;
	case NORMAL: glEnableClientState(GL_NORMAL_ARRAY);glNormalPointer(GL_FLOAT, m_iStride, m_fBuffer);break;
	case COLOR: glEnableClientState(GL_COLOR_ARRAY);glColorPointer(3, GL_FLOAT, m_iStride, m_fBuffer);break;
	case TEXTURE0: glEnableClientState(GL_TEXTURE_COORD_ARRAY);glTexCoordPointer(2,GL_FLOAT, m_iStride, m_fBuffer);break;
	}
*/
}

void VERTEXBUFFER::Deactivate()
{
	if(m_iLoc != (GLuint)-1)
	{
		glDisableVertexAttribArray(m_iLoc);
	}
/*	switch(m_Type)
	{
	case VERTEX: glDisableClientState(GL_VERTEX_ARRAY);break;
	case NORMAL: glDisableClientState(GL_NORMAL_ARRAY);break;
	case COLOR: glDisableClientState(GL_COLOR_ARRAY);break;
	case TEXTURE0: glDisableClientState(GL_TEXTURE_COORD_ARRAY);break;
	}
	*/
}
/*float* VERTEXBUFFER::GetBuffer()
{
	return m_fBuffer;
}

mathHelper::VEC3* VERTEXBUFFER::GetTransformed(mathHelper::MTX34& mOrg)
{
	float* fBuffer = (float*)malloc(m_iStride * m_iElements);

	mathHelper::VEC3TransformCoordArray((mathHelper::VEC3*)fBuffer,&mOrg,(mathHelper::VEC3*)m_fBuffer,m_iElements);

	return (mathHelper::VEC3*)fBuffer;
}
*/
unsigned int VERTEXBUFFER::GetElements()
{
	return m_iElements;
}
/*
bool VERTEXBUFFER::GetVertex(int iID,mathHelper::VEC3& v)
{
	if(m_Type == VERTEX)
	{
		if(iID < m_iElements)
		{
			v = ((mathHelper::VEC3*)m_fBuffer)[iID];
			return true;
		}
		return false;
	}
	return false;
}
*/
void VERTEXBUFFER::Bind()
{
	// Bind the VBO so we can fill it with data
	glBindBuffer(GL_ARRAY_BUFFER, ui32Vbo);
}
