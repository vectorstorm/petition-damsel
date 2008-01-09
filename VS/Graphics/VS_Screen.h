/*
 *  VS_Screen.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 12/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef VS_SCREEN_H
#define VS_SCREEN_H

#include "MIX_Singleton.h"

class vsDisplayList;
class vsLayer;
class vsRenderer;

class vsScreen : public mixSingleton<vsScreen>
{
	vsRenderer *		m_renderer;		// our renderer
	vsLayer **			m_layer;		// our draw layers

	int					m_layerCount;	// how many layers we have
	
	vsDisplayList *		m_fifo;			// our FIFO display list, for rendering
	
	int					m_width;
	int					m_height;
	int					m_depth;
	float				m_aspectRatio;

	vsScreen(int width, int height, int depth, bool fullscreen);
	~vsScreen();

public:
	static void Startup(int width, int height, int depth, bool fullscreen);
	static void Shutdown();

	void			UpdateVideoMode(int width, int height, int depth, bool fullscreen);

	int				GetWidth() { return m_width; }
	int				GetHeight() { return m_height; }
	float			GetAspectRatio() { return m_aspectRatio; }
	
	void			RenderDisplayList( vsDisplayList *list );	// used to pipe our display list to the renderer, for creating compiled display lists.
	
	void			CreateLayers(int count);
	void			DestroyLayers();
	
	void			Update( float timeStep );
	void			Draw();
	
	vsLayer *		GetLayer(int i);

#if defined(_DEBUG)
	vsLayer *		GetDebugLayer();
#endif // _DEBUG
};

#endif // VS_SCREEN_H
