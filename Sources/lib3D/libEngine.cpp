#include "libEngine.h"
#include "Node.h"
#include "Camera.h"
#ifndef WIN32
#include <linux/input.h>
#endif

using namespace lib3D;

lib3D::CAMERA* lib3D::SceneCamera = 0;

lib3D::LIBENGINE lib3D::LibraryEngine;

LIBENGINE::LIBENGINE()
{
	m_iUnikID = 0;
}

LIBENGINE::~LIBENGINE()
{
}

void LIBENGINE::Init(int argc,char** argv)
{
}

int LIBENGINE::Process(float TimeDelta)
{
	InputManager.Process();
	std::map<int,SCENENODE>::iterator it = SceneGraph.begin();
	std::map<int,SCENENODE>::iterator ie = SceneGraph.end();
	for(;it != ie;it++)
	{
		it->second.Process(TimeDelta);
	}
	return 1;
}
void LIBENGINE::Render()
{
	mathHelper::MTX34 mMat;// = cam->GetMatrix();
	mathHelper::MTX34Identity(&mMat);
//	mathHelper::MTX34Inv(&mMat,&mMat);
	std::map<int,SCENENODE>::iterator it = SceneGraph.begin();
	std::map<int,SCENENODE>::iterator ie = SceneGraph.end();

	lib3D::CAMERA* Camera = lib3D::SceneCamera;
	for(;it != ie;it++)
	{
		it->second.Render(mMat,Camera);
	}
}
void LIBENGINE::ClearNodeMousestate()
{
	std::map<int,SCENENODE>::iterator it = SceneGraph.begin();
	std::map<int,SCENENODE>::iterator ie = SceneGraph.end();
//	for(;it != ie;it++) it->second.SetMouseState(NODE::NONE);
}

SCENENODE* LIBENGINE::CheckBoundbox(mathHelper::VEC3& vPos,mathHelper::VEC3& vDir)
{
	mathHelper::MTX34 mMat;
	mathHelper::MTX34Inv(&mMat,&mMat);
	std::map<int,SCENENODE>::iterator it = SceneGraph.begin();
	std::map<int,SCENENODE>::iterator ie = SceneGraph.end();
	std::map<int,SCENENODE>::iterator itSel = SceneGraph.end();
	float Depth = 1000000000.0f;
	for(;it != ie;it++)
	{
		float fDepth = Depth;
		if(it->second.CheckBoundbox(vPos,vDir,fDepth))
		{
			if(fDepth < Depth)
			{
				Depth = fDepth;
				itSel = it;
			}
		}
	}
	if(itSel != SceneGraph.end()) 
		return itSel->second.GetPtr();
	return 0;
}

SCENENODE* LIBENGINE::AddNodeToScenegraph(SCENENODE Node)
{
	Node.SetUnikID(m_iUnikID);
	SceneGraph[m_iUnikID++] = Node;
	std::map<int,SCENENODE>::iterator it = SceneGraph.find(m_iUnikID - 1);
	if(it != SceneGraph.end())
	{
		return (SCENENODE*)it->second.GetPtr();
	}
	return 0;
}

bool LIBENGINE::CreateEntity(lib3D::NODE_ENTITY& Entity)
{
	std::map<std::string,lib3D::NODE_ENTITY>::iterator it = m_Entities.find(Entity.GetName());
	if(it == m_Entities.end())
	{
		std::string s(Entity.GetName());
		m_Entities[s] = Entity;
		return true;
	}
	return false;
}
bool LIBENGINE::GetEntity(std::string sName,lib3D::NODE_ENTITY& Node)
{
	std::map<std::string,lib3D::NODE_ENTITY>::iterator it = m_Entities.find(sName);
	if(it != m_Entities.end())
	{
		Node = it->second;
		return true;
	}
	return false;
}
bool LIBENGINE::CopyEntity(std::string sName,std::string& sNewName)
{
	std::map<std::string,lib3D::NODE_ENTITY>::iterator it = m_Entities.find(sName);
	if(it != m_Entities.end())
	{
		std::string sCopy = sName;
		sCopy += "_Copy";
		std::map<std::string,lib3D::NODE_ENTITY>::iterator itc = m_Entities.find(sCopy);
		if(itc == m_Entities.end())
		{
			m_Entities[sCopy] = it->second;
			std::map<std::string,lib3D::NODE_ENTITY>::iterator itc = m_Entities.find(sCopy);
			if(itc != m_Entities.end())
			{
				itc->second.SetName(sCopy);
				sNewName = sCopy;
				return true;
			}
		}
	}
	return false;
}

lib3D::NODE_ENTITY* LIBENGINE::GetEntityPtr(std::string sName)
{
	std::map<std::string,lib3D::NODE_ENTITY>::iterator it = m_Entities.find(sName);
	if(it != m_Entities.end())
	{
		return it->second.GetPtr();
	}
	return 0;
}

bool LIBENGINE::ChangeEntityName(const char* szOldName,const char* szNewName)
{
	std::map<std::string,lib3D::NODE_ENTITY>::iterator itOld = m_Entities.find(szOldName);
	if(itOld != m_Entities.end())
	{
		std::map<std::string,lib3D::NODE_ENTITY>::iterator itNew = m_Entities.find(szNewName);
		if(itNew == m_Entities.end())
		{
			std::string s(szNewName);
			itOld->second.SetName(s);
			m_Entities[std::string(szNewName)] = itOld->second;
			itNew = m_Entities.find(szNewName);
			std::string sOldName(szOldName);
			std::map<int,SCENENODE>::iterator itsn = SceneGraph.begin();
			std::map<int,SCENENODE>::iterator iesn = SceneGraph.end();
			for(;itsn != iesn;itsn++)
			{
				if(itsn->second.GetEntity() == itOld->second.GetPtr())
				{
					itsn->second.SetEntity(itNew->second.GetPtr());
				}
			}
			m_Entities.erase(itOld);
			return true;
		}
	}
	return false;
}
libEngine::MYINPUT* LIBENGINE::GetInputManager()
{
	return &InputManager;
}

/*
std::string MODELLIBRARY::Copy(std::string& sName)
{
	std::map<std::string,MODEL_INSTANCE>::const_iterator itOld = m_pModelInstances.find(sName.c_str());
	if(itOld != m_pModelInstances.end())
	{
		std::string s = itOld->first;
		s += "Copy";
		m_pModelInstances[s] = itOld->second;
	}
	return "";
}

NODE_ENTITY* MODELLIBRARY::GetInstancePointer(std::string& sPath)
{
	std::map<std::string,NODE_ENTITY>::iterator it = m_pModelInstances.find(sPath);
	if(it != m_pModelInstances.end())
	{
		MODEL_INSTANCE* mi = (MODEL_INSTANCE*)&(it->second);
		return mi;
	}
	return 0;
}

void MODELLIBRARY::Delete(std::string& sPath)
{
	std::map<std::string,MODEL_INSTANCE>::iterator it = m_pModelInstances.find(sPath);
	if(it != m_pModelInstances.end())
	{
		MODEL_INSTANCE m = it->second;
		m_pModelInstances.erase(it);
	}
}
*/
