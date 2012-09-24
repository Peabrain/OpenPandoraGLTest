#include "ShaderLib.h"
#include <stdio.h>
#include <malloc.h>
#include "defines.h"

ShaderLib g_ShaderLib;

ShaderLib::ShaderLib()
{
}
ShaderLib::~ShaderLib()
{
}
bool ShaderLib::LoadProgramm(const char* ShaderVS,const char* ShaderFS,GLuint& ProgramID,const char* sShaderName)
{
	GLuint uiFragShader, uiVertShader;		// Used to hold the fragment and vertex shader handles
	GLuint uiProgramObject;					// Used to hold the program handle (made out of the two previous shaders

	int iLen = 0;
	std::string sPath = PathConv(ShaderVS);
	char* pCode = LoadFile(sPath.c_str(),iLen);
	if(!pCode) return false;
	if(CompileShader(pCode,GL_VERTEX_SHADER,uiVertShader) == false) { free(pCode);return false; };
	free(pCode);
	sPath = PathConv(ShaderFS);
	pCode = LoadFile(sPath.c_str(),iLen);
	if(!pCode) return false;
	if(CompileShader(pCode,GL_FRAGMENT_SHADER,uiFragShader) == false) { free(pCode);return false; };
	free(pCode);

	// Create the shader program
	uiProgramObject = glCreateProgram();

	// Attach the fragment and vertex shaders to it
	glAttachShader(uiProgramObject, uiFragShader);
	glAttachShader(uiProgramObject, uiVertShader);

	// Link the program
	glLinkProgram(uiProgramObject);

	// Check if linking succeeded in the same way we checked for compilation success
	GLint bLinked;
	glGetProgramiv(uiProgramObject, GL_LINK_STATUS, &bLinked);

	if (!bLinked)
	{
		int ui32InfoLogLength, ui32CharsWritten;
		glGetProgramiv(uiProgramObject, GL_INFO_LOG_LENGTH, &ui32InfoLogLength);
		char* pszInfoLog = new char[ui32InfoLogLength];
		glGetProgramInfoLog(uiProgramObject, ui32InfoLogLength, &ui32CharsWritten, pszInfoLog);
		printf("Failed to link program: %s\n", pszInfoLog);
		delete [] pszInfoLog;
		glDeleteShader(uiFragShader);
		glDeleteShader(uiVertShader);
		return false;
	}

	m_ShaderList.push_back(uiFragShader);
	m_ShaderList.push_back(uiVertShader);
	m_ProgramList[sShaderName] = uiProgramObject;
	ProgramID = uiProgramObject;

	ShaderDef s;
	s.m_Vertex = glGetAttribLocation(uiProgramObject,"myVertex");
	s.m_Normal = glGetAttribLocation(uiProgramObject,"myNormal");
	s.m_Tangent = glGetAttribLocation(uiProgramObject,"myTangent");
	s.m_Binormal = glGetAttribLocation(uiProgramObject,"myBinormal");
	s.m_Color = glGetAttribLocation(uiProgramObject,"myColor");
	s.m_Texture0 = glGetAttribLocation(uiProgramObject,"myTexture0");
	s.m_ProjViewObjMtx = glGetUniformLocation(uiProgramObject, "ProjModelViewMatrix");
	s.m_ObjMtx = glGetUniformLocation(uiProgramObject, "ModelMatrix");
	s.m_ObjMtxInv = glGetUniformLocation(uiProgramObject, "ModelMatrixInv");
	s.m_CameraMtx = glGetUniformLocation(uiProgramObject, "CameraMatrix");
	s.m_DiffuseTex = glGetUniformLocation(uiProgramObject, "samplerDiff");
	s.m_NormalTex = glGetUniformLocation(uiProgramObject, "samplerNormal");
	s.m_IncandescenceTex = glGetUniformLocation(uiProgramObject, "samplerIncandescence");
	s.m_Light0_Dir = glGetUniformLocation(uiProgramObject, "Light0_Dir");

	m_ShaderDefMap[ProgramID] = s;

	return true;
}
bool ShaderLib::GetShaderDefs(GLuint ProgramID,ShaderDef& Def)
{
	std::map<GLuint,ShaderDef>::iterator it = m_ShaderDefMap.find(ProgramID);
	if(it != m_ShaderDefMap.end())
	{
		Def = it->second;
		return true;
	}
	return false;
}
bool ShaderLib::CompileShader(const char *code,int GLShaderType,GLuint& ShaderID)
{
	GLuint uiShader;		// Used to hold the fragment and vertex shader handles
	// Create the fragment shader object
	uiShader = glCreateShader(GLShaderType);

	// Load the source code into it
	glShaderSource(uiShader, 1, (const char**)&code, NULL);

	// Compile the source code
	glCompileShader(uiShader);

	// Check if compilation succeeded
	GLint bShaderCompiled;
	glGetShaderiv(uiShader, GL_COMPILE_STATUS, &bShaderCompiled);

	if (!bShaderCompiled)
	{
		// An error happened, first retrieve the length of the log message
		int i32InfoLogLength, i32CharsWritten;
		glGetShaderiv(uiShader, GL_INFO_LOG_LENGTH, &i32InfoLogLength);

		// Allocate enough space for the message and retrieve it
		char* pszInfoLog = new char[i32InfoLogLength];
		glGetShaderInfoLog(uiShader, i32InfoLogLength, &i32CharsWritten, pszInfoLog);

		// Displays the error
		printf("Failed to compile shader: %s\n", pszInfoLog);
		delete [] pszInfoLog;
		return false;
	}
	ShaderID = uiShader;
	return true;
}
GLuint ShaderLib::GetShader(std::string sName)
{
	std::map<std::string,GLuint>::iterator it = m_ProgramList.find(sName);
	if(it != m_ProgramList.end())
		return it->second;
	return -1;
}
void ShaderLib::CleanUp()
{
	std::map<std::string,GLuint>::iterator it = m_ProgramList.begin();
	std::map<std::string,GLuint>::iterator ie = m_ProgramList.end();

	for(;it != ie;it++)
	{
		glDeleteProgram(it->second);
	}
	m_ProgramList.clear();
	while(!m_ShaderList.empty())
	{
		glDeleteShader(*m_ShaderList.begin());
		m_ShaderList.pop_front();
	}
}
