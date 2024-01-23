//
// Created by César B. on 1/22/2024.
//

#pragma once

#include <windows.h>
#include <stdbool.h>

extern struct g_logic_struct
{	HWND window;
	HHOOK kbd_hook;
	bool mouse_follow_mode;
	struct
	{	bool left, right, up, down, escape;
	} pressed_keys;
} g_logic;
