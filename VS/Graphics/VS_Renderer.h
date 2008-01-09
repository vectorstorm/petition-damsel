/*
 *  VS_Renderer.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 17/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef VS_RENDERER_H
#define VS_RENDERER_H

class vsDisplayList;
class vsCamera;

class vsRenderer
{
public:
	vsRenderer() {}
	virtual ~vsRenderer() {Deinit();}
	
	virtual void		Init(int width, int height, int depth, bool fullscreen) {UNUSED(width); UNUSED(height); UNUSED(depth); UNUSED(fullscreen);}
	virtual void		Deinit() {}
	
	virtual void		PreRender() {}
	virtual void		RenderDisplayList( vsDisplayList *list ) = 0;
	virtual void		RawRenderDisplayList( vsDisplayList *list ) = 0;
	virtual void		PostRender() {}
};

#endif // VS_RENDERER_H
