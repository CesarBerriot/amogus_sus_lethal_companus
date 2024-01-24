//
// Created by César B. on 1/24/2024.
//

#pragma once

typedef enum { AUDIO_LETHAL_COMPANY_ICECREAM_MUSIC, AUDIO_VINE_BOOM, AUDIO_MAX } audio_t;

void audio_init();

void audio_play_sound(audio_t sound);

void audio_destroy();
