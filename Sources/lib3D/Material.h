#pragma once
#include <map>
#include "Texture.h"

namespace lib3D
{
	class MATERIAL
	{
	public:
		struct TEXTURE
		{
			TEXTURE() 
			{
				m_sName = "";
				m_Tex = 0;
			}
			std::string m_sName;
			TEXTURE* m_Tex;
		};
		MATERIAL(std::string sName)
		{
			m_sName = sName;
		}
		virtual ~MATERIAL();

		std::string GetName() {return m_sName;}
	private:
		std::string m_sName;
		TEXTURE m_Diffuse;
	};

	class MATERIALLIB
	{
	public:
		MATERIALLIB();
		virtual ~MATERIALLIB();

		void AddMaterial(MATERIAL* Material)
		{
			std::map<std::string,MATERIAL*>::iterator it = m_pList.find(Material->GetName());
			if(it == m_pList.end())
			{
				m_pList[Material->GetName()] = Material;
			}
		}
	private:
		std::map<std::string,MATERIAL*> m_pList;
	};

	extern MATERIALLIB MaterialLib;
}
