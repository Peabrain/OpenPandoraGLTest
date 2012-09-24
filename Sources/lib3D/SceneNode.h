#pragma once
#include "Model.h"

namespace lib3D
{
	class NODE_ENTITY
	{
	public:
		struct MESHLINK
		{
			MESHLINK()
			{
				m_pOrg = 0;
			}
			MESHLINK(lib3D::NODE::MESHLINK* pOrg)
			{
				m_pOrg = pOrg;
				m_Org = *pOrg;
				m_Mine = *pOrg;
			}
			lib3D::NODE::MESHLINK* m_pOrg;
			lib3D::NODE::MESHLINK m_Org;
			lib3D::NODE::MESHLINK m_Mine;
		};

		NODE_ENTITY()
		{
			m_pProxyModel = 0;
		}
/*		NODE_ENTITY(NODE_ENTITY Entity)
		{
			*this = Entity;
		}
*/
		virtual ~NODE_ENTITY()
		{
		}

		void SetProxyModel(lib3D::MODEL* Proxy);

		void SetName(std::string& sName) {m_sName = sName;}
		std::string GetName() {return m_sName;}

		inline NODE_ENTITY* GetPtr() {return this;}
		
		std::map<int,MESHLINK>* GetParameterList() {return &m_pMeshlinkList;}
		bool ChangeParameter(int iID,std::string sParameter);
		inline lib3D::MODEL* GetProxyModel() {return m_pProxyModel;}
		void Render(mathHelper::MTX34& mMat,lib3D::CAMERA* Camera);
		void RenderBoundbox(mathHelper::MTX34& mMat,mathHelper::VEC3& vColor);
		bool CheckBoundbox(mathHelper::MTX34& mMat,mathHelper::VEC3& vPos,mathHelper::VEC3& vDir,float& Depth);
	private:
		std::string m_sName;
	protected:
		lib3D::MODEL* m_pProxyModel;
		std::map<int,MESHLINK> m_pMeshlinkList;
	};

	class SCENENODE
	{
	public:
		enum MOUSESTATE
		{
			NONE = 0,
			MOUSEOVER,
			SELECTED
		};
		SCENENODE();
		SCENENODE(NODE_ENTITY* Entity);
		virtual ~SCENENODE();

		void SetEntity(NODE_ENTITY* Entity);
		void Render(mathHelper::MTX34& mMat,lib3D::CAMERA* Camera);
		void RenderBoundbox(mathHelper::MTX34& mMat,mathHelper::VEC3& vColor);
		bool GetBoundBox(lib3D::BOUNDBOX& Box);
		void Process(float TimeDelta);
		void SetPosition(mathHelper::VEC3 vPos);
		void SetRotation(mathHelper::QUAT qRot);
		void SetScaling(mathHelper::VEC3 vSca);
		mathHelper::VEC3 GetPosition();
		mathHelper::QUAT GetRotation();
		mathHelper::VEC3 GetScaling();

		SCENENODE* GetPtr();
		NODE_ENTITY* GetEntity();
		void SetUnikID(int iID);

		bool CheckBoundbox(mathHelper::VEC3& vPos,mathHelper::VEC3& vDir,float& Depth);
		void SetMouseState(MOUSESTATE s);
		MOUSESTATE GetMouseState();
		void GetMatrix(mathHelper::MTX34& mat);
	private:
		NODE_ENTITY* m_Entity;
		mathHelper::VEC3 m_vScaling;
		mathHelper::VEC3 m_vPosition;
		mathHelper::QUAT m_qRotation;
		MOUSESTATE m_MouseState;
		int m_iID;
	};
}
