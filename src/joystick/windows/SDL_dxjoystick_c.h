/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2012 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
#include "SDL_config.h"

#ifndef SDL_JOYSTICK_DINPUT_H

/* DirectInput joystick driver; written by Glenn Maynard, based on Andrei de
 * A. Formiga's WINMM driver. 
 *
 * Hats and sliders are completely untested; the app I'm writing this for mostly
 * doesn't use them and I don't own any joysticks with them. 
 *
 * We don't bother to use event notification here.  It doesn't seem to work
 * with polled devices, and it's fine to call IDirectInputDevice2_GetDeviceData and
 * let it return 0 events. */

#include "../../core/windows/SDL_windows.h"

#define DIRECTINPUT_VERSION 0x0800      /* Need version 7 for force feedback. Need verison 8 so IDirectInput8_EnumDevices doesn't leak like a sieve... */
#include <dinput.h>
#define COBJMACROS
#include <wbemcli.h>
#include <oleauto.h>
#include <xinput.h>
#include <devguid.h>
#include <dbt.h>

#define MAX_INPUTS	256     /* each joystick can have up to 256 inputs */


/* local types */
typedef enum Type
{ BUTTON, AXIS, HAT } Type;

typedef struct input_t
{
    /* DirectInput offset for this input type: */
    DWORD ofs;

    /* Button, axis or hat: */
    Type type;

    /* SDL input offset: */
    Uint8 num;
} input_t;

/* typedef's for XInput structs we use */
typedef struct
{
	WORD                                wButtons;
	BYTE                                bLeftTrigger;
	BYTE                                bRightTrigger;
	SHORT                               sThumbLX;
	SHORT                               sThumbLY;
	SHORT                               sThumbRX;
	SHORT                               sThumbRY;
	DWORD								dwPaddingReserved;
} XINPUT_GAMEPAD_EX;

typedef struct 
{
	DWORD                               dwPacketNumber;
	XINPUT_GAMEPAD_EX                   Gamepad;
} XINPUT_STATE_EX;

/* The private structure used to keep track of a joystick */
struct joystick_hwdata
{
    LPDIRECTINPUTDEVICE8 InputDevice;
    DIDEVCAPS Capabilities;
    int buffered;
	SDL_JoystickGUID guid;

    input_t Inputs[MAX_INPUTS];
    int NumInputs;
	int NumSliders;
	Uint8 removed;
	Uint8 send_remove_event;
	Uint8 bXInputDevice; // 1 if this device supports using the xinput API rather than DirectInput
	Uint8 userid; // XInput userid index for this joystick
	Uint8 currentXInputSlot; // the current position to write to in XInputState below, used so we can compare old and new values
	XINPUT_STATE_EX	XInputState[2];
};

#endif /* SDL_JOYSTICK_DINPUT_H */
