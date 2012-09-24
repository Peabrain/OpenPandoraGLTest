#include "Texture.h"
#include "png.h"
#include "pnginfo.h"
#include "defines.h"
#include <malloc.h>
#include <EGL/egl.h>

using namespace lib3D;

lib3D::TEXTURELIB lib3D::TextureLibrary;

bool TEXTURE::LoadPng(const char* Path)
{
	GLubyte *outData = 0;
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned int sig_read = 0;
	int color_type, interlace_type;
	FILE *fp;

	if ((fp = fopen(Path, "rb")) == NULL)
		return false;

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL, NULL, NULL);
	if (png_ptr == NULL) 
	{
		fclose(fp);
		return false;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) 
	{
		fclose(fp);
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		return false;
	}
	if (setjmp(png_jmpbuf(png_ptr))) 
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
		fclose(fp);
		return false;
	}
	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, sig_read);
	png_read_png(png_ptr, info_ptr,0/* PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND*/, png_voidp_NULL);
	m_iWidth = info_ptr->width;
	m_iHeight = info_ptr->height;
	switch (info_ptr->color_type) 
	{
	case PNG_COLOR_TYPE_RGBA: m_bHasAlpha = true;break;
	case PNG_COLOR_TYPE_RGB: m_bHasAlpha = false;break;
	default: png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		return false;
	}
	unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
	outData = (unsigned char*) malloc(row_bytes * m_iHeight);
	png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
	for (int i = 0; i < m_iHeight; i++) 
	{
		memcpy(outData+(row_bytes * (m_iHeight-1-i)), row_pointers[i], row_bytes);
	}
	png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
	fclose(fp);

	if(outData)
	{
		// allocate a texture name
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		if(m_bHasAlpha)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth,m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,outData);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_iWidth,m_iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE,outData);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glHint(GL_GENERATE_MIPMAP_HINT,GL_NICEST);
		glGenerateMipmap(GL_TEXTURE_2D);
		EGLint iErr = eglGetError();
		if (iErr != EGL_SUCCESS)
		{
			printf("failed (%d).\n", iErr);
			return false;
		}
		free(outData);
	}
	return true;
}

void TEXTURE::Activate(TEXTURE::TYPE Type)
{
	switch(Type)
	{
		case COLOR:	glActiveTexture(GL_TEXTURE0 + 0);break;
		case NORMAL: glActiveTexture(GL_TEXTURE0 + 1);break;
		case INCANDESCENCE: glActiveTexture(GL_TEXTURE0 + 2);break;
	}
	glBindTexture(GL_TEXTURE_2D, texture);
}

void TEXTURE::Deactivate(TEXTURE::TYPE Type)
{
	switch(Type)
	{
		case COLOR:	glActiveTexture(GL_TEXTURE0 + 0);break;
		case NORMAL: glActiveTexture(GL_TEXTURE0 + 1);break;
		case INCANDESCENCE: glActiveTexture(GL_TEXTURE0 + 2);break;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

lib3D::TEXTURE* TEXTURELIB::LoadPng(const char* szPath)
{
	std::string Path(szPath);
	std::string sPath = PathConv(Path.c_str());
	std::map<std::string,TEXTURE*>::iterator it = m_Map.find(sPath);
	if(it == m_Map.end())
	{
		printf("load texture: %s\n",sPath.c_str());
		TEXTURE *t = new TEXTURE();
		if(t->LoadPng(sPath.c_str()) == true)
		{
			int iPos = Path.find_first_of("Data/Textures/");
			if(iPos != -1)
			{
				char t[1024] = {0};
				Path.copy(t,Path.size() - sizeof("Data/Textures"),sizeof("Data/Textures"));
				Path = std::string(t);
			}
			t->SetPath(Path);
			m_Map[Path] = t;
			return t;
		}
	}
	else
	{
		return it->second;
	}
	return 0;
}

void TEXTURELIB::CleanUp()
{
	std::map<std::string,TEXTURE*>::iterator it = m_Map.begin();
	std::map<std::string,TEXTURE*>::iterator ie = m_Map.end();
	for(;it != ie;it++)
	{
		delete it->second;
	}
	m_Map.clear();
}

void TEXTURELIB::GetList(std::list<std::string>& List)
{
	std::map<std::string,TEXTURE*>::const_iterator it = m_Map.begin();
	std::map<std::string,TEXTURE*>::const_iterator ie = m_Map.end();
	for(;it != ie;it++)
	{
		List.push_back(it->first);
	}
}
