#include "Node.h"

using namespace lib3D;

NODE::NODE()
{
	m_Nodes.clear();
	m_MeshLinks.clear();
	m_vPosition = mathHelper::VEC3(0,0,0);
	m_vRotation = mathHelper::VEC3(0,0,0);
}

NODE::~NODE()
{
	while(!m_Nodes.empty())
	{
		NODE n = *m_Nodes.begin();
		m_Nodes.pop_front();
	}
}

void NODE::AddMeshLink(MESHLINK MeshLink)
{
	std::list<MESHLINK>::iterator it = m_MeshLinks.begin();
	std::list<MESHLINK>::iterator ie = m_MeshLinks.end();
	for(;it != ie;it++)
	{
		MESHLINK ml = *it;
		if(ml.m_pMeshLink == MeshLink.m_pMeshLink)
			return;
	}
	m_MeshLinks.push_back(MeshLink);
}

void NODE::Render(mathHelper::MTX34 mMat,lib3D::CAMERA* Camera)
{
	mathHelper::MTX34 mat,rot;
	mathHelper::MTX34Identity(&mat);
	mathHelper::MTX34RotXYZDeg(&mat,m_vRotation.x,m_vRotation.y,m_vRotation.z);
	mathHelper::MTX34Trans(&mat,&mathHelper::VEC3(m_vPosition.x,m_vPosition.y,m_vPosition.z),&mat);
	mathHelper::MTX34Mult(&mat,&mMat,&mat);

	std::list<MESHLINK>::iterator it_me = m_MeshLinks.begin();
	std::list<MESHLINK>::iterator ie_me = m_MeshLinks.end();
	for(;it_me != ie_me;it_me++)
	{
		if(it_me->m_pMeshLink)
			it_me->m_pMeshLink->Render(mat,it_me->m_Tex,it_me->m_Norm,it_me->m_Incan,it_me->m_bWireframe,it_me->m_ShaderProgram,Camera);
	}
	std::list<NODE>::iterator it_no = m_Nodes.begin();
	std::list<NODE>::iterator ie_no = m_Nodes.end();
	for(;it_no != ie_no;it_no++)
	{
		it_no->Render(mat,Camera);
	}
}

bool NODE::GetBoundBox(BOUNDBOX& Box,mathHelper::MTX34& mMat)
{
	bool bHasBox = false;

	mathHelper::MTX34 mat,rot;
	mathHelper::MTX34Identity(&mat);
	mathHelper::MTX34RotXYZDeg(&mat,m_vRotation.x,m_vRotation.y,m_vRotation.z);
	mathHelper::MTX34Trans(&mat,&mathHelper::VEC3(m_vPosition.x,m_vPosition.y,m_vPosition.z),&mat);
	mathHelper::MTX34Mult(&mat,&mMat,&mat);

	std::list<MESHLINK>::iterator it_me = m_MeshLinks.begin();
	std::list<MESHLINK>::iterator ie_me = m_MeshLinks.end();
	for(;it_me != ie_me;it_me++)
	{
		if(it_me->m_pMeshLink)
		{
			BOUNDBOX BoxTest;
			if(it_me->m_pMeshLink->GetBoundBox(BoxTest,mat))
			{
				if(BoxTest.vMin.x < Box.vMin.x) Box.vMin.x = BoxTest.vMin.x;
				if(BoxTest.vMin.y < Box.vMin.y) Box.vMin.y = BoxTest.vMin.y;
				if(BoxTest.vMin.z < Box.vMin.z) Box.vMin.z = BoxTest.vMin.z;
				if(BoxTest.vMax.x > Box.vMax.x) Box.vMax.x = BoxTest.vMax.x;
				if(BoxTest.vMax.y > Box.vMax.y) Box.vMax.y = BoxTest.vMax.y;
				if(BoxTest.vMax.z > Box.vMax.z) Box.vMax.z = BoxTest.vMax.z;
				bHasBox = true;
			}
		}
	}

	std::list<NODE>::iterator it_no = m_Nodes.begin();
	std::list<NODE>::iterator ie_no = m_Nodes.end();
	for(;it_no != ie_no;it_no++)
	{
		bool b = it_no->GetBoundBox(Box,mat);
		if(b) bHasBox = true;
	}

	return bHasBox;
}

void NODE::SetWireframe(bool bWireframe)
{
	std::list<MESHLINK>::iterator it_me = m_MeshLinks.begin();
	std::list<MESHLINK>::iterator ie_me = m_MeshLinks.end();
	for(;it_me != ie_me;it_me++)
	{
		if(it_me->m_pMeshLink)
			it_me->m_bWireframe = bWireframe;
	}
	std::list<NODE>::iterator it_no = m_Nodes.begin();
	std::list<NODE>::iterator ie_no = m_Nodes.end();
	for(;it_no != ie_no;it_no++)
	{
		it_no->SetWireframe(bWireframe);
	}
}

