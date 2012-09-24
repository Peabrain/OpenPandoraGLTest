#pragma once
#include "mathHelper.h"
#include <GLES2/gl2.h>

namespace lib3D
{
	struct BOUNDBOX
	{
		mathHelper::VEC3 vMax;
		mathHelper::VEC3 vMin;
	};
	class VERTEXBUFFER
	{
	public:
		enum TYPE
		{
			NONE = 255,
			VERTEX = 1,
			NORMAL = 2,
			TANGENT = 3,
			BINORMAL = 4,
			COLOR = 5,
			TEXTURE0 = 6
		};
		VERTEXBUFFER();
		VERTEXBUFFER(float* fBuffer,int iElements,int iStride,TYPE Type);
		VERTEXBUFFER(mathHelper::VEC3* fBuffer,int iElements,TYPE Type);
		VERTEXBUFFER(mathHelper::VEC2* fBuffer,int iElements,TYPE Type);
		virtual ~VERTEXBUFFER();

		void Activate(GLuint uiLoc);
		void Deactivate();

		TYPE GetType() {return m_Type;}

		BOUNDBOX* GetBox() {return m_pBox;}
//		float* GetBuffer();

//		mathHelper::VEC3* GetTransformed(mathHelper::MTX34& mOrg);
		unsigned int GetElements();

//		bool GetVertex(int iID,mathHelper::VEC3& v);
	private:
		void Bind();

		TYPE m_Type;
//		float* m_fBuffer;
		int m_iStride;
		int m_iElements;
		int m_iComponets;
		BOUNDBOX* m_pBox;
		GLuint ui32Vbo;
		GLuint m_iLoc;
	};
}
