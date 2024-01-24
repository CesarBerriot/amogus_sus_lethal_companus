//
// Created by César B. on 1/24/2024.
//

#include "audio.h"
#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "resources/lethal_company_icecream_music.binh"
#include "resources/vine_boom_sfx.binh"
#include "globals.h"

struct g_audio_struct
{	MCI_OPEN_PARMS args;
	char * path;
} g_audio[AUDIO_MAX];

// MCI_OPEN_PARMSk

void audio_get_path(char * buffer)
{	GetTempPathA(MAX_PATH + 1, buffer);
	// todo unsafe
	strcat(buffer, "amogus_sus_lethal_companus\\");
}

void audio_close_files()
{	char * buffer = malloc(15);
	for(register uint8_t i = 0; i < AUDIO_MAX; ++i)
	{	sprintf(buffer, "close sound%u", i);
		switch(mciSendStringA(buffer, NULL, 0, 0))
		{	case 0:
			case MCIERR_INVALID_DEVICE_NAME: // unknown alias
				break;
			default: assert(0);
		}
	}
}

void audio_init()
{	memset(g_audio, 0, sizeof(struct g_audio_struct) * (AUDIO_MAX - 1));

	char * path = malloc(MAX_PATH + 1);

	g_audio[AUDIO_LETHAL_COMPANY_ICECREAM_MUSIC].path = malloc(MAX_PATH + 1);
	g_audio[AUDIO_VINE_BOOM].path = malloc(MAX_PATH + 1);

	audio_get_path(path);
	strcpy(g_audio[AUDIO_LETHAL_COMPANY_ICECREAM_MUSIC].path, path);
	strcpy(g_audio[AUDIO_VINE_BOOM].path, path);

	mkdir(g_audio[AUDIO_LETHAL_COMPANY_ICECREAM_MUSIC].path);

	char * icecream_music_path = g_audio[AUDIO_LETHAL_COMPANY_ICECREAM_MUSIC].path;
	char * vine_boom_path = g_audio[AUDIO_VINE_BOOM].path;

	// todo unsafe
	strcat(icecream_music_path, "lethal_company_icecream_music.mp3");
	// todo unsafe
	strcat(vine_boom_path, "vine_boom_sfx.mp3");

	FILE * icecream_file = fopen(icecream_music_path, "wb");
	FILE * vine_file = fopen(vine_boom_path, "wb");

	assert(icecream_file);
	assert(vine_file);

	size_t r;

	r = fwrite(lethal_company_icecream_music, 1, lethal_company_icecream_music_len, icecream_file);
	assert(r == lethal_company_icecream_music_len);

	r = fwrite(vine_boom_sfx, 1, vine_boom_sfx_len, vine_file);
	assert(r == vine_boom_sfx_len);

	r = fclose(icecream_file);
	assert(!r);
	r = fclose(vine_file);
	assert(!r);

	free(path);
}

void audio_play_sound(audio_t sound)
{	// MCI_OPEN_PARMS * args = &g_audio[sound].args;
	// memset(args, 0, sizeof(MCI_OPEN_PARMS));
	// args->dwCallback = (WORD)(uintptr_t)g_logic.window;
	// args->wDeviceID = MCI_ALL_DEVICE_ID;
	// args->lpstrDeviceType = "";
	// args->lpstrElementName = g_audio[sound].path;
	// MCIERROR e;
	// e = mciSendCommandA(0, MCI_OPEN, MCI_NOTIFY, (DWORD)(uintptr_t)args);
	// assert(!e);
	audio_close_files();

	MCIERROR e;
	char * prefix = "open \"";
	char * suffix = "\" type mpegvideo alias sound";
	char * command = malloc(strlen(prefix) + strlen(g_audio[sound].path) + strlen(suffix) + 1);
	sprintf(command, "%s%s%s%u", prefix, g_audio[sound].path, suffix, sound);
	e = mciSendStringA(command, NULL, 0, 0);
	MCIERR_DUPLICATE_ALIAS;
	if(e)
		printf("error : %u", e - MCIERR_BASE);
	assert(!e);
	// mciSendStringA(command, NULL, 0, g_logic.window);
	sprintf(command, "play sound%u notify", sound);
	e = mciSendStringA(command, NULL, 0, g_logic.window);
	assert(!e);
	//	sprintf(command, "close sound%u", sound);
	//	e = mciSendStringA("command", NULL, 0, 0);
	//	if(e)
	//	printf("error : %u", e - MCIERR_BASE);
	//	assert(!e);
	free(command);
}

void audio_destroy()
{

}
