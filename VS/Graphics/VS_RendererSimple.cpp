/*
 *  VS_RendererSimple.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 17/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "VS_RendererSimple.h"

#include "VS_Camera.h"
#include "VS_Debug.h"
#include "VS_DisplayList.h"
#include "VS_Screen.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "SYS_Timer.h"

void
vsRendererSimple::Init(int width, int height, int depth, bool fullscreen)
{
	const SDL_VideoInfo *videoInfo = SDL_GetVideoInfo();
	int videoFlags;
	
	videoFlags = SDL_OPENGL;
	videoFlags |= SDL_GL_DOUBLEBUFFER;
	videoFlags |= SDL_HWPALETTE;
	
	if ( videoInfo->hw_available )
		videoFlags |= SDL_HWSURFACE;
	else
		videoFlags |= SDL_SWSURFACE;
	
	if ( videoInfo->blit_hw )
		videoFlags |= SDL_HWACCEL;
	
	if ( fullscreen )
		videoFlags |= SDL_FULLSCREEN;

	
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 1 );
	SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
	
	m_screen = SDL_SetVideoMode( width, height, videoInfo->vfmt->BitsPerPixel, videoFlags );
	
	if ( !m_screen ){
		fprintf(stderr, "Couldn't set %dx%dx%d video mode: %s\n",
				width, height, depth, SDL_GetError() );
		return;
	}
	
	SDL_WM_SetCaption("VectorStorm Engine",NULL);
	
	glShadeModel( GL_SMOOTH );
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClearDepth( 1.0f );  // arbitrary large value
	
	//glEnable( GL_TEXTURE_2D );		// no textures in vector graphics!
	//glEnable( GL_DEPTH_TEST );		// no depth in vector graphics!
	glShadeModel( GL_SMOOTH );
	
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);			// Set The Blending Function For Translucency
	glEnable(GL_BLEND);											// Enable Blending

	// no depth in vector graphics, so no need to provide depth tests!
	/*glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );*/
	
	glViewport( 0, 0, (GLsizei)width, (GLsizei)height );
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
}

void
vsRendererSimple::Deinit()
{
}

void
vsRendererSimple::SetCameraTransform( const vsTransform &t )
{
	vsScreen *s = vsScreen::Instance();
	float hei = t.m_scale.x;
	float wid = s->GetAspectRatio() * hei;
	float hw = wid * 0.5f;
	float hh = hei * 0.5f;
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( -hw, hw, hh, -hh, -1000, 1000 );
	glRotatef( -t.m_angle.GetDegrees(), 0.0f, 0.0f, 1.0f );
	glTranslatef( -t.m_position.x, -t.m_position.y, 0.0f );
	glMatrixMode( GL_MODELVIEW );
}

void
vsRendererSimple::PreRender()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
}

void
vsRendererSimple::PostRender()
{
	sysTimer::Instance()->EndRenderTime();
	glFinish();
	sysTimer::Instance()->EndGPUTime();
	SDL_GL_SwapBuffers();
}

void		
vsRendererSimple::RenderDisplayList( vsDisplayList *list )
{
	vsTransform defCamera;
	defCamera.m_position = vsVector2D::Zero;
	defCamera.m_angle = vsAngle::Zero;
	defCamera.m_scale = vsVector2D(1000.0f,1000.0f);
	SetCameraTransform(defCamera);
	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glColor3f (1.0f, 1.0f, 1.0f);
	
	RawRenderDisplayList(list);
}

void
vsRendererSimple::RawRenderDisplayList( vsDisplayList *list )
{
	
	vsDisplayList::op *op = list->PopOp();
	vsVector2D	cursorPos;
	vsColor		cursorColor;
	vsColor		currentColor(-1,-1,-1,0);
	vsColor		nextColor;
	bool		cursorSet = false;
	bool		inLineStrip = false;
	bool		inPointList = false;
	
	while(op)
	{
		if ( op->type != vsDisplayList::OpCode_SetColor )
		{
			if ( inLineStrip && op->type != vsDisplayList::OpCode_LineTo && op->type != vsDisplayList::OpCode_SetColor )
			{
				inLineStrip = false;
				glEnd();
			}
			if ( inPointList && op->type != vsDisplayList::OpCode_DrawPoint && op->type != vsDisplayList::OpCode_SetColor )
			{
				inPointList = false;
				glEnd();
			}
		}
		
		switch( op->type )
		{
			case vsDisplayList::OpCode_SetColor:
			{
				nextColor = op->data.GetColor();
				break;
			}
			case vsDisplayList::OpCode_MoveTo:
			{
				cursorPos = op->data.GetVector2D();
				cursorColor = nextColor;
				cursorSet = true;
				break;
			}
			case vsDisplayList::OpCode_LineTo:
			{
				vsAssert(cursorSet, "LineTo display command before a MoveTo was set!");	// error;  we haven't been given a 'MoveTo' instruction before our first 'LineTo'!
				
				if ( !inLineStrip )
				{
					glBegin( GL_LINE_STRIP );
					
					if ( cursorColor != currentColor )
					{
						glColor4f( cursorColor.r, cursorColor.g, cursorColor.b, cursorColor.a );
						currentColor = cursorColor;
					}
					glVertex2f( cursorPos.x, cursorPos.y );
					inLineStrip = true;
				}

				if ( nextColor != currentColor )
				{
					glColor4f( nextColor.r, nextColor.g, nextColor.b, nextColor.a );
					currentColor = nextColor;
				}
				
				vsVector2D pos = op->data.GetVector2D();
				glVertex2f( pos.x, pos.y );
				
				cursorPos = pos;
				break;
			}
			case vsDisplayList::OpCode_DrawPoint:
			{
				if ( !inPointList )
				{
					glBegin( GL_POINTS );
					inPointList = true;
				}
				vsVector2D pos = op->data.GetVector2D();
				glVertex2f( pos.x, pos.y );
				break;
			}
			case vsDisplayList::OpCode_PushTransform:
			{
				vsTransform t = op->data.GetTransform();
				
				glPushMatrix();
				glTranslatef( t.m_position.x, t.m_position.y, 0.0f );
				glRotatef( t.m_angle.GetDegrees(), 0.0f, 0.0f, 1.0f );
				glScalef( t.m_scale.x, t.m_scale.y, 1.0f );
				break;
			}
			case vsDisplayList::OpCode_PopTransform:
			{
				glPopMatrix();
				break;
			}
			case vsDisplayList::OpCode_SetCameraTransform:
			{
				vsTransform t = op->data.GetTransform();
				
				SetCameraTransform(t);
				break;
			}
			default:
				vsAssert(false, "Unknown opcode type in display list!");	// error;  unknown opcode type in the display list!
		}
		op = list->PopOp();
	}
	
	if ( inLineStrip )
	{
		inLineStrip = false;
		glEnd();
	}
	if ( inPointList )
	{
		inPointList = false;
		glEnd();
	}
}

void
vsRendererSimple::CheckGLError(const vsString &call)
{
    char enums[][20] =
    {
        "invalid enumeration", // GL_INVALID_ENUM
        "invalid value",       // GL_INVALID_VALUE
        "invalid operation",   // GL_INVALID_OPERATION
        "stack overflow",      // GL_STACK_OVERFLOW
        "stack underflow",     // GL_STACK_UNDERFLOW
        "out of memory"        // GL_OUT_OF_MEMORY
    };
	
    GLenum errcode = glGetError();
    if (errcode == GL_NO_ERROR)
        return;
	
    errcode -= GL_INVALID_ENUM;
    vsAssert(false,vsFormatString("OpenGL %s in '%s'", enums[errcode], call.c_str()));
}
