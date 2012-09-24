#pragma once
#include <map>
#ifndef WIN32
#include <linux/input.h>
#else
#include <windows.h>
#include <XInput.h>
#pragma comment(lib, "XInput.lib")
#endif

namespace libEngine
{
	class MYINPUT
	{
	public:
#ifndef WIN32
		enum 
		{
			DEV_PWRBTN,
			DEV_KEYPAD,
			DEV_BUTTONS,
			DEV_TS,
			DEV_LNUB,
			DEV_RNUB,
			DEVS_TOTAL
		};
#endif
		struct NUB
		{
			NUB()
			{
				fX = 0;
				fY = 0;
			};
			float fX,fY;
		};
		enum BUTTON
		{
			SHOULDER_LEFT	= 0x00000001,
			SHOULDER_RIGHT	= 0x00000002,
		};

		MYINPUT();
		virtual ~MYINPUT();

		int Init();
		void Deinit();
		void Process();

		void ClearKeyMap() {m_KeyMap.clear();}
		virtual void KeyboardDown(unsigned short Key);
		virtual void KeyboardUp(unsigned short Key);
		bool IsKeyboardDown(unsigned short Key);

		libEngine::MYINPUT::NUB GetNubLeft();
		unsigned int GetButtons();
	private:
#ifndef WIN32
		int fbdev, ifd[DEVS_TOTAL];
		int imaxfd, ts_x, ts_y;
		int nubx[2], nuby[2];
		int pressed_l, pressed_r;
//		struct tsdev *ts;
		int retval;
#else
		XINPUT_STATE ControllerState;
		int iControllerNum;
#endif
		void SetNub(float fX,float fY,int iNub);

		libEngine::MYINPUT::NUB NubLeft;
		libEngine::MYINPUT::NUB NubRight;
		std::map<unsigned short,bool> m_KeyMap;
		unsigned int m_uiButtons;
	};
}
