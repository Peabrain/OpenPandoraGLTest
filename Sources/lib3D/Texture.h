#pragma once
#ifdef WIN32
#include <Windows.h>
#endif
#include <list>
#include <map>
#include <string>
#include <string.h>
#include <GLES2/gl2.h>
#include "ShaderLib.h"

namespace lib3D
{
	class TEXTURE
	{
	public:
#define png_infopp_NULL 0
#define png_voidp_NULL 0
		enum TYPE
		{
			COLOR = 0,
			NORMAL = 1,
			INCANDESCENCE = 2
		};

		TEXTURE() {};
		virtual ~TEXTURE() {glDeleteTextures(1,&texture);};

		bool LoadPng(const char* Path);
		void Activate(TEXTURE::TYPE Type);
		void Deactivate(TEXTURE::TYPE Type);

		void SetPath(std::string& sText) {m_sPath = sText;}
		std::string GetPath() {return m_sPath;}
	private:
		std::string m_sPath;
		int m_iWidth;
		int m_iHeight;
		bool m_bHasAlpha;
		GLuint texture;
	};

	class TEXTURELIB
	{
	public:
		TEXTURELIB() {};
		virtual ~TEXTURELIB() {CleanUp();};

		void CleanUp();
		void GetList(std::list<std::string>& List);

		lib3D::TEXTURE* LoadPng(const char* Path);

		TEXTURE* Get(std::string sPath)
		{
			std::map<std::string,TEXTURE*>::const_iterator it = m_Map.find(sPath);
			if(it != m_Map.end())
			{
				return it->second;
			}
			return 0;
		}
	private:
		std::map<std::string,TEXTURE*> m_Map;
	};

	extern TEXTURELIB TextureLibrary;
}
