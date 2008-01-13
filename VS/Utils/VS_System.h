/*
 *  VS_System.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 27/12/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef VS_SYSTEM_H
#define VS_SYSTEM_H

#include "MIX_Singleton.h"

class vsPreferences;
class vsPreferenceObject;
class vsSystemPreferences;

class vsSystem : public mixSingleton<vsSystem>
{
	static bool s_initted;

	vsSystemPreferences *m_preferences;
	
public:
	
	vsSystem();
	~vsSystem();
	
	void Init();
	void Deinit();
	
	void UpdateVideoMode();
	
	vsSystemPreferences *	GetPreferences() { return m_preferences; }
};


struct Resolution
{
	int width;
	int height;
};


	/** vsSystemPreferences
	 *		Accessor class to provide managed access to the global preferences data.
	 */
class vsSystemPreferences
{
	vsPreferences *	m_preferences;
	
	vsPreferenceObject *	m_resolution;
	vsPreferenceObject *	m_resolutionX;
	vsPreferenceObject *	m_resolutionY;
	vsPreferenceObject *	m_fullscreen;
	vsPreferenceObject *	m_bloom;

	vsPreferenceObject *	m_effectVolume;
	vsPreferenceObject *	m_musicVolume;

	Resolution*	m_supportedResolution;
	int			m_supportedResolutionCount;
	
	
public:
	
	vsSystemPreferences();
	~vsSystemPreferences();
	
	void			Save();
	
	void			CheckResolutions();
	int				GetSupportedResolutionCount() { return m_supportedResolutionCount; }
	Resolution *	GetSupportedResolutions() { return m_supportedResolution; }
	
	// Video preferences
	Resolution *	GetResolution();
	int				GetResolutionId();
	void			SetResolutionId(int val);
	
	bool			GetFullscreen();
	void			SetFullscreen(bool fullscreen);
	
	bool			GetBloom();
	void			SetBloom(bool enabled);
	
	// Sound preferences
	int				GetEffectVolume();
	void			SetEffectVolume(int volume);
	
	int				GetMusicVolume();
	void			SetMusicVolume(int volume);
};

#endif // VS_SYSTEM_H

