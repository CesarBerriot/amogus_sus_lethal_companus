//
// Created by César B. on 1/22/2024.
//

#include "amogus.h"
#include <stdbool.h>
#include <math.h>
#include "data structures/vec2/vec2.h"

struct g_amogus_struct g_amogus = {
	{}
	, {
		{ 16, 0 }
		, { 25, 0 }
		, { 29, 3 }
		, { 29, 6 }
		, { 31, 9 }
		, { 35, 9 }
		, { 37, 12 }
		, { 37, 22 }
		, { 34, 25 }
		, { 30, 25 }
		, { 29, 31 }
		, { 26, 35 }
		, { 24, 35 }
		, { 21, 32 }
		, { 17, 32 }
		, { 14, 35 }
		, { 12, 35 }
		, { 10, 31 }
		, { 10, 17 }
		, { 4, 17 }
		, { 0, 14 }
		, { 0, 11 }
		, { 6, 8 }
		, { 13, 8 }
		, { 14, 3 }
	}
	, 0, 0
};

void amogus_init()
{
	bool is_width_larger; // whether the width is larger than the height
	size_t width = 0, height = 0;

	for(size_t i = 0; i < AMOGUS_POINTS_LEN; ++i)
	{
		g_amogus.points[i].x *= AMOGUS_SCALE;
		g_amogus.points[i].y *= AMOGUS_SCALE;
		if(g_amogus.points[i].x > width)
			width = g_amogus.points[i].x;
		if(g_amogus.points[i].y > height)
			height = g_amogus.points[i].y;
	}

	g_amogus.size = vec2_len(vec2_create_2(width, height));

	for(size_t i = 0; i < AMOGUS_POINTS_LEN; ++i)
	{
		g_amogus.points[i].y += is_width_larger * (width - height);// + g_amogus.size;
		g_amogus.points[i].x += !is_width_larger * (height - width);// + g_amogus.size;
	}
	//g_amogus.size *= 5;
	memcpy(g_amogus.points_original, g_amogus.points, sizeof(POINT) * AMOGUS_POINTS_LEN);
}

void amogus_rotate(float angle)
{
	static bool passed = false;
	if(!angle)// || passed)
		return;
	POINT center_point = get_amogus_center();
	struct vec2 center = vec2_create_2(center_point.x, center_point.y);
	for(size_t i = 0; i < AMOGUS_POINTS_LEN; ++i)
	{
//		double i_rad = i * 10 * DEG_TO_RAD;
//		double i_sin = sin(i_rad);
//		g_amogus.points[i].x = center.x + i * 10;
//		g_amogus.points[i].y = center.y + i * 10;
//		g_amogus.points[i].x = center.x + sin(i * 10 * DEG_TO_RAD) * 100;
//		g_amogus.points[i].y = center.y + cos(i * 10 * DEG_TO_RAD) * 100;

		struct vec2 point = vec2_create_2(g_amogus.points_original[i].x, g_amogus.points_original[i].y);
//		double point_len = vec2_len(point);
		struct vec2 point_relative = vec2_sub_2(point, center);
		// todo find out why in the fuck I have to do that
		point_relative.x = -point_relative.x;
		double point_relative_len = vec2_len(point_relative);
//		assert(point_relative_len);
		double new_angle = vec2_angle_of(point_relative) - angle - g_amogus.angle - 90;
		double new_angle_rad = new_angle * DEG_TO_RAD;
		struct vec2 rotated_vector = vec2_normalize(vec2_create_2(sin(new_angle_rad), cos(new_angle_rad)));
		assert(round(vec2_len(rotated_vector)) == 1);
		struct vec2 point_relative_rotated = vec2_mult(rotated_vector, point_relative_len);
		double new_len = vec2_len(point_relative_rotated);
		assert(round(vec2_len(point_relative_rotated)) == round(point_relative_len));
		point = vec2_add_2(center, point_relative_rotated);

		//point.x = center.x + sin(i * (angle + (360. / AMOGUS_POINTS_LEN)) * DEG_TO_RAD) * 200;//point_relative_len;
		//point.y = center.y + cos(i * (angle + (360. / AMOGUS_POINTS_LEN)) * DEG_TO_RAD) * 200;//point_relative_len;
		g_amogus.points[i].x = point.x;
		g_amogus.points[i].y = point.y;
	}
	//g_amogus.points[0] = center_point;
	passed = true;
	g_amogus.angle += angle;
}

POINT get_amogus_center()
{
	size_t x = 0, y = 0;
	for(size_t i = 0; i < AMOGUS_POINTS_LEN; ++i)
	{
		x += g_amogus.points_original[i].x;
		y += g_amogus.points_original[i].y;
	}
	x /= AMOGUS_POINTS_LEN;
	y /= AMOGUS_POINTS_LEN;
	POINT r = { x, y };
	return r;
}
