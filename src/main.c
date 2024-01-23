#include <assert.h>
#include <stdbool.h>
#include <windows.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include "globals.h"
#include "window.h"
#include "amogus/amogus.h"
#include "data structures/vec2/vec2.h"

void run_kbd_hook();

LRESULT CALLBACK kbd_hook_proc(int code, WPARAM param1, LPARAM param2);

void kill_kbd_hook();

void minimize_all_windows();

void crash_explorer();

void restore_explorer();

void send_key(uint8_t key, bool down);

int main()
{	// todo remove
	printf("%f\n", vec2_angle_of(vec2_create(0)));
	printf("%f\n", round(vec2_angle_of(vec2_create(1))));
	assert(round(vec2_angle_of(vec2_create_2(150, 0))) == 0);
	assert(round(vec2_angle_of(vec2_create(1))) == 45);
	assert(round(vec2_angle_of(vec2_create(195))) == 45);
	assert(round(vec2_angle_of(vec2_create_2(1, 0))) == 0);
	assert(round(vec2_angle_of(vec2_create_2(-1, 0))) == 180);
	assert(round(vec2_angle_of(vec2_create_2(1, -1))) == -45);

	minimize_all_windows();
	crash_explorer();
	amogus_init();
	logic_init();
	create_window();
	pthread_t thread;
	pthread_create(&thread, NULL, window_interaction_thread_proc, NULL);
	run_kbd_hook();
	receive_window_messages();
	kill_kbd_hook();
	pthread_join(thread, NULL);
	restore_explorer();

	return EXIT_SUCCESS;
}

void run_kbd_hook()
{	g_logic.kbd_hook = SetWindowsHookExA(WH_KEYBOARD_LL, kbd_hook_proc, NULL, 0);
	assert(g_logic.kbd_hook);
}

LRESULT CALLBACK kbd_hook_proc(int code, WPARAM param1, LPARAM param2)
{	assert(IsWindow(g_logic.window));
	if(code != HC_ACTION)
		return CallNextHookEx(NULL, code, param1, param2);
	KBDLLHOOKSTRUCT * kbd_event = (KBDLLHOOKSTRUCT *)param2;
	switch(kbd_event->vkCode)
	{
#pragma push_macro("_")
#define _(lower, upper) case VK_##upper: g_logic.pressed_keys.lower = param1 == WM_KEYDOWN; break;
			_(right, RIGHT)
			_(left, LEFT)
			_(up, UP)
			_(down, DOWN)
			_(escape, ESCAPE)
		case VK_RETURN:
			if(param1 == WM_KEYFIRST)
				g_logic.mouse_follow_mode = !g_logic.mouse_follow_mode;
			break;
#pragma pop_macro("_")
	}
	return CallNextHookEx(NULL, code, param1, param2);
}

void kill_kbd_hook()
{	bool r = UnhookWindowsHookEx(g_logic.kbd_hook);
	assert(r);
}

void minimize_all_windows()
{	enum { VK_A = 0x41 };
	send_key(VK_LWIN, true);
	send_key(VK_A + 'm' - 'a', true);
	send_key(VK_LWIN, false);
	send_key(VK_A + 'm' - 'a', false);
}

void crash_explorer() { system("taskkill /f /IM \"explorer.exe\""); }

void restore_explorer() { system("start explorer.exe"); }

void send_key(uint8_t key, bool down)
{	INPUT input;
	memset(&input, 0, sizeof(INPUT));
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = key;
	input.ki.dwFlags = down ? 0 : KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));
}
