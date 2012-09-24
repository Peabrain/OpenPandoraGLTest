#pragma once
#include "mathHelper.h"
#include <map>
#include <GLES2/gl2.h>

namespace lib3D
{
	class CAMERA
	{
	public:
		enum TYPE
		{
			TYPE_NONE,
			TYPE_ORTHO,
			TYPE_PERS
		};
		struct VIEWPORT
		{
			VIEWPORT() {m_iStartX = 0;m_iStartY = 0;m_iEndX = 0;m_iEndY = 0;}
			VIEWPORT(float iStartX,float iStartY,float iEndX,float iEndY) {m_iStartX = iStartX;m_iStartY = iStartY;m_iEndX = iEndX;m_iEndY = iEndY;}
			float m_iStartX;
			float m_iStartY;
			float m_iEndX;
			float m_iEndY;
		};
		struct PARAMETER
		{
			PARAMETER() {}
			PARAMETER(TYPE Type,VIEWPORT Viewport)
			{
				m_Type = Type;
				m_Viewport = Viewport;
			}
			PARAMETER(TYPE Type,VIEWPORT Viewport,float fFov)
			{
				m_Type = Type;
				m_Viewport = Viewport;
				m_fFov = fFov;
			}
			TYPE m_Type;
			VIEWPORT m_Viewport;
			float m_fFov;
		};

		CAMERA() {m_Parameter.m_Type = TYPE_NONE;m_vPosition = mathHelper::VEC3(0,0,0);m_bRightButton = false;m_fRoll = 0;m_fPitch = 0;m_fYaw = 0;m_qRotation = mathHelper::QUAT(0,0,0,1);};
		CAMERA(PARAMETER Param) {m_Parameter = Param;m_vPosition = mathHelper::VEC3(0,0,0);m_bRightButton = false;m_fRoll = 0;m_fPitch = 0;m_fYaw = 0;m_qRotation = mathHelper::QUAT(0,0,0,1);};
		virtual ~CAMERA() {};

		void Activate(GLuint uiPerspViewMtx,GLuint uiCamMtx);
		mathHelper::MTX44* GetPersMatrix();
		mathHelper::MTX34* GetObjMatrix();
		void SetViewport(VIEWPORT& v) {m_Parameter.m_Viewport = v;}
		VIEWPORT GetViewport() {return m_Parameter.m_Viewport;}
		virtual void Process(float TimeDelta);
		virtual void MouseMove(int button, int state,int x, int y);
		void GetProjectionMatrix(float* Matrix) {memcpy(Matrix,ProjectionMatrix.a,sizeof(float) * 16);}

		void SetRoll(float r);
		void SetPitch(float r);
		void SetYaw(float r);

		mathHelper::VEC3 GetPosition();
		void SetPosition(mathHelper::VEC3 v);
		void Roll(float c);
		void Pitch(float c);
	private:
		void CalculateMatrices();
		void BuildPerspProjMat(float *m, float fov, float aspect,float znear, float zfar);
		void BuildPerspProjMat1(float *m, float fov, float aspect,float znear, float zfar);

		PARAMETER m_Parameter;
		mathHelper::VEC3 m_vPosition;
		mathHelper::QUAT m_qRotation;
		bool m_bRightButton;
		int m_iRightbuttonStartX;
		int m_iRightbuttonStartY;
		mathHelper::MTX44 ProjectionMatrix;
		mathHelper::MTX34 ObjMatrix;
		float m_fRoll;
		float m_fPitch;
		float m_fYaw;
	};
}
