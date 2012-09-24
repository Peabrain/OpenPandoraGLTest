#include "Mesh.h"
#ifdef WIN32
#include <Windows.h>
#endif
#include <GLES2/gl2.h>
#include "mathHelper.h"

using namespace lib3D;

MESH::MESH(std::string sName)
{
	m_sName = sName;
}

MESH::~MESH()
{
	std::map<VERTEXBUFFER::TYPE,VERTEXBUFFER*>::iterator it = m_pBufferList.begin();
	std::map<VERTEXBUFFER::TYPE,VERTEXBUFFER*>::iterator ie = m_pBufferList.end();
	for(;it != ie;it++)
	{
		delete it->second;
	}
	m_pBufferList.clear();
	if(m_pIndexBuffer) delete m_pIndexBuffer;
}

void MESH::PreRender()
{
	m_Instances.clear();
}

void MESH::Render(mathHelper::MTX34 mMat,TEXTURE* pTexDiffuse,TEXTURE* pTexNormal,TEXTURE* pTexIncandescence,bool bWireframe,GLuint ShaderProgram,lib3D::CAMERA* Camera)
{
	m_Instances.push_back(INSTANCE(mMat,pTexDiffuse,pTexNormal,pTexIncandescence,bWireframe,ShaderProgram,Camera));
}

bool MESH::GetBoundBox(BOUNDBOX& Box,mathHelper::MTX34& mMat)
{
	std::map<VERTEXBUFFER::TYPE,VERTEXBUFFER*>::iterator itver = m_pBufferList.find(VERTEXBUFFER::VERTEX);
	if(itver != m_pBufferList.end())
	{
		BOUNDBOX* pBox = itver->second->GetBox();
		if(pBox)
		{
			mathHelper::VEC3TransformCoord(&Box.vMin,&mMat,&pBox->vMin);
			mathHelper::VEC3TransformCoord(&Box.vMax,&mMat,&pBox->vMax);
			return true;
		}
	}
	return false;
}

void MESH::PostRender()
{
	if(m_pIndexBuffer)
	{
		std::list<INSTANCE>::iterator iti = m_Instances.begin();
		std::list<INSTANCE>::iterator iei = m_Instances.end();

		if(iti == iei) return;
		glUseProgram(iti->m_ShaderProgram);

		ShaderLib::ShaderDef Def;
		g_ShaderLib.GetShaderDefs(iti->m_ShaderProgram,Def);

		mathHelper::VEC4 LightDir(1,1,1,0);
		glUniform4fv( Def.m_Light0_Dir, 1,(GLfloat*)LightDir);

		glUniform1i(Def.m_DiffuseTex, 0);
		glUniform1i(Def.m_NormalTex, 1);
		glUniform1i(Def.m_IncandescenceTex, 2);

		std::map<VERTEXBUFFER::TYPE,VERTEXBUFFER*>::iterator it = m_pBufferList.begin();
		std::map<VERTEXBUFFER::TYPE,VERTEXBUFFER*>::iterator ie = m_pBufferList.end();
		for(;it != ie;it++)
		{
			VERTEXBUFFER *vb = it->second;
			if(vb) 
			{
				switch(vb->GetType())
				{
				case lib3D::VERTEXBUFFER::VERTEX: vb->Activate(Def.m_Vertex);break;
				case lib3D::VERTEXBUFFER::COLOR: vb->Activate(Def.m_Color);break;
				case lib3D::VERTEXBUFFER::NORMAL: vb->Activate(Def.m_Normal);break;
				case lib3D::VERTEXBUFFER::BINORMAL: vb->Activate(Def.m_Binormal);break;
				case lib3D::VERTEXBUFFER::TANGENT: vb->Activate(Def.m_Tangent);break;
				case lib3D::VERTEXBUFFER::TEXTURE0: vb->Activate(Def.m_Texture0);break;
				}		
			}
		}

		for(;iti != iei;iti++)
		{
			INSTANCE Inst = *iti;
			if(Inst.m_pTexDiffuse) Inst.m_pTexDiffuse->Activate(lib3D::TEXTURE::COLOR);
			if(Inst.m_pTexNormal) Inst.m_pTexNormal->Activate(lib3D::TEXTURE::NORMAL);
			if(Inst.m_pTexIncandescence) Inst.m_pTexIncandescence->Activate(lib3D::TEXTURE::INCANDESCENCE);

			mathHelper::MTX44 ma;
			mathHelper::MTX44Transpose(&ma,&Inst.m_mMat);
			glUniformMatrix4fv( Def.m_ObjMtx, 1, GL_FALSE,ma.a);
			mathHelper::MTX34 maInv;
			mathHelper::MTX34Inv(&maInv,&Inst.m_mMat);
			mathHelper::MTX44Transpose(&ma,&maInv);
			glUniformMatrix4fv( Def.m_ObjMtxInv, 1, GL_FALSE,ma.a);

			Inst.m_Camera->Activate(Def.m_ProjViewObjMtx,Def.m_CameraMtx);

			m_pIndexBuffer->Draw();

			if(Inst.m_pTexDiffuse) Inst.m_pTexDiffuse->Deactivate(lib3D::TEXTURE::COLOR);
			if(Inst.m_pTexNormal) Inst.m_pTexNormal->Deactivate(lib3D::TEXTURE::NORMAL);
			if(Inst.m_pTexIncandescence) Inst.m_pTexIncandescence->Deactivate(lib3D::TEXTURE::INCANDESCENCE);
		}

		it = m_pBufferList.begin();
		ie = m_pBufferList.end();
		for(;it != ie;it++)
		{
			VERTEXBUFFER *vb = it->second;
			if(vb) vb->Deactivate();
		}
	}
}
VERTEXBUFFER* MESH::GetVertexBuffer(VERTEXBUFFER::TYPE t)
{
	std::map<VERTEXBUFFER::TYPE,VERTEXBUFFER*>::iterator it = m_pBufferList.find(t);
	if(it == m_pBufferList.end()) return 0;
	return it->second;
}

bool MESH::Raycast(mathHelper::MTX34& mOrg,mathHelper::MTX34& mOrgInv,int& iTriangleID,mathHelper::VEC3& vIntersect)
{
/*	mathHelper::VEC3 vOrg = mathHelper::VEC3(0,0,0);
	mathHelper::VEC3 vDir = mathHelper::VEC3(0,0,-1);
	bool bRayCast = false;
	VERTEXBUFFER* pBuffer = GetVertexBuffer(VERTEXBUFFER::VERTEX);
	if(pBuffer)
	{
		mathHelper::VEC3* vBuffer = pBuffer->GetTransformed(mOrg);
		if(vBuffer)
		{
			if(m_pIndexBuffer)
			{
				GLushort* usIndexBuffer = m_pIndexBuffer->GetBuffer();
				if(usIndexBuffer)
				{
					unsigned int* uiVertexBitBufferX = new unsigned int[pBuffer->GetElements() / 32 + 1];
					unsigned int* uiVertexBitBufferY = new unsigned int[pBuffer->GetElements() / 32 + 1];
					memset(uiVertexBitBufferX,0,sizeof(unsigned int) * (pBuffer->GetElements() / 32 + 1));
					memset(uiVertexBitBufferY,0,sizeof(unsigned int) * (pBuffer->GetElements() / 32 + 1));
					for(int i = 0;i < pBuffer->GetElements();i++)
					{
						if(vBuffer[i].x < 0.0f) uiVertexBitBufferX[i / 32] |= 1 << (i % 32);
						if(vBuffer[i].y < 0.0f) uiVertexBitBufferY[i / 32] |= 1 << (i % 32);
					}

					float fDepth = 1000000000.0f;
					unsigned int Len = m_pIndexBuffer->GetElements();
					for(int i = 0;i < Len / 3;i++)
					{
						GLushort usP0 = usIndexBuffer[i * 3 + 0];
						GLushort usP1 = usIndexBuffer[i * 3 + 1];
						GLushort usP2 = usIndexBuffer[i * 3 + 2];
						unsigned int b = 0;
						if(uiVertexBitBufferX[usP0 / 32] & 1 << (usP0 % 32)) b |= 1;
						if(uiVertexBitBufferX[usP1 / 32] & 1 << (usP1 % 32)) b |= 2;
						if(uiVertexBitBufferX[usP2 / 32] & 1 << (usP2 % 32)) b |= 4;
						if(b != 0 && b != 7)
						{
							unsigned int a = 0;
							if(uiVertexBitBufferY[usP0 / 32] & 1 << (usP0 % 32)) a |= 1;
							if(uiVertexBitBufferY[usP1 / 32] & 1 << (usP1 % 32)) a |= 2;
							if(uiVertexBitBufferY[usP2 / 32] & 1 << (usP2 % 32)) a |= 4;
							if(a != 0 && a != 7)
							{
								mathHelper::VEC3 vI;
								float fDist;
								if(Intersect_RayTriangle(vOrg,vDir,vBuffer[usP0],vBuffer[usP1],vBuffer[usP2],vI,fDist) == 1)
								{
									if(fDist < fDepth)
									{
										fDepth = fDist;
										bRayCast = true;
										iTriangleID = i;
										vIntersect = vI;
									}
								}
							}
						}
					}

					delete [] uiVertexBitBufferX;
					delete [] uiVertexBitBufferY;
				}
			}
			free(vBuffer);
		}
	}
	if(bRayCast)
	{
		mathHelper::VEC3TransformCoord(&vIntersect,&mOrgInv,&vIntersect);
	}
	return bRayCast;
*/
	return false;
}

int MESH::Intersect_RayTriangle( mathHelper::VEC3& vOrg, mathHelper::VEC3& vDir, mathHelper::VEC3& V0, mathHelper::VEC3& V1, mathHelper::VEC3& V2, mathHelper::VEC3& vIntersect,float& fDist )
{
    mathHelper::VEC3    u, v, n;             // triangle vectors
    mathHelper::VEC3    dir, w0, w;          // ray vectors
    float     r, a, b;             // params to calc ray-plane intersect

    // get triangle edge vectors and plane normal
    u = V1 - V0;
    v = V2 - V0;
    mathHelper::VEC3Cross(&n,&u,&v);             // cross product
    if (n == mathHelper::VEC3(0,0,0))            // triangle is degenerate
        return -1;                 // do not deal with this case

    w0 = vOrg - V0;
	a = -mathHelper::VEC3Dot(&n,&w0);
    b = mathHelper::VEC3Dot(&n,&vDir);
    if (fabs(b) < 0.000000001f) 
	{     // ray is parallel to triangle plane
        if (a == 0)                // ray lies in triangle plane
            return 2;
        else return 0;             // ray disjoint from plane
    }

    // get intersect point of ray with triangle plane
    r = a / b;
    if (r < 0.0)                   // ray goes away from triangle
        return 0;                  // => no intersect
    // for a segment, also test if (r > 1.0) => no intersect

	fDist = r;
    vIntersect = vOrg + r * vDir;           // intersect point of ray and plane

    // is I inside T?
    float    uu, uv, vv, wu, wv, D;
    uu = mathHelper::VEC3Dot(&u,&u);
    uv = mathHelper::VEC3Dot(&u,&v);
    vv = mathHelper::VEC3Dot(&v,&v);
    w = vIntersect - V0;
    wu = mathHelper::VEC3Dot(&w,&u);
    wv = mathHelper::VEC3Dot(&w,&v);
    D = uv * uv - uu * vv;

    // get and test parametric coords
    float s, t;
    s = (uv * wv - vv * wu) / D;
    if (s < 0.0 || s > 1.0)        // I is outside T
        return 0;
    t = (uv * wu - uu * wv) / D;
    if (t < 0.0 || (s + t) > 1.0)  // I is outside T
        return 0;

    return 1;                      // I is in T
}



bool MESH::GetTriangle(int iTriangleID,mathHelper::VEC3& vPos0,mathHelper::VEC3& vPos1,mathHelper::VEC3& vPos2)
{
/*	bool bRayCast = false;
	VERTEXBUFFER* pBuffer = GetVertexBuffer(VERTEXBUFFER::VERTEX);
	if(pBuffer)
	{
		if(m_pIndexBuffer)
		{
			GLushort* usIndexBuffer = m_pIndexBuffer->GetBuffer();
			if(usIndexBuffer)
			{
				unsigned int Len = m_pIndexBuffer->GetElements();
				if(iTriangleID < Len)
				{
					if(!pBuffer->GetVertex(usIndexBuffer[iTriangleID * 3 + 0],vPos0)) return false;
					if(!pBuffer->GetVertex(usIndexBuffer[iTriangleID * 3 + 1],vPos1)) return false;
					if(!pBuffer->GetVertex(usIndexBuffer[iTriangleID * 3 + 2],vPos2)) return false;
					return true;
				}
			}
		}
	}
*/
	return false;
}
