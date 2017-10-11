void release_sound();
void init_sound();

void place_sound(int i, float x, float y);
void activate_sound(int i);
void deactivate_sound(int i);

//for sound effect
void play_effect(int i);
void stop_effect(int i);

void set_playerpos(float x, float y, float dirx, float diry);

enum ReverbType {rt_off,rt_plain,rt_cave,rt_room,rt_forest,rt_hallway,rt_stonecorridor,rt_stoneroom,rt_psycho};
bool set_reverb(ReverbType reverbtype);

void sound_walk(bool walking);

void set_player(int p);

//these declared somewhere else entirely
void addsprite(double xp, double yp, int t);
void removesprites(int t);

//FORMERLY WAS THE CPP FILE NOW BELOW

#include <iostream>
#include <math.h>
using namespace std;

#include "fmod/fmod.h"
#include "fmod/fmod_errors.h"

#define DISTANCEFACTOR       0.5f    // Units per meter.  I.e feet would = 3.28.  centimeters would = 100.

int curvertex=0;

FMOD_VECTOR      listenerpos;

FMOD_VECTOR forward        = { 1.0f, 0.0f, 0.0f };
FMOD_VECTOR up             = { 0.0f, 0.0f, 1.0f };

FMOD_SYSTEM* fmod_system;



unsigned int      version;

namespace {
	int player=0;
} 

ReverbType oldreverb[2];

const int SOUNDCOUNT=5;
const int EFFECTCOUNT=45;
char* walk_path[2] = {"data/sound/steps1.ogg","data/sound/steps2.ogg"};
char* sound_path[SOUNDCOUNT] = {"data/sound/ambientscream.ogg","data/sound/clocktick.ogg","data/sound/dooropen.wav","data/sound/caseopen.wav","data/sound/ambientscream.ogg"};
char* effect_path[EFFECTCOUNT] =  {
"data/sound/0.ogg",
"data/sound/1.ogg",
"data/sound/2.ogg",
"data/sound/3.ogg",
"data/sound/4.ogg",
"data/sound/5.ogg",
"data/sound/6.ogg",
"data/sound/7.ogg",
"data/sound/8.ogg",
"data/sound/9.ogg",
"data/sound/10.ogg",
"data/sound/11.ogg",
"data/sound/12.ogg",
"data/sound/13.ogg",
"data/sound/14.ogg",
"data/sound/15.ogg",
"data/sound/16.ogg",
"data/sound/17.ogg",
"data/sound/18.ogg",
"data/sound/19.ogg",
"data/sound/20.ogg",
"data/sound/21.ogg",
"data/sound/22.ogg",
"data/sound/23.ogg",
"data/sound/24.ogg",
"data/sound/25.ogg",
"data/sound/26.ogg",
"data/sound/27.ogg",
"data/sound/28.ogg",
"data/sound/29.ogg",
"data/sound/30.ogg",
"data/sound/31.ogg",
"data/sound/32.ogg",
"data/sound/33.ogg",
"data/sound/34.ogg",
"data/sound/35.ogg",
"data/sound/36.ogg",
"data/sound/37.ogg",
"data/sound/38.ogg",
"data/sound/39.ogg",
"data/sound/40.ogg",
"data/sound/41.ogg",
"data/sound/42.ogg",
"data/sound/43.ogg",
"data/sound/scream.ogg"};

FMOD_SOUND* fmod_effect[EFFECTCOUNT];
FMOD_SOUND* fmod_walk[2];
FMOD_CHANNEL* effect_channel[EFFECTCOUNT];


FMOD_SOUND* fmod_sound[SOUNDCOUNT];

FMOD_CHANNEL* sound_channel[SOUNDCOUNT];

FMOD_CHANNEL* effect_walkchannel;


FMOD_VECTOR pos [SOUNDCOUNT] = { -10.0f * DISTANCEFACTOR, 0.0f, 0.0f };
FMOD_VECTOR vel = {  0.0f, 0.0f, 0.0f };

float t=0;
void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }
}

void stopchannels()
{
	for (int i=0;i<EFFECTCOUNT;i++)
	{
		FMOD_Channel_Stop(effect_channel[i]);
	}
	for (int i=0;i<SOUNDCOUNT;i++)
	{
		FMOD_Channel_Stop(sound_channel[i]);
	}
	FMOD_Channel_SetPaused(effect_walkchannel,true);
}

void
init_sound()
{
	FMOD_RESULT      result;
	result = FMOD_System_Create(&fmod_system);
	FMOD_SPEAKERMODE speakermode;
    FMOD_CAPS        caps;
	char             name[256];
	
	int num_drivers;
	FMOD_OUTPUTTYPE output_type;
	
    ERRCHECK(result);
	
    result = FMOD_System_GetVersion(fmod_system, &version);
    ERRCHECK(result);
	
    if (version < FMOD_VERSION)
    {
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
    }
	
	    /* If we try to use OSS when there ain't any, fmod bizarrely
	      * complains about an invalid parameter */
	    result = FMOD_System_GetNumDrivers(fmod_system, &num_drivers);
	    ERRCHECK(result);
	    if( 0 == num_drivers ) {
		      fprintf(stderr, "Cannot find any sound drivers with this device.\n");
		
		      result = FMOD_System_GetOutput(fmod_system, &output_type);
		      ERRCHECK(result);
		
		      if( FMOD_OUTPUTTYPE_OSS == output_type ) {
			        fprintf(stderr, "Trying ALSA output instead.\n");
			        result = FMOD_System_SetOutput(fmod_system, FMOD_OUTPUTTYPE_ALSA);
			        ERRCHECK(result);
		      }
	    }
	
    result = FMOD_System_GetDriverCaps(fmod_system, 0, &caps, 0, 0, &speakermode);
    ERRCHECK(result);
	
    result = FMOD_System_SetSpeakerMode(fmod_system, speakermode);       /* Set the user selected speaker mode. */
    ERRCHECK(result);
	
    if (caps & FMOD_CAPS_HARDWARE_EMULATED)             /* The user has the 'Acceleration' slider set to off!  This is really bad for latency!. */
    {                                                   /* You might want to warn the user about this. */
        result = FMOD_System_SetDSPBufferSize(fmod_system, 1024, 10);
        ERRCHECK(result);
    }
	
    result = FMOD_System_GetDriverInfo(fmod_system, 0, name, 256, 0);
    ERRCHECK(result);
	
    if (strstr(name, "SigmaTel"))   /* Sigmatel sound devices crackle for some reason if the format is PCM 16bit.  PCM floating point output seems to solve it. */
    {
        result = FMOD_System_SetSoftwareFormat(fmod_system, 48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, FMOD_DSP_RESAMPLER_LINEAR);
        ERRCHECK(result);
    }
	
    result = FMOD_System_Init(fmod_system, 100, /*FMOD_INIT_ENABLE_PROFILE*/ FMOD_INIT_NORMAL, 0);
    if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)         /* Ok, the speaker mode selected isn't supported by this soundcard.  Switch it back to stereo... */
    {
        result = FMOD_System_SetSpeakerMode(fmod_system, FMOD_SPEAKERMODE_STEREO);
        ERRCHECK(result);
		
        result = FMOD_System_Init(fmod_system, 100, FMOD_INIT_NORMAL, 0);/* ... and re-init. */
        ERRCHECK(result);
    }
	
    /*
	 Set the distance units. (meters/feet etc).
	 */
    result = FMOD_System_Set3DSettings(fmod_system, 1.0, DISTANCEFACTOR, 1.0f);
    ERRCHECK(result);
	
	
	for (int i=0;i<EFFECTCOUNT;i++)
	{
		effect_channel[i]=0;
		FMOD_System_CreateSound(fmod_system,effect_path[i], FMOD_SOFTWARE | FMOD_2D|FMOD_CREATESTREAM, 0, &(fmod_effect[i]));
		if (i==0)
			FMOD_Sound_SetMode(fmod_effect[i], FMOD_LOOP_NORMAL);
		else
			FMOD_Sound_SetMode(fmod_effect[i], FMOD_LOOP_OFF);
	}
	
	effect_walkchannel=0;
	for (int i=0;i<2;i++)
	{
		FMOD_System_CreateSound(fmod_system,walk_path[i], FMOD_SOFTWARE | FMOD_2D, 0, &(fmod_walk[i]));
		FMOD_Sound_SetMode(fmod_walk[i], FMOD_LOOP_NORMAL);
	}
	
	for (int i=0;i<SOUNDCOUNT;i++)
	{
		sound_channel[i]=0;
		FMOD_System_CreateSound(fmod_system,sound_path[i], FMOD_SOFTWARE | FMOD_3D, 0, &(fmod_sound[i]));
		if (i==1)//clock
		{
			FMOD_Sound_Set3DMinMaxDistance(fmod_sound[i], 1.0f * DISTANCEFACTOR, 20000.0f * DISTANCEFACTOR);
		}
		else
			FMOD_Sound_Set3DMinMaxDistance(fmod_sound[i], 1.0f * DISTANCEFACTOR, 20000.0f * DISTANCEFACTOR);
		if (i==0||i==1)
			FMOD_Sound_SetMode(fmod_sound[i], FMOD_LOOP_NORMAL);
		else
			FMOD_Sound_SetMode(fmod_sound[i], FMOD_LOOP_OFF);
		
		FMOD_System_PlaySound(fmod_system, FMOD_CHANNEL_FREE, fmod_sound[i], 1, &sound_channel[i]);
	}
	
	listenerpos.x = 0.0f;
	listenerpos.y = 0.0f;
	listenerpos.z = 0;
    FMOD_System_Set3DListenerAttributes(fmod_system, 0, &listenerpos, &vel, &forward, &up);
	
	oldreverb[0]=rt_plain;
	oldreverb[1]=rt_room;
	set_player(0);
	
}


bool set_reverb(ReverbType reverbtype)
{
	if (reverbtype==oldreverb[player])
		return false;
	oldreverb[player]=reverbtype;
	
	switch (reverbtype)
	{
		case rt_off:
		{
            FMOD_REVERB_PROPERTIES reverb= FMOD_PRESET_OFF  ;
            FMOD_System_SetReverbProperties(fmod_system, &reverb);
		}
			break;
		case rt_plain:
		{
            FMOD_REVERB_PROPERTIES reverb= FMOD_PRESET_PLAIN   ;
            FMOD_System_SetReverbProperties(fmod_system, &reverb);
		}
			break;
		case rt_cave:
		{
            FMOD_REVERB_PROPERTIES reverb= FMOD_PRESET_CAVE   ;
            FMOD_System_SetReverbProperties(fmod_system, &reverb);
		}
			break;
		case rt_room:
		{
            FMOD_REVERB_PROPERTIES reverb= FMOD_PRESET_ROOM  ;
            FMOD_System_SetReverbProperties(fmod_system, &reverb);
		}
			break;
		case rt_forest:
		{
            FMOD_REVERB_PROPERTIES reverb= FMOD_PRESET_FOREST  ;
            FMOD_System_SetReverbProperties(fmod_system, &reverb);
		}
			break;
		case rt_hallway:
		{
            FMOD_REVERB_PROPERTIES reverb= FMOD_PRESET_HALLWAY  ;
            FMOD_System_SetReverbProperties(fmod_system, &reverb);
		}
			break;
		case rt_stonecorridor:
		{
            FMOD_REVERB_PROPERTIES reverb= FMOD_PRESET_STONECORRIDOR   ;
            FMOD_System_SetReverbProperties(fmod_system, &reverb);
		}
			break;
		case rt_stoneroom:
		{
            FMOD_REVERB_PROPERTIES reverb=  FMOD_PRESET_STONEROOM  ;
            FMOD_System_SetReverbProperties(fmod_system, &reverb);
		}
			break;
		case rt_psycho:
		{
			FMOD_REVERB_PROPERTIES reverb=  FMOD_PRESET_PSYCHOTIC ;
			FMOD_System_SetReverbProperties(fmod_system, &reverb);
			
		}
		default:
			break;
	}
	
	return true;
}

void activate_sound(int i)
{
	FMOD_Channel_SetPaused(sound_channel[i], 0);
}


void deactivate_sound(int i)
{
	//     FMOD_Channel_SetPaused(sound_channel[i], 1);
	FMOD_Channel_Stop(sound_channel[i]);
	
}

void place_sound(int i, float x, float y)
{
	FMOD_VECTOR pos = { x,y,0};
	FMOD_VECTOR vel = {  0.0f, 0.0f, 0.0f };
	
	FMOD_System_PlaySound(fmod_system, FMOD_CHANNEL_REUSE, fmod_sound[i], 1, &sound_channel[i]);
	FMOD_Channel_Set3DAttributes(sound_channel[i], &pos, &vel);
}


void set_playerpos(float x, float y, float dirx, float diry)
{
    
    

	listenerpos.x=x;
	listenerpos.y=y;
	forward.x=-dirx;
	forward.y=-diry;
	
	t+=0.1;
	FMOD_System_Set3DListenerAttributes(fmod_system, 0, &listenerpos, &vel, &forward, &up);
	
	FMOD_System_Update(fmod_system);
	
	int ix=int(x);
	int iy=int(y);
	
	
	//OUTSIDE
	if ( (ix==11 && iy==33) || (ix==11 && iy==33)||(ix==21 && iy==6) || (ix==9 && iy==65) )
	{
		set_reverb(rt_plain);
	}
	//HALLWAY
	else if ( (ix==11 && iy==32) || (ix==14 && iy==24)  )
	{
		set_reverb(rt_hallway);
	}
	//ENTER BEDROOM
	else if ( (ix==19 && iy==23) || (ix==19&&iy==21)||(ix==23 && iy==19))
	{
		if (set_reverb(rt_room))
		{
			place_sound(1, 22.5, 18.5);
			activate_sound(1);
		}
	}
	//ENTER LIBRARY
	else if ((ix==14&&iy==23)|| (ix==13 && iy==18))
	{
		set_reverb(rt_room);
	}
	//EXIT BEDROOM TO HALLWAY
	else if ( (ix==19 && iy==24))
	{
		if (set_reverb(rt_hallway))
		{
			deactivate_sound(1);			
		}
		
	}
	//STONE HALLWAY
	else if (  (ix==15 && iy==15)|| (ix==12 && iy==12)|| (ix==13 && iy==7)|| (ix==21 && iy==7) || (ix==27 && iy==13)  || (ix==27 && iy==30)||(ix==26 && iy==41)||(ix==13 && iy==17) )
	{
		set_reverb(rt_stonecorridor);
	}
	//STONE ROOM
	else if ( (ix==16 && iy==15) || (ix==11 && iy==12)|| (ix==13 && iy==6)|| (ix==6 && iy==9) || (ix==6 && iy==10) || (ix==6 && iy==11)   )
	{
		set_reverb(rt_stoneroom);
	}
	//LAKE
	else if ( (ix==28 && iy==30)  )
	{
		set_reverb(rt_cave);
	}
	else if ( (ix==5 && iy==10 ) || (ix==28 && iy==13))
	{
		set_reverb(rt_psycho);
	}
	else if (  (ix==26 && iy==42))
	{
		set_reverb(rt_off);
	}
	
}

void play_effect(int i)
{
	FMOD_System_PlaySound(fmod_system, FMOD_CHANNEL_FREE, fmod_effect[i], 0, &effect_channel[i]);
}

void stop_effect(int i)
{
	FMOD_Channel_Stop(effect_channel[i]);
}

void
release_sound()
{
    for (int i=0;i<SOUNDCOUNT;i++)
        FMOD_Sound_Release(fmod_sound[i]);
	FMOD_System_Close(fmod_system);
	FMOD_System_Release(fmod_system);
}



void sound_walk(bool walking)
{
	FMOD_Channel_SetPaused(effect_walkchannel,!walking);
}

void set_player(int p)
{
	if (player!=p)
	{
		//trigger old reverb
		if (p==0)
			set_reverb(oldreverb[p]);
		else
			set_reverb(rt_room);
	}
	player=p;
	FMOD_Channel_Stop(effect_walkchannel);
	FMOD_System_PlaySound(fmod_system, FMOD_CHANNEL_FREE, fmod_walk[p], true, &effect_walkchannel);
	FMOD_Channel_SetVolume(effect_walkchannel, 0.2);
}
