#pragma once
#include "Vertexbuffer.h"
#include "Indexbuffer.h"
#include "Texture.h"
#include <map>
#include "mathHelper.h"
#include "ShaderLib.h"
#include "Camera.h"

namespace lib3D
{
	class MESH
	{
	public:
		struct INSTANCE
		{
			INSTANCE(mathHelper::MTX34 mMat,TEXTURE* pTexDiffuse,TEXTURE* pTexNormal,TEXTURE* pTexIncandescence,bool bWireframe,GLuint ShaderProgram,lib3D::CAMERA* Camera)
			{
				m_mMat = mMat;
				m_pTexDiffuse = pTexDiffuse;
				m_pTexNormal = pTexNormal;
				m_pTexIncandescence = pTexIncandescence;
				m_bWireframe = bWireframe;
				m_ShaderProgram = ShaderProgram;
				m_Camera = Camera;
			}
			TEXTURE* m_pTexDiffuse;
			TEXTURE* m_pTexNormal;
			TEXTURE* m_pTexIncandescence;
			mathHelper::MTX34 m_mMat;
			bool m_bWireframe;
			GLuint m_ShaderProgram;
			lib3D::CAMERA* m_Camera;
		};

		MESH(std::string sName);
		virtual ~MESH();

		std::string GetName() { return m_sName;};
		void SetName(std::string& sName) { m_sName = sName;};

		void PreRender();
		void Render(mathHelper::MTX34 mMat,TEXTURE* pTexDiffuse,TEXTURE* pTexNormal,TEXTURE* pTexIncandescence,bool bWireframe,GLuint ShaderProgram,lib3D::CAMERA* Camera);
		void PostRender();

		void AddVertexBuffer(VERTEXBUFFER* pBuffer) {m_pBufferList[pBuffer->GetType()] = pBuffer;};
		void SetIndexBuffer(INDEXBUFFER* pBuffer) {m_pIndexBuffer = pBuffer;};

		bool GetBoundBox(BOUNDBOX& Box,mathHelper::MTX34& mMat);
		VERTEXBUFFER* GetVertexBuffer(VERTEXBUFFER::TYPE t);

		bool Raycast(mathHelper::MTX34& mOrg,mathHelper::MTX34& mOrgInv,int& iTriangleID,mathHelper::VEC3& vIntersect);
		bool GetTriangle(int iTriangleID,mathHelper::VEC3& vPos0,mathHelper::VEC3& vPos1,mathHelper::VEC3& vPos2);
	private:
		int Intersect_RayTriangle( mathHelper::VEC3& vOrg, mathHelper::VEC3& vDir, mathHelper::VEC3& V0, mathHelper::VEC3& V1, mathHelper::VEC3& V2, mathHelper::VEC3& vIntersect,float& fDist );

		std::string m_sName;
		std::map<VERTEXBUFFER::TYPE,VERTEXBUFFER*> m_pBufferList;
		INDEXBUFFER* m_pIndexBuffer;

		std::list<INSTANCE> m_Instances;
	};
}
