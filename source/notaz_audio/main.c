/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - main.c                                                  *
 *   Mupen64Plus homepage: http://code.google.com/p/mupen64plus/           *
 *   notaz_audio: (c) notaz, 2010                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/soundcard.h>
#include <stdarg.h>
/*
#include "../main/winlnxdefs.h"

#include "Audio_1.2.h"
*/
#define M64P_PLUGIN_PROTOTYPES 1
#include "m64p_types.h"
#include "m64p_plugin.h"
#include "m64p_common.h"
#include "m64p_config.h"

#include "osal_dynamiclib.h"

/* version info */
#define NOTAZ_AUDIO_PLUGIN_VERSION 0x020000
#define AUDIO_PLUGIN_API_VERSION 0x020000
#define CONFIG_API_VERSION       0x020100
#define CONFIG_PARAM_VERSION     1.00

#define VERSION_PRINTF_SPLIT(x) (((x) >> 16) & 0xffff), (((x) >> 8) & 0xff), ((x) & 0xff)

/* declarations of pointers to Core config functions */
extern ptr_ConfigListSections     ConfigListSections;
extern ptr_ConfigOpenSection      ConfigOpenSection;
extern ptr_ConfigDeleteSection    ConfigDeleteSection;
extern ptr_ConfigSaveSection      ConfigSaveSection;
extern ptr_ConfigListParameters   ConfigListParameters;
extern ptr_ConfigSaveFile         ConfigSaveFile;
extern ptr_ConfigSetParameter     ConfigSetParameter;
extern ptr_ConfigGetParameter     ConfigGetParameter;
extern ptr_ConfigGetParameterHelp ConfigGetParameterHelp;
extern ptr_ConfigSetDefaultInt    ConfigSetDefaultInt;
extern ptr_ConfigSetDefaultFloat  ConfigSetDefaultFloat;
extern ptr_ConfigSetDefaultBool   ConfigSetDefaultBool;
extern ptr_ConfigSetDefaultString ConfigSetDefaultString;
extern ptr_ConfigGetParamInt      ConfigGetParamInt;
extern ptr_ConfigGetParamFloat    ConfigGetParamFloat;
extern ptr_ConfigGetParamBool     ConfigGetParamBool;
extern ptr_ConfigGetParamString   ConfigGetParamString;

/* definitions of pointers to Core config functions */
ptr_ConfigOpenSection      ConfigOpenSection = NULL;
ptr_ConfigDeleteSection    ConfigDeleteSection = NULL;
ptr_ConfigSaveSection      ConfigSaveSection = NULL;
ptr_ConfigSetParameter     ConfigSetParameter = NULL;
ptr_ConfigGetParameter     ConfigGetParameter = NULL;
ptr_ConfigGetParameterHelp ConfigGetParameterHelp = NULL;
ptr_ConfigSetDefaultInt    ConfigSetDefaultInt = NULL;
ptr_ConfigSetDefaultFloat  ConfigSetDefaultFloat = NULL;
ptr_ConfigSetDefaultBool   ConfigSetDefaultBool = NULL;
ptr_ConfigSetDefaultString ConfigSetDefaultString = NULL;
ptr_ConfigGetParamInt      ConfigGetParamInt = NULL;
ptr_ConfigGetParamFloat    ConfigGetParamFloat = NULL;
ptr_ConfigGetParamBool     ConfigGetParamBool = NULL;
ptr_ConfigGetParamString   ConfigGetParamString = NULL;

/* local variables */
static void (*l_DebugCallback)(void *, int, const char *) = NULL;
static void *l_DebugCallContext = NULL;
static int l_PluginInit = 0;
static int l_PausedForSync = 1; /* Audio is started in paused state after SDL initialization */
static m64p_handle l_ConfigAudio;

#undef PLUGIN_VERSION
#define PLUGIN_VERSION "r2"

#define PREFIX "[audio] "
#define log(f, ...) printf(PREFIX f, ##__VA_ARGS__)

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

/* comment from jttl_audio:
 * This sets default frequency what is used if rom doesn't want to change it.
 * Popably only game that needs this is Zelda: Ocarina Of Time Master Quest 
 * *NOTICE* We should try to find out why Demos' frequencies are always wrong
 * They tend to rely on a default frequency, apparently, never the same one ;)*/
#define DEFAULT_FREQUENCY 33600

#define OSS_FRAGMENT_COUNT 5

/* Read header for type definition */
static AUDIO_INFO audio_info;
/* Audio frequency, this is usually obtained from the game,
 * but for compatibility we set default value */
static int input_freq = DEFAULT_FREQUENCY;

/* config stuff */
static int minimum_rate = 8000;
static int pich_percent = 100;

static unsigned int sound_out_buff[48000 * 4]; // ~4 sec, enough?
static unsigned int sound_silence_buff[48000 / 10];

static int sound_dev = -1;
static int output_freq;
static int resample_step;
static int silence_bytes;
static int fade_len;
static int fade_step;

/* rates Pandora supports natively, we'll need to do
 * less resampling if we stick close to these */
static const int supported_rates[] = {
	8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000,
};

static int init(int freq)
{
	static int output_freq_old;
	int stereo, bits, rate;
	int bsize, frag;
	int ret, i;

	input_freq = freq;

	minimum_rate = ConfigGetParamInt(l_ConfigAudio, "MINIMUM_RATE" );
	output_freq = minimum_rate;
	pich_percent = ConfigGetParamInt(l_ConfigAudio, "PITCH_PERCENT" );

	// find lowest alowed rate that is higher than game's output
	for (i = 0; i < ARRAY_SIZE(supported_rates); i++) {
		int rate = supported_rates[i];
		if (freq <= rate + 100 && rate >= minimum_rate) {
			output_freq = rate;
			break;
		}
	}

	if (sound_dev >= 0) {
		if (output_freq == output_freq_old)
			goto finish;
		close(sound_dev);
	}

	sound_dev = open("/dev/dsp", O_WRONLY);
	if (sound_dev == -1) {
		perror(PREFIX "open(\"/dev/dsp\")");
		sound_dev = open("/dev/dsp1", O_WRONLY);
		if (sound_dev == -1) {
			perror(PREFIX "open(\"/dev/dsp1\")");
			return -1;
		}
	}

        bsize = output_freq / 20 * 4; // ~50ms
        for (frag = 0; bsize; bsize >>= 1, frag++)
                ;

        frag |= OSS_FRAGMENT_COUNT << 16;       // fragment count
        ret = ioctl(sound_dev, SNDCTL_DSP_SETFRAGMENT, &frag);
        if (ret < 0)
                perror(PREFIX "SNDCTL_DSP_SETFRAGMENT failed");

	silence_bytes = output_freq / 30 * 4; // ~ 25ms
	memset(sound_silence_buff, 0, sizeof(sound_silence_buff));

	stereo = 1;
        bits = 16;
	rate = output_freq;
        ret = ioctl(sound_dev, SNDCTL_DSP_STEREO, &stereo);
        if (ret == 0)
                ret = ioctl(sound_dev, SNDCTL_DSP_SETFMT, &bits);
        if (ret == 0)
                ret = ioctl(sound_dev, SNDCTL_DSP_SPEED, &rate);
        if (ret < 0)
                perror(PREFIX "failed to set audio format");

	if (rate != output_freq)
		log("warning: output rate %d differs from desired %d\n", rate, output_freq);

finish:
	resample_step = freq * 1024 / output_freq;
	if (pich_percent != 100)
		resample_step = resample_step * pich_percent / 100;
	fade_len = output_freq / 1000;
	fade_step = 64 * 1024 / fade_len;

	log("(re)started, rates: %d -> %d\n", freq, output_freq);

	return 0;
}

static unsigned int resample(unsigned int *input, int in_size, unsigned int *output, int out_size)
{
	unsigned int i, j, t;
	int step = resample_step;
	int count = 0;

	t = input[0];
	t = (t << 16) | (t >> 16);

	for (i = j = 0; i < out_size / 4;)
	{
		output[i++] = t;

		count += step;
		while (count >= 1024) {
			count -= 1024;
			j++;
			if (j >= in_size / 4)
				goto out;
			t = input[j];
			t = (t << 16) | (t >> 16);
		}
	}

out:
	return i * 4; // number of bytes to output
}

static void fade(unsigned int *buf, int buf_len, int up)
{
	signed short *sb = (void *)buf;
	int len = fade_len;
	int step = fade_step;
	int counter = 0, mult, mult_step;
	int i;

	if (up) {
		mult = 0;
		mult_step = 1;
	} else {
		sb += buf_len / 2 - len * 2;
		if (sb < (signed short *)buf)
			sb = (void *)buf;
		mult = 63;
		mult_step = -1;
	}

	for (i = 0; i < len; i++) {
		counter += step;
		while (counter >= 1024) {
			counter -= 1024;
			mult += mult_step;
		}
		sb[i * 2] = sb[i * 2] / 64 * mult;
		sb[i * 2 + 1] = sb[i * 2] / 64 * mult;
	}
}

EXPORT void CALL AiDacrateChanged( int SystemType )
{
	int f = input_freq;
	switch (SystemType)
	{
	case SYSTEM_NTSC:
		f = 48681812 / (*audio_info.AI_DACRATE_REG + 1);
		break;
	case SYSTEM_PAL:
		f = 49656530 / (*audio_info.AI_DACRATE_REG + 1);
		break;
	case SYSTEM_MPAL:
		f = 48628316 / (*audio_info.AI_DACRATE_REG + 1);
		break;
	}
	init(f);
}

EXPORT void CALL AiLenChanged(void)
{
	static int had_uflow;
	unsigned int ret, len, *in, *part2, part2_len;
	audio_buf_info bi;

	if (sound_dev < 0)
		return;

	// XXX: what about alignment? len limit? rdram overflow?
	in = (unsigned int *)(audio_info.RDRAM + (*audio_info.AI_DRAM_ADDR_REG & 0xFFFFFC));
	len = *audio_info.AI_LEN_REG;

	//log("AiLenChanged: %u\n", len);

	ret = resample(in, len, sound_out_buff, sizeof(sound_out_buff));
	if (ret >= sizeof(sound_out_buff))
		log("overflow, in_len=%d\n", len);

	if (had_uflow)
		fade(sound_out_buff, ret / 2, 1);

	write(sound_dev, sound_out_buff, ret / 2);
	part2 = sound_out_buff + (ret / 4) / 2;
	part2_len = ret - ret / 2;

	// try to keep at most 2 free fragments to avoid
	// hardware underflows that cause crackling on pandora
	// XXX: for some reason GETOSPACE only works after write?
	// XXX: .fragments sometimes overflows? ALSA OSS emu bugs?
	ret = ioctl(sound_dev, SNDCTL_DSP_GETOSPACE, &bi);
	if (ret == 0 && 2 < bi.fragments && bi.fragments <= OSS_FRAGMENT_COUNT) {
		fade(part2, part2_len, 0);
		write(sound_dev, part2, part2_len);
		write(sound_dev, sound_silence_buff, silence_bytes);
		if (bi.fragments == 4)
			write(sound_dev, sound_silence_buff, silence_bytes);
		had_uflow = 1;
	}
	else {
		write(sound_dev, part2, part2_len);
		had_uflow = 0;
	}
}

/*
EXPORT DWORD CALL AiReadLength(void)
{
	return 0;
}
EXPORT void CALL CloseDLL(void)
{
	if (sound_dev >= 0)
		close(sound_dev);
	sound_dev = -1;
}
*/
static char config_file[512];

static void read_config(void)
{
	char cfg[512], *p;
	int val;
	FILE *f;

	f = fopen(config_file, "r");
	if (f == NULL) {
		perror(PREFIX "can't open config");
		return;
	}

	while (1) {
		p = fgets(cfg, sizeof(cfg), f);
		if (p == NULL)
			break;
		if (p[0] == '#')
			continue;

		if (sscanf(p, "pich_percent = %d", &val) == 1 && 10 <= val && val <= 1000) {
			pich_percent = val;
			break;
		}
		if (sscanf(p, "minimum_rate = %d", &val) == 1) {
			minimum_rate = val;
			break;
		}
	}
	fclose(f);
}
/*
EXPORT void CALL SetConfigDir(char *configDir)
{
	snprintf(config_file, sizeof(config_file), "%snotaz_audio.conf", configDir);
	read_config();
}

EXPORT void CALL DllTest(HWND hParent)
{
}

EXPORT void CALL GetDllInfo(PLUGIN_INFO *PluginInfo)
{
	PluginInfo->Version = 0x0101;
	PluginInfo->Type    = PLUGIN_TYPE_AUDIO;
	strcpy(PluginInfo->Name, "notaz's OSS audio " PLUGIN_VERSION);
	PluginInfo->NormalMemory  = TRUE;
	PluginInfo->MemoryBswaped = TRUE;
}

EXPORT void CALL DllAbout(HWND hParent)
{
}

EXPORT void CALL DllConfig(HWND hParent)
{
	char cmd[512];
	FILE *f;

	f = fopen(config_file, "r");
	if (f != NULL)
		fclose(f);
	else {
		f = fopen(config_file, "w");
		if (f != NULL) {
			fprintf(f, "# minimum sample rate to use. Higher values sound better on Pandora's DAC.\n");
			fprintf(f, "minimum_rate = %d\n\n", minimum_rate);
			fprintf(f, "# sound playback speed compared to normal (10-200%%)\n");
			fprintf(f, "# this will affect gameplay speed and sound pitch\n");
			fprintf(f, "pich_percent = %d\n\n", pich_percent);
			fclose(f);
		}
	}

	snprintf(cmd, sizeof(cmd), "mousepad \"%s\"", config_file);
	system(cmd);

	read_config();
}
*/
EXPORT int CALL InitiateAudio(AUDIO_INFO Audio_Info)
{
    if (!l_PluginInit)
        return 0;

	audio_info = Audio_Info;
	return 1;
}

EXPORT int CALL RomOpen(void)
{
    if (!l_PluginInit)
        return 0;

	/* This function is for compatibility with Mupen64. */
	init(input_freq);
	return 1;
}

EXPORT void CALL RomClosed(void)
{
}

EXPORT void CALL ProcessAList(void)
{
}

EXPORT void CALL SetSpeedFactor(int percentage)
{
}

EXPORT void CALL VolumeUp(void)
{
}

EXPORT void CALL VolumeDown(void)
{
}

EXPORT void CALL VolumeMute(void)
{
}
EXPORT int CALL VolumeGetLevel(void)
{
    return 255;
}

EXPORT void CALL VolumeSetLevel(int level)
{
}

EXPORT const char * CALL VolumeGetString(void)
{
	return "100%%";
}

/* Global functions */
static void DebugMessage(int level, const char *message, ...)
{
  char msgbuf[1024];
  va_list args;

  if (l_DebugCallback == NULL)
      return;

  va_start(args, message);
  vsprintf(msgbuf, message, args);

  (*l_DebugCallback)(l_DebugCallContext, level, msgbuf);

  va_end(args);
}

/* Mupen64Plus plugin functions */
EXPORT m64p_error CALL PluginStartup(m64p_dynlib_handle CoreLibHandle, void *Context,
                                   void (*DebugCallback)(void *, int, const char *))
{
    ptr_CoreGetAPIVersions CoreAPIVersionFunc;
    
    int ConfigAPIVersion, DebugAPIVersion, VidextAPIVersion, bSaveConfig;
    float fConfigParamsVersion = 0.0f;
    
    if (l_PluginInit)
        return M64ERR_ALREADY_INIT;

    /* first thing is to set the callback function for debug info */
    l_DebugCallback = DebugCallback;
    l_DebugCallContext = Context;

    /* attach and call the CoreGetAPIVersions function, check Config API version for compatibility */
    CoreAPIVersionFunc = (ptr_CoreGetAPIVersions) osal_dynlib_getproc(CoreLibHandle, "CoreGetAPIVersions");
    if (CoreAPIVersionFunc == NULL)
    {
        DebugMessage(M64MSG_ERROR, "Core emulator broken; no CoreAPIVersionFunc() function found.");
        return M64ERR_INCOMPATIBLE;
    }
    
    (*CoreAPIVersionFunc)(&ConfigAPIVersion, &DebugAPIVersion, &VidextAPIVersion, NULL);
    if ((ConfigAPIVersion & 0xffff0000) != (CONFIG_API_VERSION & 0xffff0000))
    {
        DebugMessage(M64MSG_ERROR, "Emulator core Config API (v%i.%i.%i) incompatible with plugin (v%i.%i.%i)",
                VERSION_PRINTF_SPLIT(ConfigAPIVersion), VERSION_PRINTF_SPLIT(CONFIG_API_VERSION));
        return M64ERR_INCOMPATIBLE;
    }

    /* Get the core config function pointers from the library handle */
    ConfigOpenSection = (ptr_ConfigOpenSection) osal_dynlib_getproc(CoreLibHandle, "ConfigOpenSection");
    ConfigDeleteSection = (ptr_ConfigDeleteSection) osal_dynlib_getproc(CoreLibHandle, "ConfigDeleteSection");
    ConfigSaveSection = (ptr_ConfigSaveSection) osal_dynlib_getproc(CoreLibHandle, "ConfigSaveSection");
    ConfigSetParameter = (ptr_ConfigSetParameter) osal_dynlib_getproc(CoreLibHandle, "ConfigSetParameter");
    ConfigGetParameter = (ptr_ConfigGetParameter) osal_dynlib_getproc(CoreLibHandle, "ConfigGetParameter");
    ConfigSetDefaultInt = (ptr_ConfigSetDefaultInt) osal_dynlib_getproc(CoreLibHandle, "ConfigSetDefaultInt");
    ConfigSetDefaultFloat = (ptr_ConfigSetDefaultFloat) osal_dynlib_getproc(CoreLibHandle, "ConfigSetDefaultFloat");
    ConfigSetDefaultBool = (ptr_ConfigSetDefaultBool) osal_dynlib_getproc(CoreLibHandle, "ConfigSetDefaultBool");
    ConfigSetDefaultString = (ptr_ConfigSetDefaultString) osal_dynlib_getproc(CoreLibHandle, "ConfigSetDefaultString");
    ConfigGetParamInt = (ptr_ConfigGetParamInt) osal_dynlib_getproc(CoreLibHandle, "ConfigGetParamInt");
    ConfigGetParamFloat = (ptr_ConfigGetParamFloat) osal_dynlib_getproc(CoreLibHandle, "ConfigGetParamFloat");
    ConfigGetParamBool = (ptr_ConfigGetParamBool) osal_dynlib_getproc(CoreLibHandle, "ConfigGetParamBool");
    ConfigGetParamString = (ptr_ConfigGetParamString) osal_dynlib_getproc(CoreLibHandle, "ConfigGetParamString");

    if (!ConfigOpenSection || !ConfigDeleteSection || !ConfigSetParameter || !ConfigGetParameter ||
        !ConfigSetDefaultInt || !ConfigSetDefaultFloat || !ConfigSetDefaultBool || !ConfigSetDefaultString ||
        !ConfigGetParamInt   || !ConfigGetParamFloat   || !ConfigGetParamBool   || !ConfigGetParamString)
        return M64ERR_INCOMPATIBLE;

    /* ConfigSaveSection was added in Config API v2.1.0 */
    if (ConfigAPIVersion >= 0x020100 && !ConfigSaveSection)
        return M64ERR_INCOMPATIBLE;

    /* get a configuration section handle */
    if (ConfigOpenSection("Audio-Notaz", &l_ConfigAudio) != M64ERR_SUCCESS)
    {
        DebugMessage(M64MSG_ERROR, "Couldn't open config section 'Audio-Notaz'");
        return M64ERR_INPUT_NOT_FOUND;
    }

    /* check the section version number */
    bSaveConfig = 0;
    if (ConfigGetParameter(l_ConfigAudio, "Version", M64TYPE_FLOAT, &fConfigParamsVersion, sizeof(float)) != M64ERR_SUCCESS)
    {
        DebugMessage(M64MSG_WARNING, "No version number in 'Audio-Notaz' config section. Setting defaults.");
        ConfigDeleteSection("Audio-Notaz");
        ConfigOpenSection("Audio-Notaz", &l_ConfigAudio);
        bSaveConfig = 1;
    }
    else if (((int) fConfigParamsVersion) != ((int) CONFIG_PARAM_VERSION))
    {
        DebugMessage(M64MSG_WARNING, "Incompatible version %.2f in 'Audio-Notaz' config section: current is %.2f. Setting defaults.", fConfigParamsVersion, (float) CONFIG_PARAM_VERSION);
        ConfigDeleteSection("Audio-Notaz");
        ConfigOpenSection("Audio-Notaz", &l_ConfigAudio);
        bSaveConfig = 1;
    }
    else if ((CONFIG_PARAM_VERSION - fConfigParamsVersion) >= 0.0001f)
    {
        /* handle upgrades */
        float fVersion = CONFIG_PARAM_VERSION;
        ConfigSetParameter(l_ConfigAudio, "Version", M64TYPE_FLOAT, &fVersion);
        DebugMessage(M64MSG_INFO, "Updating parameter set version in 'Audio-Notaz' config section to %.2f", fVersion);
        bSaveConfig = 1;
    }

    /* set the default values for this plugin */
    ConfigSetDefaultFloat(l_ConfigAudio, "Version",             CONFIG_PARAM_VERSION,  "Mupen64Plus Notaz Audio Plugin config parameter version number");
    ConfigSetDefaultInt(l_ConfigAudio, "MINIMUM_RATE",        44100,           "Minimum sample rate to use. Higher values sound better on Pandora's DAC.");
    ConfigSetDefaultInt(l_ConfigAudio, "PITCH_PERCENT",   100,   "sound playback speed compared to normal (10-200%%). this will affect gameplay speed and sound pitch");

    if (bSaveConfig && ConfigAPIVersion >= 0x020100)
        ConfigSaveSection("Audio-SDL");

    l_PluginInit = 1;
    return M64ERR_SUCCESS;
}

EXPORT m64p_error CALL PluginShutdown(void)
{
    if (!l_PluginInit)
        return M64ERR_NOT_INIT;

    /* reset some local variables */
    l_DebugCallback = NULL;
    l_DebugCallContext = NULL;

	if (sound_dev >= 0)
		close(sound_dev);
	sound_dev = -1;

    l_PluginInit = 0;
	log("Notaz-audio Plugin shutdown\n");
    return M64ERR_SUCCESS;
}


EXPORT m64p_error CALL PluginGetVersion(m64p_plugin_type *PluginType, int *PluginVersion, int *APIVersion, const char **PluginNamePtr, int *Capabilities)
{
    /* set version info */
    if (PluginType != NULL)
        *PluginType = M64PLUGIN_AUDIO;

    if (PluginVersion != NULL)
        *PluginVersion = NOTAZ_AUDIO_PLUGIN_VERSION;

    if (APIVersion != NULL)
        *APIVersion = AUDIO_PLUGIN_API_VERSION;
    
    if (PluginNamePtr != NULL)
        *PluginNamePtr = "Mupen64Plus Notaz Audio Plugin";

    if (Capabilities != NULL)
    {
        *Capabilities = 0;
    }
                    
    return M64ERR_SUCCESS;
}
