
#include <stdio.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "EngWindow.h"
#include "GLES20Context.h"
#include "ShaderLib.h"
#include "VertexBuffer.h"
#include "Indexbuffer.h"
#include "mathHelper.h"
#include "Camera.h"
#include "Texture.h"
#include "Model.h"
#include "libEngine.h"
#ifndef WIN32
#include <linux/input.h>
#endif

bool fullscreen = true;
int main(int argc, char **argv)
{
	EngWindow *myWindow = new EngWindow();
	if(myWindow->Create())
	{
		GLES20Context* myContext = new GLES20Context();
		if(myContext->Init(myWindow))
		{
			GLuint uiProgramObject = 0;
			if(g_ShaderLib.LoadProgramm("Data/Materials/Shaders/Diffuse.vs","Data/Materials/Shaders/Diffuse.fs",uiProgramObject,"FlatShader"))
			{
				g_ShaderLib.LoadProgramm("Data/Materials/Shaders/BumpDiffuse.vs","Data/Materials/Shaders/BumpDiffuse.fs",uiProgramObject,"BumpDiffuse");

				lib3D::LibraryEngine.GetInputManager()->Init();

				lib3D::TEXTURE* pTexture1 = lib3D::TextureLibrary.LoadPng("Data/Textures/shiptex05.png");
				lib3D::TEXTURE* pTexture2 = lib3D::TextureLibrary.LoadPng("Data/Textures/shiptex06.png");
//				lib3D::TEXTURE* pTexture3 = lib3D::TextureLibrary.LoadPng("Data/Textures/sta3_nor.png");
//			 	if(pTexture1 == 0) printf("cannot load texture\n");
//			 	if(pTexture2 == 0) printf("cannot load texture\n");

//				lib3D::MODEL* pModel = 0;
//				if(lib3D::ModelLibrary.LoadModel("Data/Models/ship01.dae")) pModel = lib3D::ModelLibrary.GetModelPointer("ship01.dae");

				lib3D::MODEL* pStation = 0;
				if(lib3D::ModelLibrary.LoadModel("Data/Models/spacestation.dae")) pStation = lib3D::ModelLibrary.GetModelPointer("spacestation.dae");

				glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // clear blue

				lib3D::CAMERA::PARAMETER p(lib3D::CAMERA::TYPE_PERS,lib3D::CAMERA::VIEWPORT(0,0,1,1),45.0);
				lib3D::SceneCamera = new lib3D::CAMERA(p);

				mathHelper::Quaternion qZ;

				lib3D::NODE_ENTITY* Entity0 = new lib3D::NODE_ENTITY();
				Entity0->SetProxyModel(pStation);
				lib3D::SCENENODE sn0(Entity0);
				qZ = mathHelper::EulerToQuat(0,3.14159f / 2.0f,0);
				sn0.SetPosition(mathHelper::VEC3(0,0,-400));
				sn0.SetRotation(mathHelper::QUAT(qZ.x,qZ.y,qZ.z,qZ.w));
				sn0.SetScaling(mathHelper::VEC3(1,1,1));
				lib3D::SCENENODE* scNow0 = lib3D::LibraryEngine.AddNodeToScenegraph(sn0);
/**/

/*				lib3D::NODE_ENTITY* Entity1 = new lib3D::NODE_ENTITY();
				Entity1->SetProxyModel(pModel);
				lib3D::SCENENODE sn1(Entity1);
				sn1.SetPosition(mathHelper::VEC3(30,0,25 - 150));
				sn1.SetRotation(mathHelper::QUAT(0,0,0,1));
				sn1.SetScaling(mathHelper::VEC3(1,1,1));
				lib3D::SCENENODE* scNow1 = lib3D::LibraryEngine.AddNodeToScenegraph(sn1);
				scNow1->GetEntity()->ChangeParameter(0,"shiptex05.png");

				lib3D::NODE_ENTITY* Entity2 = new lib3D::NODE_ENTITY();
				Entity2->SetProxyModel(pModel);
				lib3D::SCENENODE sn2(Entity2);
				sn2.SetPosition(mathHelper::VEC3(-30,0,-25 - 150));
				sn2.SetRotation(mathHelper::QUAT(0,0,0,1));
				sn2.SetScaling(mathHelper::VEC3(1,1,1));
				lib3D::SCENENODE* scNow2 = lib3D::LibraryEngine.AddNodeToScenegraph(sn2);
				scNow2->GetEntity()->ChangeParameter(0,"shiptex06.png");
*/
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);

				float rot = 0;
				float fRoll = 0;
				float fPitch= 0;
				float fYaw = 0;
				mathHelper::VEC3 vCam = mathHelper::VEC3(0,0,100);
				lib3D::SceneCamera->SetPosition(vCam);
				while(myWindow->GetEnd() == false && !lib3D::LibraryEngine.GetInputManager()->IsKeyboardDown(0x1b))
				{
					lib3D::LibraryEngine.Process(0);
					libEngine::MYINPUT::NUB NubLeft = lib3D::LibraryEngine.GetInputManager()->GetNubLeft();
					lib3D::SceneCamera->Roll(NubLeft.fX * -0.02f);
					lib3D::SceneCamera->Pitch(NubLeft.fY * -0.02f);
					unsigned int uiButtons = lib3D::LibraryEngine.GetInputManager()->GetButtons();
					if(uiButtons & (libEngine::MYINPUT::SHOULDER_LEFT | libEngine::MYINPUT::SHOULDER_RIGHT))
					{
						mathHelper::MTX34* cam = lib3D::SceneCamera->GetObjMatrix();
						mathHelper::MTX34 mat;
						mathHelper::MTX34Transpose(&mat,cam);
						mathHelper::VEC3 v = mathHelper::VEC3(0,0,1);
						mathHelper::VEC3TransformNormal(&v,&mat,&v);
						mathHelper::VEC3 vCam = lib3D::SceneCamera->GetPosition();
						if(uiButtons & libEngine::MYINPUT::SHOULDER_LEFT) 
							vCam += v * 0.5;
						else
						if(uiButtons & libEngine::MYINPUT::SHOULDER_RIGHT)
							vCam += v * -0.5;
						lib3D::SceneCamera->SetPosition(vCam);
					}
#ifdef WIN32
//					if(lib3D::LibraryEngine.GetInputManager()->IsKeyboardDown(0x25)) lib3D::SceneCamera->Roll(0.02);
//					if(lib3D::LibraryEngine.GetInputManager()->IsKeyboardDown(0x27)) lib3D::SceneCamera->Roll(-0.02);
					if(lib3D::LibraryEngine.GetInputManager()->IsKeyboardDown(0x28)) lib3D::SceneCamera->Pitch(0.02);
					if(lib3D::LibraryEngine.GetInputManager()->IsKeyboardDown(0x26)) lib3D::SceneCamera->Pitch(-0.02);
					if(lib3D::LibraryEngine.GetInputManager()->IsKeyboardDown(L'o') || lib3D::LibraryEngine.GetInputManager()->IsKeyboardDown(L'l')) 
					{
						mathHelper::MTX34* cam = lib3D::SceneCamera->GetObjMatrix();
						mathHelper::MTX34 mat;
						mathHelper::MTX34Transpose(&mat,cam);
						mathHelper::VEC3 v = mathHelper::VEC3(0,0,1);
						mathHelper::VEC3TransformNormal(&v,&mat,&v);
						mathHelper::VEC3 vCam = lib3D::SceneCamera->GetPosition();
						if(lib3D::LibraryEngine.GetInputManager()->IsKeyboardDown(L'o')) 
							vCam += v * 0.2;
						else
							if(lib3D::LibraryEngine.GetInputManager()->IsKeyboardDown(L'l'))
								vCam += v * -0.2;
						lib3D::SceneCamera->SetPosition(vCam);
					}
#else
					if(lib3D::LibraryEngine.GetInputManager()->IsKeyboardDown(KEY_LEFT)) lib3D::SceneCamera->Roll(0.02);
					if(lib3D::LibraryEngine.GetInputManager()->IsKeyboardDown(KEY_RIGHT)) lib3D::SceneCamera->Roll(-0.02);
					if(lib3D::LibraryEngine.GetInputManager()->IsKeyboardDown(KEY_DOWN)) lib3D::SceneCamera->Pitch(0.02);
					if(lib3D::LibraryEngine.GetInputManager()->IsKeyboardDown(KEY_UP)) lib3D::SceneCamera->Pitch(-0.02);
					if(lib3D::LibraryEngine.GetInputManager()->IsKeyboardDown(KEY_O) || lib3D::LibraryEngine.GetInputManager()->IsKeyboardDown(KEY_L)) 
					{
						mathHelper::MTX34* cam = lib3D::SceneCamera->GetObjMatrix();
						mathHelper::MTX34 mat;
						mathHelper::MTX34Transpose(&mat,cam);
						mathHelper::VEC3 v = mathHelper::VEC3(0,0,1);
						mathHelper::VEC3TransformNormal(&v,&mat,&v);
						mathHelper::VEC3 vCam = lib3D::SceneCamera->GetPosition();
						if(lib3D::LibraryEngine.GetInputManager()->IsKeyboardDown(KEY_O)) 
							vCam += v * 0.2;
						else
							if(lib3D::LibraryEngine.GetInputManager()->IsKeyboardDown(KEY_L))
								vCam += v * -0.2;
						lib3D::SceneCamera->SetPosition(vCam);
					}
#endif

					lib3D::SceneCamera->SetRoll(fRoll);
					lib3D::SceneCamera->SetPitch(fPitch);
					lib3D::SceneCamera->SetYaw(fYaw);
					lib3D::SceneCamera->Process(0);

					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					lib3D::ModelLibrary.PreRender();

					mathHelper::Quaternion qZ;

/*					qZ = mathHelper::EulerToQuat(-rot / 1.7f,-rot / 1.1,-rot / 1.9);
					scNow1->SetRotation(mathHelper::QUAT(qZ.x,qZ.y,qZ.z,qZ.w));

					qZ = mathHelper::EulerToQuat(rot / 0.7f,rot / 2.4,rot / 2.1);
					scNow2->SetRotation(mathHelper::QUAT(qZ.x,qZ.y,qZ.z,qZ.w));
*/
//					qZ = mathHelper::EulerToQuat(0,rot / 2.4,0);
//					scNow0->SetRotation(mathHelper::QUAT(qZ.x,qZ.y,qZ.z,qZ.w));

					lib3D::LibraryEngine.Render();

					lib3D::ModelLibrary.PostRender();

					myContext->Swap();
					myWindow->Process();
					rot += 0.01;
				}
				lib3D::TextureLibrary.CleanUp();
				lib3D::LibraryEngine.GetInputManager()->Deinit();
			}
			g_ShaderLib.CleanUp();
		}
		if(myContext) delete myContext;
	}
	if(myWindow) delete myWindow;
	return 0;
}
