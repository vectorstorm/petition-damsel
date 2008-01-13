/*
 *  VS_Screen.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 12/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "VS_Screen.h"
#include "VS_DisplayList.h"
#include "VS_Layer.h"
#include "VS_RendererSimple.h"
#include "VS_RendererPretty.h"
#include "VS_RendererBloom.h"
#include "VS_System.h"

#include "SYS_Timer.h"

const int c_fifoSize = 1024 * 400;		// 200k for our FIFO display list

vsScreen::vsScreen(int width, int height, int depth, bool fullscreen):
	m_width(width),
	m_height(height),
	m_depth(depth),
	m_layer(NULL),
	m_layerCount(0)
{
	vsRendererSimple *bootstrap = new vsRendererSimple;
	bootstrap->Init(width, height, depth, fullscreen);
	delete bootstrap;

	if ( vsRendererBloom::Supported() && vsSystem::Instance()->GetPreferences()->GetBloom() )
		m_renderer = new vsRendererBloom();
	else
		m_renderer = new vsRendererPretty();

	m_renderer->Init(width, height, depth, fullscreen);
		
	m_aspectRatio = ((float)m_width)/((float)m_height);
	printf("Screen Ratio:  %f\n", m_aspectRatio);
	
	m_fifo = new vsDisplayList(c_fifoSize);
}

vsScreen::~vsScreen()
{
	m_renderer->Deinit();
	
	delete m_fifo;
	delete m_renderer;
}

void
vsScreen::Startup(int width, int height, int depth, bool fullscreen)
{
	new vsScreen(width, height, depth, fullscreen);
}

void
vsScreen::Shutdown()
{
	delete vsScreen::Instance();
}

void
vsScreen::UpdateVideoMode(int width, int height, int depth, bool fullscreen)
{
	m_width = width;
	m_height = height;
	m_depth = depth;
	
	m_renderer->Deinit();
	delete m_renderer;
	if ( vsRendererBloom::Supported() && vsSystem::Instance()->GetPreferences()->GetBloom() )
		m_renderer = new vsRendererBloom();
	else
		m_renderer = new vsRendererPretty();
	m_renderer->Init(width, height, depth, fullscreen);
	
	m_aspectRatio = ((float)m_width)/((float)m_height);
	printf("Screen Ratio:  %f\n", m_aspectRatio);
	
}

void
vsScreen::CreateLayers(int count)
{
	DestroyLayers();

#if defined(_DEBUG)
	count++;	// extra layer for our debug data
#endif // _DEBUG
	
	m_layer = new vsLayer *[count];
	for ( int i = 0; i < count; i++ )
		m_layer[i] = new vsLayer;
	m_layerCount = count;


#if defined(_DEBUG)
	m_layer[m_layerCount-1]->SetDebugCamera();
#endif // _DEBUG
}

void
vsScreen::DestroyLayers()
{
	if ( m_layer )
	{
		for ( int i = 0; i < m_layerCount; i++ )
			delete m_layer[i];
		delete [] m_layer;
		
		m_layer = NULL;
	}
}

void
vsScreen::Update( float timeStep )
{
	for ( int i = 0; i < m_layerCount; i++ )
	{
		m_layer[i]->Update( timeStep );
	}
}

static long s_fifoHighWaterMark = c_fifoSize / 2;	// don't start warning us about how much display list we're using until we're at least half full
//static long s_fifoHighWaterMark = 0;	// don't start warning us about how much display list we're using until we're at least half full

void
vsScreen::Draw()
{
	m_renderer->PreRender();
	
	for ( int i = 0; i < m_layerCount; i++ )
	{
		m_fifo->Clear();
		m_layer[i]->Draw( m_fifo );
		m_renderer->RenderDisplayList(m_fifo);
		
		if ( m_fifo->GetSize() > s_fifoHighWaterMark )
		{
			printf("New FIFO high water mark:  Layer %d, %0.2fk/%0.2fkk\n", i, m_fifo->GetSize()/1024.f, m_fifo->GetMaxSize()/1024.f);
			s_fifoHighWaterMark = m_fifo->GetSize();
		}
	}
	
	m_renderer->PostRender();

}

vsLayer *
vsScreen::GetLayer(int i)
{
	vsAssert( i < m_layerCount, "Requested non-allocated vsLayer!" );
	
#if defined(_DEBUG )
	vsAssert( i < m_layerCount-1, "Requested debug layer, which won't be there in release builds!" );
#endif // _DEBUG
	
	return m_layer[i];
}

void
vsScreen::RenderDisplayList( vsDisplayList *list )
{
	m_renderer->RawRenderDisplayList(list);
}

#if defined(_DEBUG)

vsLayer *
vsScreen::GetDebugLayer()
{
	return m_layer[ m_layerCount-1 ];
}

#endif // _DEBUG
