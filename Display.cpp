#include <SDL/SDL.h>
#include "Display.h"
#include "ConfigFile.h"
#include "Log.h"

#ifdef WIN32

PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB		 = NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB = NULL;
PFNGLMULTITEXCOORD2FPROC		glMultiTexCoord2fARB = NULL;

#else

#include<sys/types.h>
#include<unistd.h>

#endif

extern ConfigFile Config;

Display::Display()
{

}

void Display::Init()
{
	if(SDL_InitSubSystem(SDL_INIT_VIDEO) == -1){
		Log::Output("Could not init SDL\n");
		return;
	}

	Width=Config.getInteger("width");
	Height=Config.getInteger("height");

	Screen=SDL_SetVideoMode(Width,Height,0,SDL_OPENGL);
	SDL_WM_SetCaption("MERC's",NULL);

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glShadeModel( GL_SMOOTH );
	glEnable( GL_DEPTH_TEST );
	glEnable(GL_LIGHTING);

	glClearDepth( 1.0f );
	glDepthFunc( GL_LEQUAL );
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	glViewport(0, 0, Width, Height);
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	gluPerspective( 90.0f, Width/Height, 0.1f, 10000.0f );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );

	#ifdef WIN32
	glActiveTextureARB		= 
	(PFNGLACTIVETEXTUREARBPROC)	wglGetProcAddress("glActiveTextureARB");
	glClientActiveTextureARB= 
	(PFNGLACTIVETEXTUREARBPROC)	wglGetProcAddress("glClientActiveTextureARB");
	glMultiTexCoord2fARB	= 
	(PFNGLMULTITEXCOORD2FPROC)	wglGetProcAddress("glMultiTexCoord2fARB");	
	#endif


}

void Display::AddObject(RenderAble *Obj)
{


}

void Display::Render()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	SDL_GL_SwapBuffers();
}