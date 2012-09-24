#pragma once
#include <stdio.h>
#include <string>
#include "mathHelper.h"

inline std::string PathConv(const char* szPath)
{
	std::string sPath;
//#ifdef WIN32 
	sPath = "../"; 
//#else 
//	sPath = "/";
//#endif
	sPath += szPath;
	return sPath;
}

mathHelper::VEC3 GetOGLPos(int x, int y);
mathHelper::VEC3 GetOGLPosOnScreen(float x, float y,float z);
float PointDistanceFromLine(mathHelper::VEC2 A, mathHelper::VEC2 B, mathHelper::VEC2 P);
char* LoadFile(const char* szFileName,int& iLen);
