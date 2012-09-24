#ifdef WIN32
#include <Windows.h>
#endif
#include <sstream>
#include <iostream> 
#include "ConvertDAE.h"
#include "Texture.h"
#include "defines.h"

using namespace lib3D;

void CONVERT_DAE::GetPolygons(const char* szPol,TiXmlElement *eMesh,std::map<std::string,SOURCE> SourceMap,std::list<MESH*>& MeshList)
{
	TiXmlElement *ePolygons = eMesh->FirstChildElement(szPol);
	while(ePolygons)
	{
		const char* szCount = ePolygons->Attribute("count");
		int iCount = atoi(szCount);

		SOURCE SourceVertex;
		SOURCE SourceNormal;
		SOURCE SourceBinormal;
		SOURCE SourceTangent;
		SOURCE SourceUV0;

		int iMaxElementOffset = 0;
		TiXmlElement *eInput = ePolygons->FirstChildElement("input");
		while(eInput)
		{
			const char* szSemantic = eInput->Attribute("semantic");
			const char* szOffset = eInput->Attribute("offset");
			int iOffset = atoi(szOffset);
			if(iOffset + 1 > iMaxElementOffset) iMaxElementOffset = iOffset + 1;
			std::string sSource(eInput->Attribute("source"));
			char szSource[1024] = {0};
			strncpy(szSource,sSource.c_str() + 1,1024);
			if(strcmp(szSemantic,"VERTEX") == 0)
			{
				TiXmlElement *eVertices = eMesh->FirstChildElement("vertices");
				while(eVertices)
				{
					if(std::string(eVertices->Attribute("id")) == std::string(szSource))
					{
						TiXmlElement *eInput = eVertices->FirstChildElement("input");
						if(eInput)
						{
							std::string sSource(eInput->Attribute("source"));
							strncpy(szSource,sSource.c_str() + 1,1024);
							std::map<std::string,SOURCE>::iterator it = SourceMap.find(szSource);
							if(it != SourceMap.end())
							{
								SourceVertex = it->second;
								SourceVertex.m_iOffset = iOffset;
							}
							break;
						}
						eVertices = eVertices->NextSiblingElement("vertices");
					}
				}
			}
			else
			if(strcmp(szSemantic,"NORMAL") == 0)
			{
				std::map<std::string,SOURCE>::iterator it = SourceMap.find(szSource);
				if(it != SourceMap.end())
				{
					SourceNormal = it->second;
					SourceNormal.m_iOffset = iOffset;
				}
			}
			else
			if(strcmp(szSemantic,"TEXTANGENT") == 0)
			{
				std::map<std::string,SOURCE>::iterator it = SourceMap.find(szSource);
				if(it != SourceMap.end())
				{
					SourceTangent = it->second;
					SourceTangent.m_iOffset = iOffset;
				}
			}
			else
			if(strcmp(szSemantic,"TEXBINORMAL") == 0)
			{
				std::map<std::string,SOURCE>::iterator it = SourceMap.find(szSource);
				if(it != SourceMap.end())
				{
					SourceBinormal = it->second;
					SourceBinormal.m_iOffset = iOffset;
				}
			}
			else
			if(strcmp(szSemantic,"TEXCOORD") == 0)
			{
				std::map<std::string,SOURCE>::iterator it = SourceMap.find(szSource);
				if(it != SourceMap.end())
				{
					SourceUV0 = it->second;
					SourceUV0.m_iOffset = iOffset;
				}
			}
			eInput = eInput->NextSiblingElement("input");
		}

		int iElements = 0;
		if(SourceVertex.m_iCount) iElements++;
		if(SourceNormal.m_iCount) iElements++;
		if(SourceTangent.m_iCount) iElements++;
		if(SourceBinormal.m_iCount) iElements++;
		if(SourceUV0.m_iCount) iElements++;

		int* pPolyMem1 = new int[iMaxElementOffset * 3 * iCount];
		int* pPolyMem = new int[iElements * 3 * iCount];

		int i = 0;
		bool bError = false;
		int iPolyCount = 0;
		TiXmlElement *eP = ePolygons->FirstChildElement("p");
		while(eP)
		{
			const char* szText = eP->GetText();
			std::stringstream ss(szText);
			std::string out;
			while(ss >> out)
			{
				int fValue = (int)atoi(out.c_str());
				pPolyMem1[i++] = fValue;
			}
			eP = eP->NextSiblingElement("p");
		}

		int off = 0;
		if(SourceVertex.m_iCount)
		{
			for(int i = 0;i < 3 * iCount;i++) pPolyMem[iElements * i + off] = pPolyMem1[iMaxElementOffset * i + SourceVertex.m_iOffset];
			SourceVertex.m_iOffset = off++;
		}
		if(SourceNormal.m_iCount)
		{
			for(int i = 0;i < 3 * iCount;i++) pPolyMem[iElements * i + off] = pPolyMem1[iMaxElementOffset * i + SourceNormal.m_iOffset];
			SourceNormal.m_iOffset = off++;
		}
		if(SourceTangent.m_iCount)
		{
			for(int i = 0;i < 3 * iCount;i++) pPolyMem[iElements * i + off] = pPolyMem1[iMaxElementOffset * i + SourceTangent.m_iOffset];
			SourceTangent.m_iOffset = off++;
		}
		if(SourceBinormal.m_iCount)
		{
			for(int i = 0;i < 3 * iCount;i++) pPolyMem[iElements * i + off] = pPolyMem1[iMaxElementOffset * i + SourceBinormal.m_iOffset];
			SourceBinormal.m_iOffset = off++;
		}
		if(SourceUV0.m_iCount)
		{
			for(int i = 0;i < 3 * iCount;i++) pPolyMem[iElements * i + off] = pPolyMem1[iMaxElementOffset * i + SourceUV0.m_iOffset];
			SourceUV0.m_iOffset = off++;
		}

		if(bError == false)
		{
			unsigned short* pTmpIdxBuf = new unsigned short[iElements * 3 * iCount];
			unsigned short* pIdxBuf = new unsigned short[3 * iCount];
			int iLastIdx = 0;
			for(int i = 0;i < iCount * 3;i++)
			{
				int iFoundIdx = -1;
				for(int k = 0;k < iLastIdx * iElements;k += iElements)
				{
					bool bFound = true;
					for(int p = 0;p < iElements;p++)
					{
						if(pTmpIdxBuf[k + p] != pPolyMem[i * iElements + p])
						{
							bFound = false;
							break;
						}
					}
					if(bFound == true)
					{
						iFoundIdx = k / iElements;
						break;
					}
				}
				if(iFoundIdx == -1)
				{
					for(int m = 0;m < iElements;m++)
					{
						pTmpIdxBuf[iLastIdx * iElements + m] = pPolyMem[i * iElements + m];
					}
					pIdxBuf[i] = iLastIdx;
					iLastIdx++;
				}
				else
				{
					pIdxBuf[i] = iFoundIdx;
				}
			}

			const char* Name = ePolygons->Attribute("material");
			MESH* pMesh = new MESH(std::string(Name));

			if(SourceVertex.m_iCount) 
			{
				float* pBuffer = (float*)malloc(sizeof(float) * iLastIdx * 3);
				for(int k = SourceVertex.m_iOffset,i = 0;k < iLastIdx * iElements + SourceVertex.m_iOffset;k += iElements,i++)
				{
					int e = pTmpIdxBuf[k];
					float x = SourceVertex.m_pMem[e * 3]; 
					float y = SourceVertex.m_pMem[e * 3 + 1];
					float z = SourceVertex.m_pMem[e * 3 + 2];
					pBuffer[i * 3] = x;
					pBuffer[i * 3 + 1] = y;
					pBuffer[i * 3 + 2] = z;
				}
				VERTEXBUFFER* vb = new VERTEXBUFFER(pBuffer,iLastIdx,3 * sizeof(float),VERTEXBUFFER::VERTEX);
				pMesh->AddVertexBuffer(vb);
				free(pBuffer);
			}
			if(SourceNormal.m_iCount)
			{
				float* pBuffer = (float*)malloc(sizeof(float) * iLastIdx * 3);
				for(int k = SourceNormal.m_iOffset,i = 0;k < iLastIdx * iElements + SourceNormal.m_iOffset;k += iElements,i++)
				{
					int e = pTmpIdxBuf[k];
					float x = SourceNormal.m_pMem[e * 3];
					float y = SourceNormal.m_pMem[e * 3 + 1];
					float z = SourceNormal.m_pMem[e * 3 + 2];
					pBuffer[i * 3] = x;
					pBuffer[i * 3 + 1] = y;
					pBuffer[i * 3 + 2] = z;
				}
				VERTEXBUFFER* vb = new VERTEXBUFFER(pBuffer,iLastIdx,3 * sizeof(float),VERTEXBUFFER::NORMAL);
				pMesh->AddVertexBuffer(vb);
				free(pBuffer);
			}
			if(SourceTangent.m_iCount)
			{
				float* pBuffer = (float*)malloc(sizeof(float) * iLastIdx * 3);
				for(int k = SourceTangent.m_iOffset,i = 0;k < iLastIdx * iElements + SourceTangent.m_iOffset;k += iElements,i++)
				{
					int e = pTmpIdxBuf[k];
					float x = SourceTangent.m_pMem[e * 3];
					float y = SourceTangent.m_pMem[e * 3 + 1];
					float z = SourceTangent.m_pMem[e * 3 + 2];
					pBuffer[i * 3] = x;
					pBuffer[i * 3 + 1] = y;
					pBuffer[i * 3 + 2] = z;
				}
				VERTEXBUFFER* vb = new VERTEXBUFFER(pBuffer,iLastIdx,3 * sizeof(float),VERTEXBUFFER::TANGENT);
				pMesh->AddVertexBuffer(vb);
				free(pBuffer);
			}
			if(SourceBinormal.m_iCount)
			{
				float* pBuffer = (float*)malloc(sizeof(float) * iLastIdx * 3);
				for(int k = SourceBinormal.m_iOffset,i = 0;k < iLastIdx * iElements + SourceBinormal.m_iOffset;k += iElements,i++)
				{
					int e = pTmpIdxBuf[k];
					float x = SourceBinormal.m_pMem[e * 3];
					float y = SourceBinormal.m_pMem[e * 3 + 1];
					float z = SourceBinormal.m_pMem[e * 3 + 2];
					pBuffer[i * 3] = x;
					pBuffer[i * 3 + 1] = y;
					pBuffer[i * 3 + 2] = z;
				}
				VERTEXBUFFER* vb = new VERTEXBUFFER(pBuffer,iLastIdx,3 * sizeof(float),VERTEXBUFFER::BINORMAL);
				pMesh->AddVertexBuffer(vb);
				free(pBuffer);
			}
			if(SourceUV0.m_iCount)
			{
				float* pBuffer = (float*)malloc(sizeof(float) * iLastIdx * 2);
				for(int k = SourceUV0.m_iOffset,i = 0;k < iLastIdx * iElements + SourceUV0.m_iOffset;k += iElements,i++)
				{
					int e = pTmpIdxBuf[k];
					float x = SourceUV0.m_pMem[e * 2];
					float y = SourceUV0.m_pMem[e * 2 + 1];
					pBuffer[i * 2] = x;
					pBuffer[i * 2 + 1] = y;
				}
				VERTEXBUFFER* vb = new VERTEXBUFFER(pBuffer,iLastIdx,2 * sizeof(float),VERTEXBUFFER::TEXTURE0);
				pMesh->AddVertexBuffer(vb);
				free(pBuffer);
			}

			pMesh->SetIndexBuffer(new INDEXBUFFER(pIdxBuf,3 * iCount,sizeof(unsigned short)));
			MeshList.push_back(pMesh);

			delete [] pIdxBuf;
			delete [] pTmpIdxBuf;
		}

		delete [] pPolyMem;
		delete [] pPolyMem1;
		ePolygons = ePolygons->NextSiblingElement(szPol);
	}
}

TEXTURE* CONVERT_DAE::GetTextureFromNewparam(const char* szID,std::map<std::string,TEXTURE*>& lTextureIDMap,std::map<std::string,TiXmlElement*>& NewParam)
{
	std::map<std::string,TiXmlElement*>::const_iterator itn = NewParam.find(std::string(szID));
	if(itn != NewParam.end())
	{
		TiXmlElement* e = itn->second;
		TiXmlElement* eSurface = e->FirstChildElement("surface");
		if(eSurface)
		{
			TiXmlElement* eInitfrom = eSurface->FirstChildElement("init_from");
			if(eInitfrom)
			{
				const char* szText = eInitfrom->GetText();
				std::map<std::string,TEXTURE*>::iterator it = lTextureIDMap.find(std::string(szText));
				if(it != lTextureIDMap.end())
				{
					return it->second;
				}
			}
		}
	}
	return 0;
}
TEXTURE* CONVERT_DAE::GetTexture(TiXmlElement* eTexture,std::map<std::string,TEXTURE*>& lTextureIDMap,std::map<std::string,TiXmlElement*>& NewParam)
{
	const char* szSampler = eTexture->Attribute("texture");
	if(szSampler)
	{
		std::map<std::string,TiXmlElement*>::const_iterator it = NewParam.find(std::string(szSampler));
		if(it != NewParam.end())
		{
			TiXmlElement* eSampler2D = it->second->FirstChildElement("sampler2D");
			if(eSampler2D)
			{
				TiXmlElement* eSource = eSampler2D->FirstChildElement("source");
				if(eSource)
				{
					const char* szText = eSource->GetText();
					if(szText)
					{
						return GetTextureFromNewparam(szText,lTextureIDMap,NewParam);
					}
				}
			}
		}
	}
	return 0;
}
void CONVERT_DAE::GetTexture(TiXmlElement* e,const char* Component,TEXTURE** Tex,std::map<std::string,TEXTURE*>& lTextureIDMap,std::map<std::string,TiXmlElement*>& NewParam)
{
	TiXmlElement* eDiffuse = e->FirstChildElement(Component);
	if(eDiffuse)
	{
		TiXmlElement* eTexture = eDiffuse->FirstChildElement("texture");
		if(eTexture) *Tex = GetTexture(eTexture,lTextureIDMap,NewParam);
	}
}

MODEL* CONVERT_DAE::Load(const char* szPath)
{
	std::string sPath = PathConv(szPath);

	std::map<std::string,TiXmlElement*> lMaterialMap;
	std::map<std::string,TiXmlElement*> lMaterialEffectMap;
	std::map<std::string,TEXTURE*> lTextureIDMap;
//	std::map<std::string,bool> lBumpTextureMap;

	MODEL* pModel = 0;
	TiXmlDocument doc;
	doc.LoadFile(sPath.c_str());
	if(doc.Error() == false)
	{
		TiXmlElement *e = doc.FirstChildElement();
		const char* szValue = e->Value();
		if(strcmp(szValue,"COLLADA") == 0)
		{
			pModel = new MODEL();
			TiXmlElement *eCollada = e->FirstChildElement();
			while(eCollada)
			{
				const char* szValue = eCollada->Value();
				if(strcmp(szValue,"library_geometries") == 0)
				{
					TiXmlElement *eGeometry = eCollada->FirstChildElement("geometry");
					while(eGeometry)
					{
						const char* szGeometryID = eGeometry->Attribute("id");
						const char* szGeometryName = eGeometry->Attribute("name");
						TiXmlElement *eMesh = eGeometry->FirstChildElement("mesh");
						while(eMesh)
						{
							std::map<std::string,SOURCE> SourceMap;
							TiXmlElement *eSource = eMesh->FirstChildElement("source");
							while(eSource)
							{
								SOURCE sou;
								const char* szSourceID = eSource->Attribute("id");
								TiXmlElement *eFloat_array = eSource->FirstChildElement("float_array");
								if(eFloat_array)
								{
									const char* szCount = eFloat_array->Attribute("count");
									int iCount = atoi(szCount);
									const char* szText = eFloat_array->GetText();
									std::stringstream ss(szText);
									std::string out;
									int i = 0;
									float *mem = new float[iCount];
									while(ss >> out)
									{
										float fValue = (float)atof(out.c_str());
										mem[i++] = fValue;
									}
									sou.m_pMem = mem;
									sou.m_iCount = iCount;
									SourceMap[std::string(szSourceID)] = sou;
								}
								eSource = eSource->NextSiblingElement("source");
							}

							
							std::list<MESH*> MeshList;
							GetPolygons("polygons",eMesh,SourceMap,MeshList);
							GetPolygons("triangles",eMesh,SourceMap,MeshList);

							std::list<MESH*>::iterator it = MeshList.begin();
							std::list<MESH*>::iterator ie = MeshList.end();
							for(;it != ie;it++)
							{
								MESH* m = *it;
								std::string s = std::string(szGeometryID);
								s += "/";
								s += m->GetName();
								m->SetName(s);
								pModel->AddMesh(m);
							}

							std::map<std::string,SOURCE>::iterator itm = SourceMap.begin();
							std::map<std::string,SOURCE>::iterator iem = SourceMap.end();
							for(;itm != iem;itm++)
							{
								SOURCE sou = itm->second;
								delete [] sou.m_pMem;
							}
							SourceMap.clear();
							eMesh = eMesh->NextSiblingElement("mesh");
						}
						eGeometry = eGeometry->NextSiblingElement("geometry");
					}
				}
				else
				if(strcmp(szValue,"library_images") == 0)
				{
					TiXmlElement *eImage = eCollada->FirstChildElement("image");
					while(eImage)
					{
						const char* szID = eImage->Attribute("id");
						if(!szID) continue;
						TiXmlElement *eFileInit = eImage->FirstChildElement("init_from");
						if(eFileInit)
						{
							const char* szFile = eFileInit->GetText();
							if(!szFile) continue;
							std::string sFile(szFile);
							int pos = sFile.rfind("Textures/");
							if(pos != -1)
							{
								sFile = std::string("Data/") + std::string(szFile + pos);
								if(TEXTURE* tex = TextureLibrary.LoadPng(sFile.c_str()))
								{
									lTextureIDMap[std::string(szID)] = tex;
								}
							}
						}
						eImage = eImage->NextSiblingElement("image");
					}
				}
				else
				if(strcmp(szValue,"library_effects") == 0)
				{
					TiXmlElement *eEffect = eCollada->FirstChildElement("effect");
					while(eEffect)
					{
						const char* szID = eEffect->Attribute("id");
						std::map<std::string,TiXmlElement*>::const_iterator it = lMaterialMap.find(std::string(szID));
						if(it != lMaterialMap.end())
						{
							const char* szName = it->second->Attribute("id");
							lMaterialEffectMap[std::string(szName)] = eEffect;
						}
						eEffect = eEffect->NextSiblingElement("effect");
					}
				}
				else
				if(strcmp(szValue,"library_materials") == 0)
				{
					TiXmlElement *eMaterial = eCollada->FirstChildElement("material");
					while(eMaterial)
					{
						TiXmlElement *eEffectInstance = eMaterial->FirstChildElement("instance_effect");
						if(eEffectInstance)
						{
							const char* szUrl = eEffectInstance->Attribute("url");
							std::string sUrl(szUrl + 1);
							lMaterialMap[sUrl] = eMaterial;
						}
						eMaterial = eMaterial->NextSiblingElement("material");
					}
				}
				else
				if(strcmp(szValue,"library_visual_scenes") == 0)
				{
					TiXmlElement *eScenes = eCollada->FirstChildElement("visual_scene");
					if(eScenes)
					{
						NODE RootNode;
						TiXmlElement *eNode = eScenes->FirstChildElement("node");
						while(eNode)
						{
							NODE Node;
							mathHelper::VEC3 vPosition(0,0,0);
							mathHelper::VEC3 vRotation(0,0,0);
							TiXmlElement *ePosition = eNode->FirstChildElement("translate");
							if(ePosition)
							{
								const char* szPosition = ePosition->GetText();
								sscanf(szPosition,"%f %f %f",&vPosition.x,&vPosition.y,&vPosition.z);
							}
							Node.SetPosition(vPosition);
							TiXmlElement *eRotation = eNode->FirstChildElement("rotate");
							while(eRotation)
							{
								const char* szParam = eRotation->Attribute("sid");
								const char* szText = eRotation->GetText();
								if(szText)
								{
									mathHelper::VEC3 vAxis;
									float fValue;
									sscanf(szText,"%f %f %f %f",&vAxis.x,&vAxis.y,&vAxis.z,&fValue);
									if(strcmp(szParam,"rotateZ") == 0) vRotation.z = fValue;
									if(strcmp(szParam,"rotateY") == 0) vRotation.y = fValue;
									if(strcmp(szParam,"rotateX") == 0) vRotation.x = fValue;
								}
								eRotation = eRotation->NextSiblingElement("rotate");
							}
							Node.SetPosition(vPosition);
							Node.SetRotation(vRotation);
							TiXmlElement *eInstance = eNode->FirstChildElement("instance_geometry");
							while(eInstance)
							{
								const char* szGeometry = eInstance->Attribute("url");
								std::string sGeometry(szGeometry + 1);
								TiXmlElement *eMaterial = eInstance->FirstChildElement("bind_material");
								if(eMaterial)
								{
									TiXmlElement *eTechnique = eMaterial->FirstChildElement("technique_common");
									if(eTechnique)
									{
										TiXmlElement *eMaterialInstance = eTechnique->FirstChildElement("instance_material");
										while(eMaterialInstance)
										{
											const char* szSymbol = eMaterialInstance->Attribute("symbol");
											const char* szTarget = eMaterialInstance->Attribute("target");
											std::string sTarget(szTarget + 1);
											std::string s = sGeometry;
											s += "/";
											s += std::string(szSymbol);
											MESH* m = pModel->GetMesh(s);
											if(m)
											{
												TEXTURE* pTexDiffuse = 0;
												TEXTURE* pTexNormal = 0;
												TEXTURE* pTexIncandescence = 0;
												std::map<std::string,TiXmlElement*>::const_iterator it = lMaterialEffectMap.find(sTarget);
												if(it != lMaterialEffectMap.end())
												{
													TiXmlElement* eEffect = it->second;
													TiXmlElement* eProfile = eEffect->FirstChildElement("profile_COMMON");
													if(eProfile)
													{
														std::map<std::string,TiXmlElement*> NewParam;
														TiXmlElement* eNewparam = eProfile->FirstChildElement("newparam");
														while(eNewparam)
														{
															const char* szID = eNewparam->Attribute("sid");
															if(szID) NewParam[std::string(szID)] = eNewparam;
															eNewparam = eNewparam->NextSiblingElement("newparam");
														}
														TiXmlElement* eTechnique = eProfile->FirstChildElement("technique");
														if(eTechnique)
														{
															TiXmlElement* e = eTechnique->FirstChildElement();
															if(e)
															{
																GetTexture(e,"diffuse",&pTexDiffuse,lTextureIDMap,NewParam);
																GetTexture(e,"emission",&pTexIncandescence,lTextureIDMap,NewParam);
															}
															TiXmlElement* Extra = eTechnique->FirstChildElement("extra");
															if(Extra)
															{
																TiXmlElement* tech = Extra->FirstChildElement("technique");
																if(tech)
																{
																	TiXmlElement* bump = tech->FirstChildElement("bump");
																	if(bump)
																	{
																		TiXmlElement* texture = bump->FirstChildElement("texture");
																		if(texture)
																		{
																			TEXTURE *Tex = GetTexture(texture,lTextureIDMap,NewParam);
																			pTexNormal = Tex;
																		}
																	}
																}
															}
														}
													}
												}
												if(pTexNormal)
													Node.AddMeshLink(NODE::MESHLINK(m,pTexDiffuse,pTexNormal,pTexIncandescence,g_ShaderLib.GetShader("BumpDiffuse")));
												else
													Node.AddMeshLink(NODE::MESHLINK(m,pTexDiffuse,pTexNormal,pTexIncandescence,g_ShaderLib.GetShader("FlatShader")));
											}
											eMaterialInstance = eMaterialInstance->NextSiblingElement("instance_material");
										}
									}
								}
								eInstance = eInstance->NextSiblingElement("instance_geometry");
							}
							RootNode.AddNode(Node);
							eNode = eNode->NextSiblingElement("node");
						}
						pModel->SetRootNode(RootNode);
						std::string s = std::string(szPath);
						pModel->SetModelName(s);
//						eScenes = eScenes->NextSiblingElement("visual_scene");
					}
				}
				else
				if(strcmp(szValue,"scene") == 0)
				{
				}
				eCollada = eCollada->NextSiblingElement();
			}
			return pModel;
		}
	}
	else
	{
		printf("Cannot open File: %s\n",sPath.c_str());
		printf("%s\n",doc.ErrorDesc());
	}

	return 0;
}
