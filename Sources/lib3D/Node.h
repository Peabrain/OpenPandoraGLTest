#pragma once
#include <list>
#include "mathHelper.h"
#include "Mesh.h"
#include "Texture.h"
namespace lib3D
{
	class NODE
	{
	public:
		struct MESHLINK
		{
			MESHLINK()
			{
				m_pMeshLink = 0;
				m_Tex = 0;
				m_Norm = 0;
				m_bWireframe = false;
			}
			MESHLINK(MESH *MeshLink,TEXTURE* Tex,TEXTURE* Norm,TEXTURE* Incan,GLuint ShaderProgram)
			{
				m_pMeshLink = MeshLink;
				m_Tex = Tex;
				m_Norm = Norm;
				m_Incan = Incan;
				m_bWireframe = false;
				m_ShaderProgram = ShaderProgram;
			}
			MESHLINK* GetPtr() {return this;}
			
			TEXTURE* m_Tex;
			TEXTURE* m_Norm;
			TEXTURE* m_Incan;
			MESH* m_pMeshLink;
			bool m_bWireframe;
			GLuint m_ShaderProgram;
		};

		NODE();
		virtual ~NODE();

		inline void SetPosition(const mathHelper::VEC3& vPosition)
		{
			m_vPosition = vPosition;
		}
		inline void SetRotation(const mathHelper::VEC3& vRotation)
		{
			m_vRotation = vRotation;
		}
		std::list<MESHLINK>* GetMeshLinks() {return &m_MeshLinks;}
		std::list<NODE>* GetNodes() { return &m_Nodes;}
		void AddMeshLink(MESHLINK MeshLink);
		void Render(mathHelper::MTX34 mMat,lib3D::CAMERA* Camera);
		void AddNode(NODE& Node) {m_Nodes.push_back(Node);}

		NODE* GetPtr() {return this;}

		bool GetBoundBox(BOUNDBOX& Box,mathHelper::MTX34& mMat);
		void SetWireframe(bool bWireframe);
	private:
		mathHelper::VEC3 m_vPosition;
		mathHelper::VEC3 m_vRotation;
		mathHelper::VEC3 m_vScaling;

		std::list<NODE> m_Nodes;
		std::list<MESHLINK> m_MeshLinks;
	};
}
