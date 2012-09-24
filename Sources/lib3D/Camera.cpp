#ifdef WIN32
#include <Windows.h>
#endif
//#include "lib3D.h"
#include "Camera.h"
#include "mathHelper.h"
#include "EngWindow.h"

using namespace lib3D;

void CAMERA::Activate(GLuint uiPerspViewMtx,GLuint uiCamMtx)
{
	int iStartX = (int)(g_MyWindow->GetWidth() * m_Parameter.m_Viewport.m_iStartX);
	int iStartY = (int)(g_MyWindow->GetHeight() * m_Parameter.m_Viewport.m_iStartY);
	int iSizeX = (int)(g_MyWindow->GetWidth() * (m_Parameter.m_Viewport.m_iEndX - m_Parameter.m_Viewport.m_iStartX));
	int iSizeY = (int)(g_MyWindow->GetHeight() * (m_Parameter.m_Viewport.m_iEndY - m_Parameter.m_Viewport.m_iStartY));

	glViewport(iStartX, iStartY, iSizeX, iSizeY); // Set our viewport to the size of our window  
	glScissor(iStartX, iStartY, iSizeX, iSizeY);
	switch(m_Parameter.m_Type)
	{
	case TYPE_PERS:
		{
			GetPersMatrix();
			glUniformMatrix4fv( uiPerspViewMtx, 1, GL_FALSE, ProjectionMatrix.a);
			GetObjMatrix();
			mathHelper::MTX44 mat;
			mathHelper::MTX44Transpose(&mat,&ObjMatrix);
			glUniformMatrix4fv( uiCamMtx, 1, GL_FALSE, mat.a);
		}break;
	case TYPE_ORTHO:
		{
//			glMatrixMode(GL_PROJECTION); // Switch to the projection matrix so that we can manipulate how our scene is viewed  
//			glLoadIdentity(); // Reset the projection matrix to the identity matrix so that we don't get any artifacts (cleaning up)  
//			gluOrtho2D(iStartX, iStartX + iSizeX, iStartY + iSizeY, iStartY);
		}break;
	}

//	glGetFloatv(GL_PROJECTION_MATRIX, m_mProjection);
//	glMatrixMode(GL_MODELVIEW); // Switch back to the model view matrix, so that we can start drawing shapes correctly  

//	glLoadIdentity();									// Reset The Current Modelview Matrix
}

void CAMERA::Process(float TimeDelta)
{
	CalculateMatrices();
/*
	std::map<unsigned char,bool>::iterator it;
	mathHelper::VEC3 v = mathHelper::VEC3(0,0,-50);
	mathHelper::VEC3 vl = mathHelper::VEC3(-50,0,0);

	it = m_KeyMap.find('w');if(it != m_KeyMap.end()) m_vPosition += v * TimeDelta;
	it = m_KeyMap.find('s');if(it != m_KeyMap.end()) m_vPosition -= v * TimeDelta;
	it = m_KeyMap.find('a');if(it != m_KeyMap.end()) m_vPosition += vl * TimeDelta;
	it = m_KeyMap.find('d');if(it != m_KeyMap.end()) m_vPosition -= vl * TimeDelta;
*/
}  
mathHelper::MTX44* CAMERA::GetPersMatrix()
{
	return &ProjectionMatrix;
}
mathHelper::MTX34* CAMERA::GetObjMatrix()
{
	return &ObjMatrix;
}
void CAMERA::CalculateMatrices()
{
	int iStartX = (int)(g_MyWindow->GetWidth() * m_Parameter.m_Viewport.m_iStartX);
	int iStartY = (int)(g_MyWindow->GetHeight() * m_Parameter.m_Viewport.m_iStartY);
	int iSizeX = (int)(g_MyWindow->GetWidth() * (m_Parameter.m_Viewport.m_iEndX - m_Parameter.m_Viewport.m_iStartX));
	int iSizeY = (int)(g_MyWindow->GetHeight() * (m_Parameter.m_Viewport.m_iEndY - m_Parameter.m_Viewport.m_iStartY));
	BuildPerspProjMat(ProjectionMatrix, m_Parameter.m_fFov, (GLfloat)iSizeX / (GLfloat)iSizeY,0.1, 500);

	mathHelper::Quaternion qZ = mathHelper::EulerToQuat(m_fPitch,m_fRoll,m_fYaw);
	mathHelper::QUAT q = mathHelper::QUAT(qZ.x,qZ.y,qZ.z,qZ.w);

	mathHelper::MTX34 mat,matRP,matRR,matR;
	mathHelper::MTX34Identity(&mat);
	mathHelper::MTX34Identity(&matR);
	mathHelper::MTX34RotX(&matRP,m_fPitch);
	mathHelper::MTX34RotZ(&matRR,m_fRoll);
	mathHelper::MTX34Mult(&matR,&matR,&matRP);
	mathHelper::MTX34Mult(&matR,&matR,&matRR);

//	mathHelper::MTX34RotXYZRad(&matR,m_fPitch,0,m_fRoll);
	mathHelper::QUATToMTX34(&matR,&m_qRotation);
	mathHelper::MTX34Inv(&matR,&matR);
	mathHelper::MTX34Trans(&mat,&m_vPosition,&mat);
	mathHelper::MTX34Mult(&ObjMatrix,&matR,&mat);
}
void CAMERA::Roll(float c)
{
	mathHelper::quaternion_rotate_about_z(&m_qRotation,c);
}
void CAMERA::Pitch(float c)
{
	mathHelper::quaternion_rotate_about_x(&m_qRotation,c);
}
void CAMERA::SetRoll(float r) 
{
	m_fRoll = r;
}
void CAMERA::SetPitch(float r) 
{
	m_fPitch = r;
}
void CAMERA::SetYaw(float r) 
{
	m_fYaw = r;
}
mathHelper::VEC3 CAMERA::GetPosition()
{
	return m_vPosition;
}
void CAMERA::SetPosition(mathHelper::VEC3 v)
{
	m_vPosition = v;
}
void CAMERA::MouseMove(int button, int state,int x, int y)
{
/*	if(button == 2)
	{
		if(state == 0)
		{
			m_bRightButton = true;
			m_iRightbuttonStartX = x;
			m_iRightbuttonStartY = y;
		}
		else
		if(state == 1)
		{
			m_bRightButton = false;
		}
	}
	if(m_bRightButton)
	{
		m_vRotation.y += (float)(m_iRightbuttonStartX - x) * 200 * Library3D.getSystemTimerDelta();m_iRightbuttonStartX = x;
		m_vRotation.x += (float)(m_iRightbuttonStartY - y) * 200 * Library3D.getSystemTimerDelta();m_iRightbuttonStartY = y;
		if(m_vRotation.x < -89.0f) m_vRotation.x = -89.0f;
		if(m_vRotation.x > 89.0f) m_vRotation.x = 89.0f;
	}
*/
}

void CAMERA::BuildPerspProjMat(float *m, float fov, float aspect,float znear, float zfar)
{
#define PI_OVER_360 (3.14159 / 360.0)
	float xymax = znear * tan(fov * PI_OVER_360);
	float ymin = -xymax;
	float xmin = -xymax;

	float width = xymax - xmin;
	float height = xymax - ymin;

	float depth = zfar - znear;

	m[0]  = 2 * znear / width / aspect;
	m[1]  = 0;
	m[2]  = 0;
	m[3]  = 0;

	m[4]  = 0;
	m[5]  = 2 * znear / height;
	m[6]  = 0;
	m[7]  = 0;

	m[8]  = 0;
	m[9]  = 0;
	m[10] = -(zfar + znear) / depth;
	m[11] = -1;

	m[12] = 0;
	m[13] = 0;
	m[14] = -2 * (zfar * znear) / depth;
	m[15] = 0;
#undef PI_OVER
}

void CAMERA::BuildPerspProjMat1(float *matrix, float fov, float aspect,float znear, float zfar)
{
#define PI_OVER_360 (3.14159 / 360.0)
 
    // Some calculus before the formula.
    float size = znear * tan(fov * PI_OVER_360);
    float left = -size;
	float right = size;
	float bottom = -size;
	float top = size;
 
    // First Column
    matrix[0] = 2 * znear / (right - left) / aspect;
    matrix[1] = 0.0;
    matrix[2] = 0.0;
    matrix[3] = 0.0;
 
    // Second Column
    matrix[4] = 0.0;
    matrix[5] = 2 * znear / (top - bottom);
    matrix[6] = 0.0;
    matrix[7] = 0.0;
 
    // Third Column
    matrix[8] = (right + left) / (right - left);
    matrix[9] = (top + bottom) / (top - bottom);
    matrix[10] = -(zfar + znear) / (zfar - znear);
    matrix[11] = -1;//-1;
 
    // Fourth Column
    matrix[12] = 0.0;
    matrix[13] = 0.0;
    matrix[14] = -(2 * zfar * znear) / (zfar - znear);
    matrix[15] = 0;//0.0;
#undef PI_OVER
}
