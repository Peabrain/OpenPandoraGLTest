#include "SceneNode.h"
#include "Model.h"

using namespace lib3D;

void NODE_ENTITY::SetProxyModel(lib3D::MODEL* Proxy)
{
	m_pProxyModel = Proxy;
	std::list<lib3D::NODE*> NodeList;
	lib3D::NODE* Node = m_pProxyModel->GetRootNode();
	NodeList.push_back(Node);
	int iMeshLinkID = 0;
	while(!NodeList.empty())
	{
		Node = *NodeList.begin();
		NodeList.pop_front();
		std::list<lib3D::NODE::MESHLINK>* pMeshLinks = Node->GetMeshLinks();
		std::list<lib3D::NODE::MESHLINK>::iterator it = pMeshLinks->begin();
		std::list<lib3D::NODE::MESHLINK>::iterator ie = pMeshLinks->end();
		for(;it != ie;it++)
		{
			lib3D::NODE::MESHLINK* ml = (lib3D::NODE::MESHLINK*)(it->GetPtr());
			if(ml->m_pMeshLink)
				m_pMeshlinkList[iMeshLinkID++] = MESHLINK(ml);
		}
		std::list<lib3D::NODE>* Nodes;
		Nodes = Node->GetNodes();
		std::list<lib3D::NODE>::iterator itn = Nodes->begin();
		std::list<lib3D::NODE>::iterator ien = Nodes->end();
		for(;itn != ien;itn++)
		{
			lib3D::NODE* n = itn->GetPtr();
			NodeList.push_back(n);
		}
	}
}
bool lib3D::NODE_ENTITY::ChangeParameter(int iID,std::string sParameter)
{
	std::map<int,MESHLINK>::iterator it = m_pMeshlinkList.find(iID);
	if(it != m_pMeshlinkList.end())
	{
		it->second.m_Mine.m_Tex = lib3D::TextureLibrary.Get(sParameter);
		return true;
	}
	return false;
}
void lib3D::NODE_ENTITY::Render(mathHelper::MTX34& mMat,lib3D::CAMERA* Camera)
{
	std::map<int,MESHLINK>::iterator it = m_pMeshlinkList.begin();
	std::map<int,MESHLINK>::iterator ie = m_pMeshlinkList.end();
	for(;it != ie;it++) *it->second.m_pOrg = it->second.m_Mine;
	if(m_pProxyModel) m_pProxyModel->Render(mMat,Camera);
	it = m_pMeshlinkList.begin();
	ie = m_pMeshlinkList.end();
	for(;it != ie;it++) *it->second.m_pOrg = it->second.m_Org;
}
void lib3D::NODE_ENTITY::RenderBoundbox(mathHelper::MTX34& mMat,mathHelper::VEC3& vColor)
{
	if(m_pProxyModel) m_pProxyModel->RenderBoundbox(mMat,vColor);
}
bool lib3D::NODE_ENTITY::CheckBoundbox(mathHelper::MTX34& mMat,mathHelper::VEC3& vPos,mathHelper::VEC3& vDir,float& Depth)
{
	if(m_pProxyModel) 
	{
		return m_pProxyModel->CheckBoundbox(mMat,vPos,vDir,Depth);
	}
	return false;
}

SCENENODE::SCENENODE()
{
	m_iID = -1;
	m_Entity = 0;
	m_MouseState = MOUSEOVER;
	m_vScaling = mathHelper::VEC3(1,1,1);
	m_qRotation = mathHelper::QUAT(0,0,0,1);
}

SCENENODE::SCENENODE(lib3D::NODE_ENTITY* Entity)
{
	m_MouseState = MOUSEOVER;
	SetEntity(Entity);
	m_iID = -1;
	m_vScaling = mathHelper::VEC3(1,1,1);
}

SCENENODE::~SCENENODE()
{
}

void SCENENODE::SetUnikID(int iID)
{
	m_iID = iID;
}

void SCENENODE::GetMatrix(mathHelper::MTX34& mat)
{
	mathHelper::MTX34 matR;
	mathHelper::MTX34 matS;
	mathHelper::MTX34Identity(&mat);
	mathHelper::MTX34Identity(&matS);
	mathHelper::MTX34Identity(&matR);
	mathHelper::MTX34Scale(&matS,&m_vScaling,&matS);
	mathHelper::QUATToMTX34(&matR,&m_qRotation);
	mathHelper::MTX34Mult(&mat,&matR,&matS);
	mathHelper::MTX34Trans(&mat,&m_vPosition,&mat);
}

void SCENENODE::Render(mathHelper::MTX34& mMat,lib3D::CAMERA* Camera)
{
	if(m_Entity)
	{
		mathHelper::MTX34 mat;
		GetMatrix(mat);
		mathHelper::MTX34Mult(&mat,&mMat,&mat);
		m_Entity->Render(mat,Camera);
	}
}
void SCENENODE::RenderBoundbox(mathHelper::MTX34& mMat,mathHelper::VEC3& vColor)
{
	if(m_Entity)
	{
		mathHelper::MTX34 mat;
		GetMatrix(mat);
		mathHelper::MTX34Mult(&mat,&mMat,&mat);
		mathHelper::VEC3 vColor = mathHelper::VEC3(1,1,1);
		switch(m_MouseState)
		{
		case NONE:vColor = mathHelper::VEC3(1,1,1);break;
		case MOUSEOVER:vColor = mathHelper::VEC3(1,1,0);break;
		case SELECTED:vColor = mathHelper::VEC3(0,1,0);break;
		}
		m_Entity->RenderBoundbox(mat,vColor);
	}
}
void SCENENODE::Process(float TimeDelta)
{
//	m_vRotation.y += 0.1f * TimeDelta;
}
bool SCENENODE::GetBoundBox(lib3D::BOUNDBOX& Box)
{
	if(m_Entity->GetProxyModel())
	{
		Box.vMax = mathHelper::VEC3(-10000000000000.0f,-10000000000000.0f,-10000000000000.0f);
		Box.vMin = mathHelper::VEC3(10000000000000.0f,10000000000000.0f,10000000000000.0f);
		mathHelper::MTX34 mat;
		mathHelper::MTX34Identity(&mat);
//		GetMatrix(mat);
		m_Entity->GetProxyModel()->GetBoundBox(Box,mat);
		return true;
	}
	return false;
}

bool SCENENODE::CheckBoundbox(mathHelper::VEC3& vPos,mathHelper::VEC3& vDir,float& Depth)
{
	if(m_Entity)
	{
		m_MouseState = NONE;
		mathHelper::MTX34 mat;
		GetMatrix(mat);
		return m_Entity->CheckBoundbox(mat,vPos,vDir,Depth);
	}
	return false;
}
void SCENENODE::SetEntity(lib3D::NODE_ENTITY* Entity) 
{
	m_Entity = Entity;
}
void SCENENODE::SetPosition(mathHelper::VEC3 vPos) 
{
	m_vPosition = vPos;
}
void SCENENODE::SetRotation(mathHelper::QUAT qRot) 
{
//	mathHelper::MTX34 m;
//	mathHelper::MTX34RotY(&m,0.5f);
//	mathHelper::MTX34ToQUAT(&m_qRotation,&m);
	m_qRotation = qRot;
}
void SCENENODE::SetScaling(mathHelper::VEC3 vSca) 
{
	m_vScaling = vSca;
}
mathHelper::VEC3 SCENENODE::GetPosition() 
{
	return m_vPosition;
}
mathHelper::QUAT SCENENODE::GetRotation() 
{
	return m_qRotation;
}
mathHelper::VEC3 SCENENODE::GetScaling() 
{
	return m_vScaling;
}

SCENENODE* SCENENODE::GetPtr() 
{
	return this;
}
lib3D::NODE_ENTITY* SCENENODE::GetEntity() 
{
	return m_Entity;
}

void SCENENODE::SetMouseState(MOUSESTATE s) 
{
	m_MouseState = s;
}
SCENENODE::MOUSESTATE SCENENODE::GetMouseState() 
{
	return m_MouseState;
}
