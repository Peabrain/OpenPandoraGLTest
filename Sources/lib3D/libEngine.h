#pragma once
#include <map>
#include <string>
#include "SceneNode.h"
#include "Camera.h"
#include "Input.h"

namespace lib3D
{
	class LIBENGINE
	{
	public:
		LIBENGINE();
		virtual ~LIBENGINE();
		
		void Init(int argc,char** argv);
		
		int Process(float TimeDelta);
		void Render();

		bool CreateEntity(lib3D::NODE_ENTITY& Entity);
		SCENENODE* AddNodeToScenegraph(SCENENODE Node);

		bool ChangeEntityName(const char* szOldName,const char* szNewName);
//		NODE_ENTITY* GetEntityPointer(std::string& sPath);
		bool GetEntity(std::string sName,lib3D::NODE_ENTITY& Node);
		bool CopyEntity(std::string sName,std::string& sNewName);
		lib3D::NODE_ENTITY* GetEntityPtr(std::string sName);
		std::map<std::string,lib3D::NODE_ENTITY>* GetEntityList() { return &m_Entities;};

		SCENENODE* CheckBoundbox(mathHelper::VEC3& vPos,mathHelper::VEC3& vDir);
		void ClearNodeMousestate();

		libEngine::MYINPUT* GetInputManager();
	private:
		int m_iUnikID;

		std::map<int,SCENENODE> SceneGraph;
		std::map<std::string,lib3D::NODE_ENTITY> m_Entities;

		libEngine::MYINPUT InputManager;
	};
	extern LIBENGINE LibraryEngine;
	extern CAMERA* SceneCamera;
}
