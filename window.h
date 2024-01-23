//
// Created by César B. on 1/22/2024.
//

#pragma once

#include <windows.h>

#define WINDOW_CLASS_NAME "custom_window_class"

#define WINDOW_MOVEMENT_SPEED_FACTOR 500
#define WINDOW_ROTATION_SPEED_FACTOR 400
#define WINDOW_REGION_REFRESH_RATE 15 // ms

void * window_interaction_thread_proc(void *);

LRESULT CALLBACK window_proc(HWND window, UINT msg, WPARAM param1, LPARAM param2);

void create_window();

void receive_window_messages();