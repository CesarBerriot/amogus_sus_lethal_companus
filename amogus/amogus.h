//
// Created by César B. on 1/22/2024.
//

#pragma once

#include <windows.h>

enum
{
	AMOGUS_POINTS_LEN = 25,
	AMOGUS_SCALE = 3,
};

extern struct g_amogus_struct
{
	POINT points_original[AMOGUS_POINTS_LEN];
	POINT points[AMOGUS_POINTS_LEN];
	size_t size; // pixels
	double angle;
} g_amogus;

void amogus_init();

void amogus_rotate(float angle);

POINT get_amogus_center();
