#pragma once
#include <map>
#include "Mesh.h"
#include "Node.h"
#include "Model.h"

namespace lib3D
{
	class MODEL
	{
	public:
		MODEL();
		virtual ~MODEL();

		MESH* AddMesh(MESH *Mesh);
		MESH* GetMesh(std::string& sName);

		void PreRender();
		void PostRender();

		inline void SetRootNode(NODE& Node)
		{
			NODE r = m_RootNode;
			m_RootNode = Node;
		}
		inline NODE* GetRootNode()
		{
			return &m_RootNode;
		}
		inline void SetModelName(std::string& sName) {m_sModelName = sName;}
		inline std::string GetModelName() {return m_sModelName;}
		void Render(mathHelper::MTX34 mMat,lib3D::CAMERA* Camera);
		void RenderBoundbox(mathHelper::MTX34& mMat,mathHelper::VEC3& vColor);
		bool CheckBoundbox(mathHelper::MTX34& mMat,mathHelper::VEC3& vPos,mathHelper::VEC3& vDir,float& Depth);
		MODEL* GetPtr() { return this;}

		bool GetBoundBox(BOUNDBOX& Box,mathHelper::MTX34 mMat);
		void SetWireframe(bool bWireframe) {m_RootNode.SetWireframe(bWireframe);}
	protected:
		std::string m_sModelName;
		NODE m_RootNode;
	private:
		std::map<std::string,MESH*> m_Meshes;
	};
	class MODELLIBRARY
	{
	public:
		MODELLIBRARY() 
		{
		}
		virtual ~MODELLIBRARY()
		{
			std::map<std::string,MODEL*>::const_iterator it = m_pModels.begin();
			std::map<std::string,MODEL*>::const_iterator ie = m_pModels.end();
			for(;it != ie;it++)
			{
				MODEL *m = it->second;
				delete m;
			}
			m_pModels.clear();
		}
		bool LoadModel(const char* szPath);
		void GetList(std::list<std::string>& List);
		MODEL* GetModelPointer(std::string sPath);
		void Delete(std::string& sPath);

		void PreRender();
		void PostRender();
	private:
		std::map<std::string,MODEL*> m_pModels;
	};

	extern MODELLIBRARY ModelLibrary;
}
