//
// Created by César B. on 1/22/2024.
//

#include "globals.h"

struct g_logic_struct g_logic;

void logic_init()
{	memset(&g_logic, 0, sizeof(struct g_logic_struct));
	g_logic.mouse_follow_mode = true;
}
