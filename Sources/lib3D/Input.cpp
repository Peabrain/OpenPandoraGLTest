#include "Input.h"
#ifndef WIN32
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <tslib.h>
#include <string.h>
#include <stdio.h>
#endif

using namespace libEngine;

#ifndef WIN32

struct tsdev* ts;
#define array_size(x) (sizeof(x) / sizeof(x[0]))

/* this sucks, but EvilDragon wants it */
#ifdef VALIDATION
#define NUB_RANGE_REQ (256-8*3)
#else
#define NUB_RANGE_REQ 256
#endif

typedef struct {
        int x, y;
        int code;
        unsigned int active:1;
        unsigned int tested:1;
        const char *name;
} key_item;

static key_item key_items[] =
{
{  66,  26, KEY_1, 0, 0, "1" },
{ 133,  26, KEY_2, 0, 0, "2" },
{ 200,  26, KEY_3, 0, 0, "3" },
{ 266,  26, KEY_4, 0, 0, "4" },
{ 333,  26, KEY_5, 0, 0, "5" },
{ 400,  26, KEY_6, 0, 0, "6" },
{ 466,  26, KEY_7, 0, 0, "7" },
{ 533,  26, KEY_8, 0, 0, "8" },
{ 600,  26, KEY_9, 0, 0, "9" },
{ 666,  26, KEY_0, 0, 0, "0" },
{ 733,  26, KEY_BACKSPACE, 0, 0, "DEL" },
//
{  10,  55, KEY_RIGHTSHIFT,0, 0, "L" },
{  10,  75, KEY_KPPLUS,    0, 0, "L2" },
{ 758,  55, KEY_RIGHTCTRL, 0, 0, "R" },
{ 758,  75, KEY_KPMINUS,   0, 0, "R2" },
{  10, 220, KEY_POWER,     0, 0, "POWER" },
{  10, 240, KEY_COFFEE,    0, 0, "HOLD" },
//
{ 130,  94, KEY_UP,     0, 0, "U" },
{  80, 163, KEY_LEFT,   0, 0, "L" },
{ 180, 163, KEY_RIGHT,  0, 0, "R" },
{ 130, 232, KEY_DOWN,   0, 0, "D" },
{ 376,  94, KEY_LEFTALT,0, 0, "ALT" },
{ 368, 163, KEY_LEFTCTRL,0,0, "CTRL" },
{ 368, 232, KEY_MENU,   0, 0, "MENU" },
{ 700,  94, KEY_PAGEUP, 0, 0, "Y" },
{ 650, 163, KEY_HOME,   0, 0, "A" },
{ 750, 163, KEY_END,    0, 0, "B" },
{ 700, 232, KEY_PAGEDOWN,0,0, "X" },
//
{  92, 300, KEY_Q,      0, 0, "Q" },
{ 158, 300, KEY_W,      0, 0, "W" },
{ 225, 300, KEY_E,      0, 0, "E" },
{ 292, 300, KEY_R,      0, 0, "R" },
{ 358, 300, KEY_T,      0, 0, "T" },
{ 425, 300, KEY_Y,      0, 0, "Y" },
{ 492, 300, KEY_U,      0, 0, "U" },
{ 558, 300, KEY_I,      0, 0, "I" },
{ 625, 300, KEY_O,      0, 0, "O" },
{ 692, 300, KEY_P,      0, 0, "P" },
 //
{  30, 369, KEY_LEFTSHIFT, 0, 0, "SHIFT" },
{ 133, 369, KEY_A,      0, 0, "A" },
{ 200, 369, KEY_S,      0, 0, "S" },
{ 266, 369, KEY_D,      0, 0, "D" },
{ 333, 369, KEY_F,      0, 0, "F" },
{ 400, 369, KEY_G,      0, 0, "G" },
{ 466, 369, KEY_H,      0, 0, "H" },
{ 533, 369, KEY_J,      0, 0, "J" },
{ 600, 369, KEY_K,      0, 0, "K" },
{ 666, 369, KEY_L,      0, 0, "L" },
{ 710, 369, KEY_ENTER,  0, 0, "ENTER" },
//
{  25, 437, KEY_COMMA,  0, 0, "," },
{  92, 437, KEY_DOT,    0, 0, "." },
{ 158, 437, KEY_Z,      0, 0, "Z" },
{ 225, 437, KEY_X,      0, 0, "X" },
{ 292, 437, KEY_C,      0, 0, "C" },
{ 358, 437, KEY_V,      0, 0, "V" },
{ 425, 437, KEY_B,      0, 0, "B" },
{ 492, 437, KEY_N,      0, 0, "N" },
{ 558, 437, KEY_M,      0, 0, "M" },
{ 625, 437, KEY_SPACE,  0, 0, "SPACE" },
{ 758, 437, KEY_FN,     0, 0, "Fn" },
};
 

#define KI_COUNT (sizeof(key_items) / sizeof(key_items[0]))
 
static int nub_range_hit, all_btns_tested, lid_tested, ts_tested;
static int ts_old_x, ts_old_y;
static int lid_closed;
#endif

MYINPUT::MYINPUT()
{
#ifndef WIN32
	for(int i = 0;i < DEVS_TOTAL;i++) ifd[i] = -1;
	imaxfd = 0;
	ts_x = 0;
	ts_y = 0;
	nubx[0] = 0;nubx[1] = 0;
	nuby[0] = 0;nuby[1] = 0;
	pressed_l = 0;
	pressed_r = 0;
	tsdev *ts = NULL;
	retval = 0;
#else
	m_uiButtons = 0;
	iControllerNum = 0;
#endif
}
MYINPUT::~MYINPUT()
{
}
int MYINPUT::Init()
{
#ifndef WIN32
	int i, id;
	fbdev = open("/dev/fb0", O_RDWR);
	if (fbdev == -1)
	{
		perror("open(\"/dev/fb0\") failed");
		return -1;
	}

	for (id = 0; ; id++)
	{
		char fname[64];
		char name[256] = { 0, };
		int fd;
 
		snprintf(fname, sizeof(fname), "/dev/input/event%i", id);
		fd = open(fname, O_RDONLY);
		if (fd == -1) break;
 
		ioctl(fd, EVIOCGNAME(sizeof(name)), name);
 
		if (strcasestr(name, "power") != NULL || strcasestr(name, "pwrbutton") != NULL)
			ifd[DEV_PWRBTN] = fd;
		else 
		if (strcasestr(name, "keypad") != NULL)
			ifd[DEV_KEYPAD] = fd;
		else 
		if (strcmp(name, "gpio-keys") == 0)
			ifd[DEV_BUTTONS] = fd;
		else 
		if (strcasestr(name, "touchscreen") != NULL)
		{
			close(fd);
			ts = ts_open(fname, 0);
			if (ts == NULL)
			{
				perror("ts_open");
				return -1;
			}
			if (ts_config(ts))
			{
				perror("ts_config");
				return -1;
			}
			ifd[DEV_TS] = ts_fd(ts);
		}
		else 
		if (strcmp(name, "nub0") == 0)
		{
			ifd[DEV_LNUB] = fd;
		}
		else 
		if (strcmp(name, "nub1") == 0)
		{
			ifd[DEV_RNUB] = fd;
		}
		else
		{
			printf("skipping \"%s\"\n", name);
			close(fd);
			continue;
		}
		if (imaxfd < fd) imaxfd = fd;
	}
 
	if (ifd[DEV_PWRBTN]  == -1) printf("Warning: couldn't find pwrbutton device\n");
	if (ifd[DEV_KEYPAD]  == -1) printf("Warning: couldn't find keypad device\n");
	if (ifd[DEV_TS]      == -1) printf("Warning: couldn't find touchscreen device\n");
	if (ifd[DEV_LNUB]    == -1) printf("Warning: couldn't find nub1 device\n");
	if (ifd[DEV_RNUB]    == -1) printf("Warning: couldn't find nub2 device\n");
	if (ifd[DEV_BUTTONS] == -1) 
	{
		printf("Error: couldn't find button device\n");
		return -1;
	}
#else
/*    ZeroMemory(&ControllerState, sizeof(XINPUT_STATE));
    DWORD Result = XInputGetState(iControllerNum, &ControllerState);
    if(Result == ERROR_SUCCESS)
    {
		printf("Controller ready\n");
    }
*/
#endif
	return 0;
}
void MYINPUT::Deinit()
{
#ifndef WIN32
	if(ts != 0) ts_close(ts);
	for(int i = 0;i < DEVS_TOTAL;i++)
		if(i != DEV_TS && ifd[i] != -1)
			close(ifd[i]);
	close(fbdev);
#endif
}
void MYINPUT::Process()
{
#ifndef WIN32
	struct input_event ev[64];
	int fd = -1, rd, which, ret,i;
	fd_set fdset;
 
/*	draw_ts_cross(screen, 0x0000, ts_old_x, ts_old_y);
	redraw_keys_lid(screen);
	redraw_nubs(screen, nubx[0], nuby[0], nubx[1], nuby[1]);
	draw_ts_cross(screen, 0xf800, ts_x, ts_y);
*/ 
	FD_ZERO(&fdset);
	for (i = 0; i < DEVS_TOTAL; i++)
		if (ifd[i] != -1)
			FD_SET(ifd[i], &fdset);
 
	timeval tv;
	tv.tv_usec = 1;
	tv.tv_sec = 0;
	ret = select(imaxfd + 1, &fdset, NULL, NULL, &tv);
	if (ret == -1)
	{
		perror("select");
		return;
	}
 
	for (i = 0; i < DEVS_TOTAL; i++)
		if (ifd[i] != -1 && FD_ISSET(ifd[i], &fdset))
			fd = ifd[i];

	if(fd == -1) return; 
	/* touch event? */
	if (fd == ifd[DEV_TS])
	{
		struct ts_sample samp;
		ret = ts_read(ts, &samp, 1);
		if (ret < 0) 
		{
				perror("ts_read");
				return;
		}
		if (ret != 1) return;
		ts_x = samp.x; ts_y = samp.y;
		//printf("ts: %6d %6d %6d\n", samp.x, samp.y, samp.pressure);
		return;
	}
 
	/* buttons or keypad */
	rd = read(fd, ev, sizeof(ev));
	if (rd < (int) sizeof(ev[0])) 
	{
		perror("\nevtest: error reading");
		return;
	}
 
    for (i = 0; i < rd / sizeof(ev[0]); i++)
    {
        switch (ev[i].type) 
		{
        case EV_SYN:
                break;
        case EV_KEY:
			{
				unsigned short c = ev[i].code;
				if(ev[i].value == 1)
					KeyboardDown(c);
				else
					KeyboardUp(c);

				if(c == 54)
				{
					if(ev[i].value == 1)
						m_uiButtons |= SHOULDER_LEFT;
					else
						m_uiButtons &= ~SHOULDER_LEFT;
				} 
				if(c == 97)
				{
					if(ev[i].value == 1)
						m_uiButtons |= SHOULDER_RIGHT;
					else
						m_uiButtons &= ~SHOULDER_RIGHT;
				}
/*                set_key(ev[i].code, ev[i].value);
 
                if (ev[i].code == KEY_RIGHTSHIFT)
                        pressed_l = !!ev[i].value;
                if (ev[i].code == KEY_RIGHTCTRL)
                        pressed_r = !!ev[i].value;
#ifdef VALIDATION
                if (ev[i].code == KEY_MENU && !!ev[i].value) 
				{
                    if (nub_range_hit == 0xff && all_btns_tested && lid_tested && ts_tested)
						return; // goto end;
                    if (pressed_l && pressed_r) 
					{
                        retval = 1;
                        return; // goto end;
                    }
                    early_leave_attempt++;
                }
#endif
                if (ev[i].code == KEY_LEFT)
					sound_do(!!ev[i].value, snd_test_r);
                if (ev[i].code == KEY_RIGHT)
                    sound_do(snd_test_l, !!ev[i].value);
*/
			}break;
        case EV_ABS:
                which = (fd == ifd[DEV_LNUB]) ? 0 : 1;
                if (ev[i].code == ABS_X)
                        nubx[which] = ev[i].value;
                else if (ev[i].code == ABS_Y)
                        nuby[which] = ev[i].value;
                else
                        printf("unexpected EV_ABS code: %i\n", ev[i].code);
				if(which == 0)
					SetNub((float)nubx[which] / 256.0f,-(float)nuby[which] / 256.0f,0);
				else
					SetNub((float)nubx[which] / 256.0f,-(float)nuby[which] / 256.0f,1);
                break;
/*        case EV_SW:
                if (ev[i].code == SW_LID) {
                        lid_closed = ev[i].value;
                        lid_tested |= ev[i].value;
                }
                else
                        printf("unexpected EV_SW code: %i\n", ev[i].code);
                break;
        case EV_MSC:
                if (ev[i].code == MSC_SCAN)
                        break;
        default:
                printf("unexpected event: type %i, code %d\n", ev[i].type, ev[i].code);
                break;
*/
        }
    }
#else
    ZeroMemory(&ControllerState, sizeof(XINPUT_STATE));
    DWORD Result = XInputGetState(iControllerNum, &ControllerState);
    if(Result == ERROR_SUCCESS)
    {
/*XINPUT_GAMEPAD_DPAD_UP          0x00000001
XINPUT_GAMEPAD_DPAD_DOWN        0x00000002
XINPUT_GAMEPAD_DPAD_LEFT        0x00000004
XINPUT_GAMEPAD_DPAD_RIGHT       0x00000008
XINPUT_GAMEPAD_START            0x00000010
XINPUT_GAMEPAD_BACK             0x00000020
XINPUT_GAMEPAD_LEFT_THUMB       0x00000040
XINPUT_GAMEPAD_RIGHT_THUMB      0x00000080
XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100
XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200
XINPUT_GAMEPAD_A                0x1000
XINPUT_GAMEPAD_B                0x2000
XINPUT_GAMEPAD_X                0x4000
XINPUT_GAMEPAD_Y                0x8000
*/
		SetNub((float)ControllerState.Gamepad.sThumbLX / 32768.0f,(float)ControllerState.Gamepad.sThumbLY / 32768.0f,0);
		SetNub((float)ControllerState.Gamepad.sThumbRX / 32768.0f,(float)ControllerState.Gamepad.sThumbRY / 32768.0f,1);

		if(ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) 
			m_uiButtons |= SHOULDER_LEFT;
		else
			m_uiButtons &= ~SHOULDER_LEFT;

		if(ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) 
			m_uiButtons |= SHOULDER_RIGHT;
		else
			m_uiButtons &= ~SHOULDER_RIGHT;
	}
#endif
}
void MYINPUT::SetNub(float fX,float fY,int iNub)
{
	float mn = 0.1f;
	if(fabs(fX) < mn)
		fX = 0;
	else
		fX = ((fabs(fX) - mn) / (1.0f - mn)) * fX / fabs(fX);
	if(fabs(fY) < mn)
		fY = 0;
	else
		fY = ((fabs(fY) - mn) / (1.0f - mn)) * fY / fabs(fY);

	if(iNub == 0)
	{
		NubLeft.fX = fX;
		NubLeft.fY = fY;
	}
	else
	{
		NubRight.fX = fX;
		NubRight.fY = fY;
	}
}
libEngine::MYINPUT::NUB libEngine::MYINPUT::GetNubLeft()
{
	return NubLeft;
}

void MYINPUT::KeyboardDown(unsigned short Key)
{
	m_KeyMap[Key] = 1;
}
void MYINPUT::KeyboardUp(unsigned short Key)
{
	m_KeyMap.erase(Key);
}
bool MYINPUT::IsKeyboardDown(unsigned short Key)
{
	std::map<unsigned short,bool>::iterator it = m_KeyMap.find(Key);
	if(it != m_KeyMap.end())
		return true;
	return false;
}
unsigned int MYINPUT::GetButtons()
{
	return m_uiButtons;
}

