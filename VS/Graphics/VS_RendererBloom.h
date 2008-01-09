/*
 *  VS_RendererBloom.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 4/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef VS_RENDERERBLOOM_H
#define VS_RENDERERBLOOM_H

#include "VS_RendererSimple.h"

#define NO_SDL_GLEXT
#include <SDL_opengl.h>

struct vsViewport
{
    GLint x;
    GLint y;
    GLsizei width;
    GLsizei height;
};

struct vsBloomSurface
{
    GLsizei width;
    GLsizei height;
	vsViewport	viewport;
	GLfloat	texWidth;	// 0..1  viewport.width/width
	GLfloat	texHeight;	// 0..1  
    GLfloat modelview[16];
    GLfloat projection[16];
    GLuint texture;
    GLuint depth;
    GLuint fbo;
};

#define FILTER_COUNT (4)

class vsRendererBloom : public vsRendererSimple
{
	typedef vsRendererSimple Parent;
	
	static GLuint			s_combineProg;
	static GLuint			s_filterProg;
	static bool				s_shadersBuilt;
	//GLuint			m_hiPassProg;
	
	vsBloomSurface	m_window;
	vsBloomSurface	m_scene;
	vsBloomSurface	m_pass[FILTER_COUNT];
	vsBloomSurface	m_pass2[FILTER_COUNT];
	
	typedef enum {HORIZONTAL, VERTICAL} Direction;
	

	void			Blur(vsBloomSurface *sources, vsBloomSurface *dests, int count, Direction dir);
	void			CreateSurface(vsBloomSurface *, bool depth, bool fp, bool linear);
	void			DeleteSurface(vsBloomSurface *surface);
	GLuint			Compile(const char *vert, const char *frag);
	void			BindSurface(vsBloomSurface *surface);
	void			ClearSurface();
	
public:
	vsRendererBloom();
	virtual			~vsRendererBloom();
	
	virtual void	Init(int width, int height, int depth, bool fullscreen);
	virtual void	Deinit();
	
	virtual void	PreRender();
	virtual void	RenderDisplayList( vsDisplayList *list );
	virtual void	PostRender();
	
	void			CheckFBO();

	static bool		Supported();
	
};


#endif // VS_RENDERERBLOOM_H
