/*
 *  VS_RendererSimple.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 17/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef VS_RENDERER_SIMPLE_H
#define VS_RENDERER_SIMPLE_H

#include "VS_Renderer.h"

class vsDisplayList;
class vsTransform;
struct SDL_Surface;

class vsRendererSimple : public vsRenderer
{
	SDL_Surface *		m_screen;		// our front buffer

	void			SetCameraTransform( const vsTransform &t );

public:

	virtual void	Init(int width, int height, int depth, bool fullscreen);
	virtual void	Deinit();
	
	virtual void	PreRender();
	virtual void	RenderDisplayList( vsDisplayList *list );
	virtual void	RawRenderDisplayList( vsDisplayList *list );
	virtual void	PostRender();
	
	void			CheckGLError(const vsString &call);
};

#endif // VS_RENDERER_SIMPLE_H
