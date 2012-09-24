#pragma once
#include "Convert.h"
#include "tinyxml.h"
#include "Model.h"

namespace lib3D
{
	class CONVERT_DAE: public CONVERT
	{
	public:
		CONVERT_DAE() : CONVERT()
		{
		}
		virtual ~CONVERT_DAE()
		{
		}

		MODEL* Load(const char* szPath);
	private:
		void GetTexture(TiXmlElement* e,const char* Component,TEXTURE** Tex,std::map<std::string,TEXTURE*>& lTextureIDMap,std::map<std::string,TiXmlElement*>& NewParam);
		TEXTURE* GetTextureFromNewparam(const char* szID,std::map<std::string,TEXTURE*>& lTextureIDMap,std::map<std::string,TiXmlElement*>& NewParam);
		TEXTURE* GetTexture(TiXmlElement* eTexture,std::map<std::string,TEXTURE*>& lTextureIDMap,std::map<std::string,TiXmlElement*>& NewParam);

		struct SOURCE
		{
			SOURCE()
			{
				m_pMem = 0;
				m_iCount = 0;
				m_iOffset = 0;
			}
			float* m_pMem;
			int m_iCount;
			int m_iOffset;
		};
		void GetPolygons(const char* szPol,TiXmlElement *eMesh,std::map<std::string,SOURCE> SourceMap,std::list<MESH*>& MeshList);
	};
}
