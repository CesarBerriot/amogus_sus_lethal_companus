//
// Created by César B. on 1/23/2024.
//

/// this header is included by default in all files within the software

#pragma once

#define CUSTOM_WINDOW_MESSAGE(x) RegisterWindowMessageA(__##x##_STR)

#define __WM_DESTRUCTION_REQUIREMENT_STR "WM_DESTRUCTION_REQUIREMENT-{B02C92EA-817F-4AD0-9992-898C21416825}"

#define RAD_TO_DEG 57.2958
#define DEG_TO_RAD 0.017453
