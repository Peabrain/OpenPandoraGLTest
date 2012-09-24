#include "Model.h"
#include "ConvertDAE.h"

using namespace lib3D;

MODELLIBRARY lib3D::ModelLibrary;

MODEL::MODEL()
{
	m_Meshes.clear();
}

MODEL::~MODEL()
{
	std::map<std::string,MESH*>::iterator it = m_Meshes.begin();
	std::map<std::string,MESH*>::iterator ie = m_Meshes.end();
	for(;it != ie;it++)
	{
		MESH *m = it->second;
		delete m;
	}
	m_Meshes.clear();
}

MESH* MODEL::AddMesh(MESH *Mesh)
{
	MESH* ret = 0;
	std::map<std::string,MESH*>::iterator it = m_Meshes.find(Mesh->GetName());
	if(it != m_Meshes.end())
	{
		ret = it->second;
		it->second = Mesh;
	}
	else
		m_Meshes[Mesh->GetName()] = Mesh;
	return ret;
}

MESH* MODEL::GetMesh(std::string& sName)
{
	std::map<std::string,MESH*>::const_iterator it = m_Meshes.find(sName);
	if(it != m_Meshes.end())
	{
		return it->second;
	}
	return 0;
}
void MODEL::PreRender()
{
	std::map<std::string,MESH*>::iterator it = m_Meshes.begin();
	std::map<std::string,MESH*>::iterator ie = m_Meshes.end();
	for(;it != ie;it++)
	{
		it->second->PreRender();
	}
}
void MODEL::Render(mathHelper::MTX34 mMat,lib3D::CAMERA* Camera)
{
	m_RootNode.Render(mMat,Camera);
}

bool MODEL::CheckBoundbox(mathHelper::MTX34& mMat,mathHelper::VEC3& vPos,mathHelper::VEC3& vDir,float& Depth)
{
	bool bHit = false;
	BOUNDBOX Box;
	Box.vMax = mathHelper::VEC3(-10000000000000.0f,-10000000000000.0f,-10000000000000.0f);
	Box.vMin = mathHelper::VEC3(10000000000000.0f,10000000000000.0f,10000000000000.0f);
	mathHelper::MTX34 Mat;
	mathHelper::MTX34Identity(&Mat);
	if(GetBoundBox(Box,Mat))
	{
		mathHelper::MTX34 mMatInv;
		mathHelper::MTX34Inv(&mMatInv,&mMat);
		mathHelper::VEC3 p = vPos;
		mathHelper::VEC3 d = p + vDir;
		mathHelper::VEC3TransformCoord(&p,&mMatInv,&p);
		mathHelper::VEC3TransformCoord(&d,&mMatInv,&d);
		d = d - p;
		mathHelper::VEC3Normalize(&d,&d);
		if(d.z != 0)
		{
			float fDistXYMin = (Box.vMin.z - p.z) / d.z;
			if(fDistXYMin > 0)
			{
				mathHelper::VEC3 vXYMin = p + d * fDistXYMin;
				if(vXYMin.x >= Box.vMin.x && vXYMin.x < Box.vMax.x && vXYMin.y >= Box.vMin.y && vXYMin.y < Box.vMax.y)
				{
					if((vXYMin - p).LenSq() < Depth) Depth = (vXYMin - p).LenSq();
					bHit = true;
				}
			}
			float fDistXYMax = (Box.vMax.z - p.z) / d.z;
			if(fDistXYMax > 0)
			{
				mathHelper::VEC3 vXYMax = p + d * fDistXYMax;
				if(vXYMax.x >= Box.vMin.x && vXYMax.x < Box.vMax.x && vXYMax.y >= Box.vMin.y && vXYMax.y < Box.vMax.y)
				{
					if((vXYMax - p).LenSq() < Depth) Depth = (vXYMax - p).LenSq();
					bHit = true;
				}
			}
		}
		if(d.x != 0)
		{
			float fDistYZMin = (Box.vMin.x - p.x) / d.x;
			if(fDistYZMin > 0)
			{
				mathHelper::VEC3 vYZMin = p + d * fDistYZMin;
				if(vYZMin.y >= Box.vMin.y && vYZMin.y < Box.vMax.y && vYZMin.z >= Box.vMin.z && vYZMin.z < Box.vMax.z)
				{
					if((vYZMin - p).LenSq() < Depth) Depth = (vYZMin - p).LenSq();
					bHit = true;
				}
			}
			float fDistYZMax = (Box.vMax.x - p.x) / d.x;
			if(fDistYZMax > 0)
			{
				mathHelper::VEC3 vYZMax = p + d * fDistYZMax;
				if(vYZMax.y >= Box.vMin.y && vYZMax.y < Box.vMax.y && vYZMax.z >= Box.vMin.z && vYZMax.z < Box.vMax.z)
				{
					if((vYZMax - p).LenSq() < Depth) Depth = (vYZMax - p).LenSq();
					bHit = true;
				}
			}
		}
		if(d.y != 0)
		{
			float fDistXZMin = (Box.vMin.y - p.y) / d.y;
			if(fDistXZMin > 0)
			{
				mathHelper::VEC3 vXZMin = p + d * fDistXZMin;
				if(vXZMin.x >= Box.vMin.x && vXZMin.x < Box.vMax.x && vXZMin.z >= Box.vMin.z && vXZMin.z < Box.vMax.z)
				{
					if((vXZMin - p).LenSq() < Depth) Depth = (vXZMin - p).LenSq();
					bHit = true;
				}
			}
			float fDistXZMax = (Box.vMax.y - p.y) / d.y;
			if(fDistXZMax > 0)
			{
				mathHelper::VEC3 vXZMax = p + d * fDistXZMax;
				if(vXZMax.x >= Box.vMin.x && vXZMax.x < Box.vMax.x && vXZMax.z >= Box.vMin.z && vXZMax.z < Box.vMax.z)
				{
					if((vXZMax - p).LenSq() < Depth) Depth = (vXZMax - p).LenSq();
					bHit = true;
				}
			}
		}
	}

	return bHit;
}

void MODEL::RenderBoundbox(mathHelper::MTX34& mMat,mathHelper::VEC3& vColor)
{
/*	BOUNDBOX Box;
	Box.vMax = mathHelper::VEC3(-10000000000000.0f,-10000000000000.0f,-10000000000000.0f);
	Box.vMin = mathHelper::VEC3(10000000000000.0f,10000000000000.0f,10000000000000.0f);
	mathHelper::MTX34 Mat;
	mathHelper::MTX34Identity(&Mat);
	if(GetBoundBox(Box,Mat))
	{
		glPushMatrix();
		float m[16];
		for(int i = 0;i < 16;i++) m[i] = 0;
		for(int i = 0;i < 3;i++) 
			for(int j = 0;j < 4;j++) 
				m[j * 4 + i] = mMat.m[i][j];
		m[15] = 1;
		glLoadMatrixf(m);

		glDisable(GL_CULL_FACE);
		glPolygonMode (GL_FRONT,GL_LINE);
		glPolygonMode (GL_BACK,GL_LINE);

		glBegin(GL_QUADS);
		glColor3f(vColor.x,vColor.y,vColor.z);
		glVertex3f(Box.vMin.x,Box.vMin.y,Box.vMin.z);
		glVertex3f(Box.vMin.x,Box.vMin.y,Box.vMax.z);
		glVertex3f(Box.vMax.x,Box.vMin.y,Box.vMax.z);
		glVertex3f(Box.vMax.x,Box.vMin.y,Box.vMin.z);

		glVertex3f(Box.vMin.x,Box.vMax.y,Box.vMin.z);
		glVertex3f(Box.vMin.x,Box.vMax.y,Box.vMax.z);
		glVertex3f(Box.vMax.x,Box.vMax.y,Box.vMax.z);
		glVertex3f(Box.vMax.x,Box.vMax.y,Box.vMin.z);

		glVertex3f(Box.vMin.x,Box.vMin.y,Box.vMax.z);
		glVertex3f(Box.vMin.x,Box.vMax.y,Box.vMax.z);
		glVertex3f(Box.vMax.x,Box.vMax.y,Box.vMax.z);
		glVertex3f(Box.vMax.x,Box.vMin.y,Box.vMax.z);

		glVertex3f(Box.vMin.x,Box.vMin.y,Box.vMin.z);
		glVertex3f(Box.vMin.x,Box.vMax.y,Box.vMin.z);
		glVertex3f(Box.vMax.x,Box.vMax.y,Box.vMin.z);
		glVertex3f(Box.vMax.x,Box.vMin.y,Box.vMin.z);

		glColor3f(1,1,1);
		glEnd();

		glPopMatrix();

		glPolygonMode (GL_FRONT,GL_FILL);
		glPolygonMode (GL_BACK,GL_FILL);
		glEnable(GL_CULL_FACE);
	}
*/
}
void MODEL::PostRender()
{
	std::map<std::string,MESH*>::iterator it = m_Meshes.begin();
	std::map<std::string,MESH*>::iterator ie = m_Meshes.end();
	for(;it != ie;it++)
	{
		it->second->PostRender();
	}
}
bool MODEL::GetBoundBox(BOUNDBOX& Box,mathHelper::MTX34 mMat)
{
	if(m_RootNode.GetBoundBox(Box,mMat))
		return true;
	return false;
}

void MODELLIBRARY::PreRender()
{
	std::map<std::string,MODEL*>::const_iterator it = m_pModels.begin();
	std::map<std::string,MODEL*>::const_iterator ie = m_pModels.end();
	for(;it != ie;it++)
	{
		it->second->PreRender();
	}
}

void MODELLIBRARY::PostRender()
{
	std::map<std::string,MODEL*>::const_iterator it = m_pModels.begin();
	std::map<std::string,MODEL*>::const_iterator ie = m_pModels.end();
	for(;it != ie;it++)
	{
		it->second->PostRender();
	}
}

void MODELLIBRARY::GetList(std::list<std::string>& List)
{
	std::map<std::string,MODEL*>::const_iterator it = m_pModels.begin();
	std::map<std::string,MODEL*>::const_iterator ie = m_pModels.end();
	for(;it != ie;it++)
	{
		List.push_back(it->first);
	}
}

bool MODELLIBRARY::LoadModel(const char* szPath)
{
	std::map<std::string,MODEL*>::const_iterator it = m_pModels.find(szPath);
	if(it == m_pModels.end())
	{
		CONVERT_DAE* Convert = new CONVERT_DAE();
		MODEL* pModel = Convert->Load(szPath);
		delete Convert;
		if(pModel == 0) 
			return false;
		std::string Path(szPath);
		int iPos = Path.find_first_of("Data/Models/");
		if(iPos != -1)
		{
			char t[1024] = {0};
			Path.copy(t,Path.size() - sizeof("Data/Models"),sizeof("Data/Models"));
			Path = std::string(t);
		}
		m_pModels[Path] = pModel;
	}
	return true;
}

MODEL* MODELLIBRARY::GetModelPointer(std::string sPath)
{
	std::map<std::string,MODEL*>::const_iterator it = m_pModels.find(sPath);
	if(it != m_pModels.end())
	{
		return it->second;
	}
	return 0;
}
