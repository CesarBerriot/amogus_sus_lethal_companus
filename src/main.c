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
	amogus_init();
	ZeroMemory(&g_logic, sizeof(struct g_logic_struct));
	create_window();
	pthread_t thread;
	pthread_create(&thread, NULL, window_interaction_thread_proc, NULL);
	run_kbd_hook();
	receive_window_messages();
	kill_kbd_hook();
	pthread_join(thread, NULL);
}

void run_kbd_hook()
{	g_logic.kbd_hook = SetWindowsHookExA(WH_KEYBOARD_LL, kbd_hook_proc, NULL, 0);
	assert(g_logic.kbd_hook);
}

LRESULT CALLBACK kbd_hook_proc(int code, WPARAM param1, LPARAM param2)
{	if(code != HC_ACTION)
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

void send_key(uint8_t key, bool down)
{	INPUT input;
	memset(&input, 0, sizeof(INPUT));

}
