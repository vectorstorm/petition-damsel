/*
 *  VS_System.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 27/12/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "VS_System.h"

#include "MEM_Heap.h"

#include "VS_Preferences.h"
#include "VS_Random.h"
#include "VS_Screen.h"

#include <SDL/SDL.h>


bool				vsSystem::s_initted			= false;

extern memHeap gGlobalHeap;	// there exists this global heap;  we need to use this when changing video modes etc.



#define VS_VERSION ("0.0.1")


static void initAttributes ()
{
    // Setup attributes we want for the OpenGL context
    
    int value;
	
    // Don't set color bit sizes (SDL_GL_RED_SIZE, etc)
	//   TODO:  Investigate this.  SDL_OpenGL Seems to pick reasonable default values?
    
    // 2D Vector graphics don't require a depth buffer, so don't bother allocating one.
	//   If we actually were doing something 3D, we'd want the below lines during our
	//   initialisation.
#if 0
    value = 16;
    SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, value);
#endif    
    
    // Request double-buffered OpenGL
    value = 1;
    SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, value);
}

static void printAttributes ()
{
    // Print out attributes of the context we created
    int nAttr;
    int i;
    
    SDL_GLattr  attr[] = { SDL_GL_RED_SIZE, SDL_GL_BLUE_SIZE, SDL_GL_GREEN_SIZE,
	SDL_GL_ALPHA_SIZE, SDL_GL_BUFFER_SIZE, SDL_GL_DEPTH_SIZE };
	
    char *desc[] = { "Red size: %d bits", "Blue size: %d bits", "Green size: %d bits",
		"Alpha size: %d bits", "Color buffer size: %d bits", 
	"Depth bufer size: %d bits" };
	
    nAttr = sizeof(attr) / sizeof(int);
    
    for (i = 0; i < nAttr; i++) 
	{
        int value;
        SDL_GL_GetAttribute (attr[i], &value);
        vsLog(vsFormatString(desc[i], value));
    } 
}



vsSystem::vsSystem()
{
	// Perform some basic initialisation
	vsRandom::Init();
	vsLog("VectorStorm engine version %s\n",VS_VERSION);
	
	vsLog("Loading preferences...\n");
	m_preferences = new vsSystemPreferences;
	
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ){
		fprintf(stderr, "Couldn't initialise SDL: %s\n", SDL_GetError() );
		exit(1);
	}
	atexit(SDL_Quit);
	
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	SDL_InitSubSystem(SDL_INIT_AUDIO);
	
}

vsSystem::~vsSystem()
{
	SDL_Quit();
}

void
vsSystem::Init()
{
	m_preferences->CheckResolutions();
	Resolution *res = m_preferences->GetResolution();

	vsLog("Initialising [%dx%d] resolution...\n", res->width, res->height);

	SDL_ShowCursor( !m_preferences->GetFullscreen() );
	
	vsScreen::Startup( res->width, res->height, 32, m_preferences->GetFullscreen() );

	// Set GL context attributes
	initAttributes ();
	
	// Get GL context attributes
	printAttributes ();
	
	SDL_WM_SetCaption("Game a Week 1 -- Damsel",NULL);
}

void
vsSystem::Deinit()
{
	m_preferences->Save();

	vsScreen::Shutdown();
}

void
vsSystem::UpdateVideoMode()
{
	// this function is called when we change video mode while the game is running, post-initialisation, so we need to switch
	// back to our system heap.  (So that potentially changing video mode doesn't get charged to the currently active game,
	// and then treated as a memory leak)
	
	memHeap::Push(&gGlobalHeap);
	
	Resolution *res = m_preferences->GetResolution();
	
	vsLog("Initialising [%dx%d] resolution...\n", res->width, res->height);
	
	SDL_ShowCursor( !m_preferences->GetFullscreen() );
	
	vsScreen::Instance()->UpdateVideoMode( res->width, res->height, 32, m_preferences->GetFullscreen() );
	
	// Set GL context attributes
	initAttributes ();
	
	// Get GL context attributes
	printAttributes ();
	
	SDL_WM_SetCaption("Game a Week 1 -- Damsel",NULL);

	memHeap::Pop(&gGlobalHeap);

}

vsSystemPreferences::vsSystemPreferences()
{
	m_preferences = new vsPreferences("global");
	
	m_resolution = NULL;	// can't get this one until we can actually check what SDL supports, later on.
	m_fullscreen = m_preferences->GetPreference("Fullscreen", 0, 0, 1);
	m_bloom = m_preferences->GetPreference("Bloom", 1, 0, 1);
	m_effectVolume = m_preferences->GetPreference("EffectVolume", 8, 0, 10);
	m_musicVolume = m_preferences->GetPreference("MusicVolume", 7, 0, 10);
}

vsSystemPreferences::~vsSystemPreferences()
{
	delete m_preferences;
}

void
vsSystemPreferences::CheckResolutions()
{
	vsLog("Checking supported resolutions...\n");
	
	SDL_Rect **modes;
	int i;
	/* Get available fullscreen/hardware modes */
	modes=SDL_ListModes(NULL, SDL_FULLSCREEN|SDL_HWSURFACE);
	
	/* Check if there are any modes available */
	if(modes == (SDL_Rect **)0){
		vsLog("No modes available!\n");
		exit(-1);
	}
	
	/* Check if our resolution is restricted */
	if(modes == (SDL_Rect **)-1){
		vsLog("All resolutions available.\n");
	}
	else
	{
		m_supportedResolutionCount = 0;
		/* Print valid modes */
		vsLog("Available Modes\n");
		for(i=0;modes[i];++i)
		{
			m_supportedResolutionCount++;
			vsLog("%d:  %d x %d", i, modes[i]->w, modes[i]->h);
		}
		
		m_supportedResolution = new Resolution[m_supportedResolutionCount];
		
		for(i=0;modes[i];++i)
		{
			m_supportedResolution[i].width = modes[i]->w;
			m_supportedResolution[i].height = modes[i]->h;
		}
	}
	m_resolution = m_preferences->GetPreference("Resolution", i-1, 0, i-1);
}

Resolution *
vsSystemPreferences::GetResolution()
{
	int resolutionId = GetResolutionId();
	return &m_supportedResolution[resolutionId];
}

int
vsSystemPreferences::GetResolutionId()
{
	return m_resolution->m_value;
}

void
vsSystemPreferences::SetResolutionId(int id)
{
	m_resolution->m_value = id;
}

bool
vsSystemPreferences::GetBloom()
{
	return !!(m_bloom->m_value);
}

void
vsSystemPreferences::SetBloom(bool enabled)
{
	m_bloom->m_value = enabled;
}

void
vsSystemPreferences::Save()
{
	m_preferences->Save();
}

void
vsSystemPreferences::SetFullscreen(bool fullscreen)
{
	m_fullscreen->m_value = fullscreen;
}

bool
vsSystemPreferences::GetFullscreen()
{
	return !!m_fullscreen->m_value;	
}

void
vsSystemPreferences::SetEffectVolume(int volume)
{
	m_effectVolume->m_value = volume;
}

int
vsSystemPreferences::GetEffectVolume()
{
	return m_effectVolume->m_value;	
}

void
vsSystemPreferences::SetMusicVolume(int volume)
{
	m_musicVolume->m_value = volume;
}

int
vsSystemPreferences::GetMusicVolume()
{
	return m_musicVolume->m_value;	
}

