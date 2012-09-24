#pragma once
#include <GLES2/gl2.h>
#include <list>
#include <map>
#include <string>


class ShaderLib
{
public:
	ShaderLib();
	~ShaderLib();

	struct ShaderDef
	{
		ShaderDef()
		{
			m_Vertex = -1;
			m_Normal = -1;
			m_Tangent = -1;
			m_Binormal = -1;
			m_Color = -1;
			m_Texture0 = -1;
		}

		GLuint m_Vertex;
		GLuint m_Normal;
		GLuint m_Tangent;
		GLuint m_Binormal;
		GLuint m_Color;
		GLuint m_Texture0;
		GLuint m_ProjViewObjMtx;
		GLuint m_ObjMtx;
		GLuint m_ObjMtxInv;
		GLuint m_CameraMtx;
		GLuint m_DiffuseTex; 
		GLuint m_NormalTex; 
		GLuint m_IncandescenceTex;
		GLuint m_Light0_Dir;
	};

	bool LoadProgramm(const char* ShaderVS,const char* ShaderFS,GLuint& ProgrammID,const char* sShaderName);

	void CleanUp();

	bool GetShaderDefs(GLuint ProgramID,ShaderDef& Def);
	GLuint GetShader(std::string sName);
private:
	bool CompileShader(const char* code,int GLShaderType,GLuint& ShaderID);

	std::map<GLuint,ShaderDef> m_ShaderDefMap;
	std::list<GLuint> m_ShaderList;
	std::map<std::string,GLuint> m_ProgramList;
};

extern ShaderLib g_ShaderLib;
