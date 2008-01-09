/*
 *  VS_RendererBloom.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 4/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#include <gl/glew.h>
#include "VS_RendererBloom.h"


vsBloomSurface *g_boundSurface = NULL;

#define STRINGIFY(A)  #A
/*
const char *blitf = STRINGIFY(
							  uniform sampler2D source;
							  uniform vec4 bkgd;
							  
							  void main(void)
							  {
							  vec4 t = texture2D(source, gl_TexCoord[0].st);
							  gl_FragColor = t + bkgd;
							  }
);*/

const char *passv = STRINGIFY(
							  void main(void)
							  {
							  gl_TexCoord[0] = gl_MultiTexCoord0;
							  gl_Position    = ftransform();
							  }
);

const char *combine5f = STRINGIFY(
								  uniform sampler2D Pass0;
								  uniform sampler2D Pass1;
								  uniform sampler2D Pass2;
								  uniform sampler2D Pass3;
								  uniform sampler2D Scene;
								  
								  void main(void)
								  {
								  vec4 t0 = texture2D(Pass0, gl_TexCoord[0].st);
								  vec4 t1 = texture2D(Pass1, gl_TexCoord[0].st);
								  vec4 t2 = texture2D(Pass2, gl_TexCoord[0].st);
								  vec4 t3 = texture2D(Pass3, gl_TexCoord[0].st);
								  vec4 t4 = texture2D(Scene, gl_TexCoord[0].st);
								  gl_FragColor = t0 + t1 + t2 + t3 + t4;
								  }
);

const char *row3f = STRINGIFY(
							  uniform sampler2D source;
							  uniform float coefficients[3];
							  uniform float offsetx;
							  uniform float offsety;
							  
							  void main(void)
							  {
							  vec4 c;
							  vec2 tc = gl_TexCoord[0].st;
							  vec2 offset = vec2(offsetx, offsety);
							  
							  c = coefficients[0] * texture2D(source, tc - offset);
							  c += coefficients[1] * texture2D(source, tc);
							  c += coefficients[2] * texture2D(source, tc + offset);
							  
							  gl_FragColor = c;
							  }
);
/*
const char *row5f = STRINGIFY(
							  uniform sampler2D source;
							  uniform float coefficients[5];
							  uniform float offsetx;
							  uniform float offsety;
							  
							  void main(void)
							  {
							  vec4 c;
							  vec2 tc = gl_TexCoord[0].st;
							  vec2 offset = vec2(offsetx, offsety);
							  
							  c = coefficients[0] * texture2D(source, tc - (offset*2.0));
							  c += coefficients[1] * texture2D(source, tc - offset);
							  c += coefficients[2] * texture2D(source, tc);
							  c += coefficients[3] * texture2D(source, tc + offset);
							  c += coefficients[4] * texture2D(source, tc + (offset*2.0));
							  
							  gl_FragColor = c;
							  }
);

const char *row7f = STRINGIFY(
							  uniform sampler2D source;
							  uniform float coefficients[7];
							  uniform float offsetx;
							  uniform float offsety;
							  
							  void main(void)
							  {
							  vec4 c;
							  vec2 tc = gl_TexCoord[0].st;
							  vec2 offset = vec2(offsetx, offsety);
							  
							  c  = coefficients[0] * texture2D(source, tc - (offset*3.0));
							  c += coefficients[1] * texture2D(source, tc - (offset*2.0));
							  c += coefficients[2] * texture2D(source, tc - offset);
							  c += coefficients[3] * texture2D(source, tc);
							  c += coefficients[4] * texture2D(source, tc + offset);
							  c += coefficients[5] * texture2D(source, tc + (offset*2.0));
							  c += coefficients[6] * texture2D(source, tc + (offset*3.0));
							  
							  gl_FragColor = c;
							  }
);

const char *hipassf = STRINGIFY(
								uniform sampler2D source;
								
								void main(void)
								{
								vec4 color = texture2D(source, gl_TexCoord[0].st);
								//float intensity = (color.x+color.y+color.z) * 0.3333;
								//color.x *= color.x > 0.8 ? color.x : 0.0;
								//color.y *= color.y > 0.8 ? color.y : 0.0;
								//color.z *= color.z > 0.8 ? color.z : 0.0;
								//color.x *= intensity;// * color.x;
								//color.y *= intensity;// * color.y;
								//color.z *= intensity;// * color.z;
								color.xyz *= 0.5;//intensity;
								
								gl_FragColor = color;
								}
);
*/
#define KERNEL_SIZE   3

GLfloat black[4] = {0,0,0,0};
float kernel[KERNEL_SIZE] = { 4, 8, 4  };

GLuint vsRendererBloom::s_combineProg;
GLuint vsRendererBloom::s_filterProg;
bool vsRendererBloom::s_shadersBuilt = false;

//#include <SDL/SDL_opengl.h>





vsRendererBloom::vsRendererBloom()
{
}

#define BUFFER_HEIGHT (256)
#define BUFFER_WIDTH  (256)

void
vsRendererBloom::Init(int width, int height, int depth, bool fullscreen)
{
	Parent::Init(width, height, depth, fullscreen);

	//if ( !s_shadersBuilt )
	{
		// Compile shaders
		s_combineProg = Compile(passv, combine5f);
		s_filterProg = Compile(passv, row3f);
		s_shadersBuilt = true;
	}
	
	// Normalize kernel coefficients
    float sum = 0;
    for (int c = 0; c < KERNEL_SIZE; c++)
        sum += kernel[c];
    for (int c = 0; c < KERNEL_SIZE; c++)
        kernel[c] /= sum;
	
    glMatrixMode(GL_MODELVIEW);
	
	// Create Window Surface
	m_window.fbo = 0;
	m_window.depth = 0;
	m_window.width = width;
	m_window.height = height;
    m_window.viewport.x = 0;
    m_window.viewport.y = 0;
    m_window.viewport.width = width;
    m_window.viewport.height = height;
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, m_window.modelview);
    glOrtho(0, m_window.width, 0, m_window.height, 0, 10);
    glGetFloatv(GL_MODELVIEW_MATRIX, m_window.projection);
    glLoadIdentity();
	
    // Create 3D Scene Surface
	// we want to be big enough to hold our full m_window resolution, and set our viewport to match the window.
	
	int actualWidth = 1024;
	int actualHeight = 512;
	
	if ( width > 1024 )
		actualWidth = 2048;
	if ( height > 512 )
		actualHeight = 1024;
	
    m_scene.width = actualWidth;
    m_scene.height = actualHeight;
	m_scene.viewport.x = 0;
    m_scene.viewport.y = 0;
    m_scene.viewport.width = width;
    m_scene.viewport.height = height;
	m_scene.texWidth = (float)m_scene.viewport.width / (float)m_scene.width;
	m_scene.texHeight = (float)m_scene.viewport.height / (float)m_scene.height;
    glGetFloatv(GL_MODELVIEW_MATRIX, m_scene.modelview);
    glOrtho(0, m_scene.width, 0, m_scene.height, 0, 10);
    glGetFloatv(GL_MODELVIEW_MATRIX, m_scene.projection);
    glLoadIdentity();
//    glGetFloatv(GL_MODELVIEW_MATRIX, m_scene.projection);
    CreateSurface(&m_scene, GL_FALSE, GL_FALSE, GL_TRUE);
	
	width = BUFFER_WIDTH;
	height = BUFFER_HEIGHT;
	// Create Source Surfaces
	for (int p = 0; p < FILTER_COUNT; p++)
	{
		m_pass[p].width = width;
		m_pass[p].height = height;
		m_pass[p].viewport.x = 0;
		m_pass[p].viewport.y = 0;
		m_pass[p].viewport.width = width;
		m_pass[p].viewport.height = height;
        glGetFloatv(GL_MODELVIEW_MATRIX, m_pass[p].modelview);
        glGetFloatv(GL_MODELVIEW_MATRIX, m_pass[p].projection);
		CreateSurface(&m_pass[p], GL_FALSE, GL_FALSE, GL_TRUE);
		width = width >> 1;
		height = height >> 1;
	}
	
    width = BUFFER_WIDTH;
    height = BUFFER_HEIGHT;
    for (int p = 0; p < FILTER_COUNT; p++)
    {
        m_pass2[p].width = width;
        m_pass2[p].height = height;
        m_pass2[p].viewport.x = 0;
        m_pass2[p].viewport.y = 0;
        m_pass2[p].viewport.width = width;
        m_pass2[p].viewport.height = height;
        glGetFloatv(GL_MODELVIEW_MATRIX, m_pass2[p].modelview);
        glGetFloatv(GL_MODELVIEW_MATRIX, m_pass2[p].projection);
        CreateSurface(&m_pass2[p], GL_FALSE, GL_FALSE, GL_TRUE);
        width = width >> 1;
        height = height >> 1;
    }
}

void
vsRendererBloom::Deinit()
{
	DeleteSurface(&m_scene);
	for (int p = 0; p < FILTER_COUNT; p++)
	{
		DeleteSurface(&m_pass[p]);
		DeleteSurface(&m_pass2[p]);
	}
}

void
vsRendererBloom::CreateSurface(vsBloomSurface *surface, bool depth, bool fp, bool linear)
{
	vsAssert(linear, "Asked for a non-linear surface!");
    GLenum internalFormat = fp ? GL_RGBA16F_ARB : GL_RGBA;
    GLenum type = fp ? GL_FLOAT : GL_UNSIGNED_BYTE;
    GLenum filter = linear ? GL_LINEAR : GL_NEAREST;
	
    // create a color texture
    glGenTextures(1, &surface->texture);
    glBindTexture(GL_TEXTURE_2D, surface->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, surface->width, surface->height, 0, GL_RGBA, type, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
    glBindTexture(GL_TEXTURE_2D, 0);
	
	CheckGLError("Creation of the color texture for the FBO");
	
    // create depth renderbuffer
    if (depth)
    {
        glGenRenderbuffersEXT(1, &surface->depth);
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, surface->depth);
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, surface->width, surface->height);
        CheckGLError("Creation of the depth renderbuffer for the FBO");
    }
    else
    {
        surface->depth = 0;
    }
	
    // create FBO itself
    glGenFramebuffersEXT(1, &surface->fbo);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, surface->fbo);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, surface->texture, 0);
    if (depth)
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, surface->depth);
    CheckFBO();
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    CheckGLError("Creation of the FBO itself");
}

void
vsRendererBloom::DeleteSurface(vsBloomSurface *surface)
{
	glDeleteTextures(1, &surface->texture);
	glDeleteFramebuffersEXT(1, &surface->fbo);
}

void
vsRendererBloom::CheckFBO()
{
    char enums[][20] =
    {
        "attachment",         // GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT........... All framebuffer attachment points are 'framebuffer attachment complete'.
        "missing attachment", // GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT....There is at least one image attached to the framebuffer.
        "",                   //
        "dimensions",         // GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT............All attached images have the same width and height.
        "formats",            // GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT...............All images attached to the attachment points COLOR_ATTACHMENT0_EXT through COLOR_ATTACHMENTn_EXT must have the same internal format.
        "draw buffer",        // GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT...........The value of FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT must not be NONE for any color attachment point(s) named by DRAW_BUFFERi.
        "read buffer",        // GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT...........If READ_BUFFER is not NONE, then the value of FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT must not be NONE for the color attachment point named by READ_BUFFER.
        "unsupported format"  // GL_FRAMEBUFFER_UNSUPPORTED_EXT......................The combination of internal formats of the attached images does not violate an implementation-dependent set of restrictions.
    };
	
    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    if (status == GL_FRAMEBUFFER_COMPLETE_EXT)
        return;
	
    status -= GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT;
    vsAssert(false,vsFormatString("incomplete framebuffer object due to %s", enums[status]));
}

GLuint 
vsRendererBloom::Compile(const char *vert, const char *frag)
{
    GLchar buf[256];
    GLuint vertShader, fragShader, program;
    GLint success;
	
    vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, (const GLchar**) &vert, 0);
    glCompileShader(vertShader);
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertShader, sizeof(buf), 0, buf);
        //debugf(buf);
        vsAssert(success,"Unable to compile vertex shader.\n");
    }
	
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, (const GLchar**) &frag, 0);
    glCompileShader(fragShader);
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragShader, sizeof(buf), 0, buf);
        //debugf(buf);
        vsAssert(success,"Unable to compile fragment shader.\n");
    }
	
    program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
	
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, sizeof(buf), 0, buf);
        //debugf(buf);
        vsAssert(success,"Unable to link shaders.\n");
    }
	
    return program;
}


vsRendererBloom::~vsRendererBloom()
{
}

void
vsRendererBloom::Blur(vsBloomSurface *sources, vsBloomSurface *dests, int count, Direction dir)
{
    GLint loc;
    int p;
	
    // Set up the filter.
    glUseProgram(s_filterProg);
    loc = glGetUniformLocation(s_filterProg, "source");
    glUniform1i(loc, 0);
    loc = glGetUniformLocation(s_filterProg, "coefficients");
    glUniform1fv(loc, KERNEL_SIZE, kernel);
    loc = glGetUniformLocation(s_filterProg, "offsetx");
    glUniform1f(loc, 0);
    loc = glGetUniformLocation(s_filterProg, "offsety");
    glUniform1f(loc, 0);
    if (dir == HORIZONTAL)
        loc = glGetUniformLocation(s_filterProg, "offsetx");
	
    // Perform the blurring.
    for (p = 0; p < count; p++)
    {
        float offset = 1.2f / sources[p].width;
        glUniform1f(loc, offset);
        BindSurface(dests + p);
        glBindTexture(GL_TEXTURE_2D, sources[p].texture);
        glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex2i(-1, -1);
        glTexCoord2i(1, 0); glVertex2i(1, -1);
        glTexCoord2i(1, 1); glVertex2i(1, 1);
        glTexCoord2i(0, 1); glVertex2i(-1, 1);
        glEnd();
    }
}


void
vsRendererBloom::PreRender()
{
    //int p;
    //GLint loc;
	
    // Draw 3D scene.
    BindSurface(&m_scene);
	glBlendFunc( GL_ONE, GL_ZERO );
	ClearSurface();
	Parent::PreRender();
}


/*	BindSurface(&m_window);
 ClearSurface();
 glActiveTexture(GL_TEXTURE0);
 glBindTexture(GL_TEXTURE_2D, m_pass[0].texture);
 glEnable(GL_TEXTURE_2D);
 glBlendFunc( GL_SRC_COLOR, GL_ONE );
 
 glColor4f(1,1,1,1);
 glBegin(GL_QUADS);
 glTexCoord2i(0, 1); glVertex2i(0.0f, 0);
 glTexCoord2i(1, 1); glVertex2i(m_window.width, 0);
 glTexCoord2i(1, 0); glVertex2i(m_window.width, m_window.height);
 glTexCoord2i(0, 0); glVertex2i(0.0f, m_window.height);
 //    glTexCoord2i(0, 0); glVertex2i(-1, -1);
 //    glTexCoord2i(1, 0); glVertex2i(1, -1);
 //    glTexCoord2i(1, 1); glVertex2i(1, 1);
 //    glTexCoord2i(0, 1); glVertex2i(-1, 1 );
 glEnd();
 
 glDisable(GL_TEXTURE_2D);
 
 Parent::PostRender();
 return;*/


void
vsRendererBloom::PostRender()
{
	int p;
	int loc;
	
	glBlendFunc( GL_ONE, GL_ZERO );
	
	vsTuneable float glowBrightness = 0.7f;
	// don't hi-pass;  just scale down colors.
	glColor4f(glowBrightness,glowBrightness,glowBrightness,1.f);
	
	glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_scene.texture);
    BindSurface(m_pass);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmapEXT(GL_TEXTURE_2D);
	
	//	ClearSurface();
    glBegin(GL_QUADS);
    glTexCoord2i(0, 0); glVertex2i(-1, -1);
    glTexCoord2i(1, 0); glVertex2i(1, -1);
    glTexCoord2i(1, 1); glVertex2i(1, 1);
    glTexCoord2i(0, 1); glVertex2i(-1, 1);
    glEnd();
	
	glColor4f(1.f,1.f,1.f,1.f);
	/*
    // Hi-pass filter into pass0[0]
    glUseProgram(m_hiPassProg);
    loc = glGetUniformLocation(m_hiPassProg, "source");
    glUniform1i(loc, 0);
	glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_scene.texture);
    BindSurface(m_pass);
	ClearSurface();
    glBegin(GL_QUADS);
    glTexCoord2i(0, 0); glVertex2i(-1, -1);
    glTexCoord2i(1, 0); glVertex2i(1, -1);
    glTexCoord2i(1, 1); glVertex2i(1, 1);
    glTexCoord2i(0, 1); glVertex2i(-1, 1);
    glEnd();
    glUseProgram(0);
	*/
    // Downsample the scene into the source surfaces.
    glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, m_pass[0].texture);
    for (p = 1; p < FILTER_COUNT; p++)
    {
        BindSurface(&m_pass[p]);
        glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex2i(-1, -1);
        glTexCoord2i(1, 0); glVertex2i(1, -1);
        glTexCoord2i(1, 1); glVertex2i(1, 1);
        glTexCoord2i(0, 1); glVertex2i(-1, 1);
        glEnd();
    }
	
	// Perform the horizontal blurring pass.
    Blur(m_pass, m_pass2, FILTER_COUNT, HORIZONTAL);
    // Perform the vertical blurring pass.
    Blur(m_pass2, m_pass, FILTER_COUNT, VERTICAL);
	
	BindSurface(&m_window);
	//ClearSurface();

    glUseProgram(s_combineProg);
	
    for (p = 0; p < FILTER_COUNT; p++)
    {
        char name[] = "Pass#";
		
        glActiveTexture(GL_TEXTURE0 + p);
        glBindTexture(GL_TEXTURE_2D, m_pass[p].texture);
        glEnable(GL_TEXTURE_2D);
		
        sprintf(name, "Pass%d", p);
        loc = glGetUniformLocation(s_combineProg, name);
        glUniform1i(loc, p);
    }
	
    glActiveTexture(GL_TEXTURE0 + FILTER_COUNT);
    glBindTexture(GL_TEXTURE_2D, m_scene.texture);
    glEnable(GL_TEXTURE_2D);
    loc = glGetUniformLocation(s_combineProg, "Scene");
    glUniform1i(loc, FILTER_COUNT);
	
    glBegin(GL_QUADS);
    glTexCoord2i(0, 0); glVertex2i(0, 0);
    glTexCoord2f(m_scene.texWidth, 0); glVertex2i(m_window.width, 0);
    glTexCoord2f(m_scene.texWidth, m_scene.texHeight); glVertex2i(m_window.width, m_window.height);
    glTexCoord2f(0, m_scene.texHeight); glVertex2i(0, m_window.height);
    glEnd();
	
    glUseProgram(0);
	
    for (p = 0; p < FILTER_COUNT; p++)
    {
        glActiveTexture(GL_TEXTURE0 + p);
        glDisable(GL_TEXTURE_2D);
    }
    glActiveTexture(GL_TEXTURE0 + FILTER_COUNT);
    glDisable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
	
#if defined(_DEBUG)
	
	static bool checkPass = false;
	
	if ( checkPass )
	{
		static int passId = 0;
		// now let's just check contents of our stuff.
		BindSurface(&m_window);
		ClearSurface();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_pass[passId].texture);
		glEnable(GL_TEXTURE_2D);
		
		glBegin(GL_QUADS);
		glTexCoord2i(0, 0); glVertex2i(0, 0);
		glTexCoord2f(m_scene.texWidth, 0); glVertex2i(m_window.width, 0);
		glTexCoord2f(m_scene.texWidth, m_scene.texHeight); glVertex2i(m_window.width, m_window.height);
		glTexCoord2f(0, m_scene.texHeight); glVertex2i(0, m_window.height);
		glEnd();
		
		glDisable(GL_TEXTURE_2D);
	}
#endif // _DEBUG
	
	Parent::PostRender();
}

void
vsRendererBloom::RenderDisplayList( vsDisplayList *list )
{
	// give us thicker lines, nicely smoothed.
	glLineWidth( 2.0f );
	glPointSize( 2.5f );
	glEnable( GL_LINE_SMOOTH );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	
	// let our parent class actually perform the rendering, now that we've modified our GL settings.
	Parent::RenderDisplayList(list);
}

void 
vsRendererBloom::BindSurface(vsBloomSurface *surface)
{
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, surface->fbo);
    glViewport(0,0, surface->viewport.width, surface->viewport.height);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(surface->projection);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(surface->modelview);
	g_boundSurface = surface;
}



void 
vsRendererBloom::ClearSurface()
{
    const vsBloomSurface *surface = g_boundSurface;
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | (surface->depth ? GL_DEPTH_BUFFER_BIT : 0));
}


bool
vsRendererBloom::Supported()
{
	GLenum err = glewInit();
	vsAssert(GLEW_OK == err, "Error initialising glew!");

	if ( glGenRenderbuffersEXT && glBindRenderbufferEXT && glRenderbufferStorageEXT && glGenFramebuffersEXT &&
		glBindFramebufferEXT && glFramebufferTexture2DEXT && glCreateShader && glShaderSource && glCompileShader &&
		glGetShaderiv && glCreateProgram && glAttachShader && glLinkProgram )
	{
		return true;
	}

	return false;
}
