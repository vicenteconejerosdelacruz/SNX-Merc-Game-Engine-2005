#include <math.h>
#include <SDL/SDL.h>
#include "Display.h"
#include "ConfigFile.h"
#include "Log.h"
#include "Frustum.h"
#include "DynamicObject.h"

#ifdef WIN32

typedef void (APIENTRY * PFNGLACTIVETEXTUREARBPROC) (GLenum);
typedef void (APIENTRY * PFNGLCLIENTACTIVETEXTUREARBPROC) (GLenum);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2FPROC) (GLenum,GLfloat,GLfloat);

PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB		 = NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB = NULL;
PFNGLMULTITEXCOORD2FPROC		glMultiTexCoord2fARB = NULL;

#else

#include<sys/types.h>
#include<unistd.h>

#endif

//extern float start[3],end[3],ColitionPos[3];

extern ConfigFile Config;

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
Uint32 getpixel(SDL_Surface *surface, int x, int y);

Display::Display()
{
	Olist=NULL;
	RenderMode=FILL;
	Accum=0;
	FPSAccum=0;
	NumObjects=0;
}

void Display::Init()
{
	if(SDL_InitSubSystem(SDL_INIT_VIDEO) == -1){
		Log::Output("Could not init SDL\n");
		return;
	}

	Width=Config.getInteger("width");
	Height=Config.getInteger("height");
	FOV=(float)atof(Config.getString("FOV"));

	Screen=SDL_SetVideoMode(Width,Height,0,SDL_OPENGL);
	SDL_WM_SetCaption("MERC's",NULL);

	glEnable(GL_TEXTURE_2D);
	glShadeModel( GL_SMOOTH );
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClearDepth( 1.0f );
	glEnable( GL_DEPTH_TEST );
	glEnable(GL_LIGHTING);
	glDepthFunc( GL_LEQUAL );
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	glViewport(0, 0, Width, Height);
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	gluPerspective( FOV, Width/Height, 0.1f, 100000.0f );
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

	float Light[]={15.0f,15.0f,15.0f,1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,Light);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	TTF_Init();
	Last=SDL_GetTicks();
	font = TTF_OpenFont("Arial.ttf", 24);

}

void Display::AddObject(RenderAble *Obj)
{

	RenderObject *ROptr;

	ROptr=Olist;
	if(ROptr==NULL)
	{
		Olist=new RenderObject;
		Olist->next=NULL;
		Olist->prev=NULL;
		ROptr=Olist;
	}
	else
	{
		while(ROptr->next!=NULL)
			ROptr=ROptr->next;

		RenderObject *NewRenderObj;
		NewRenderObj=new RenderObject;

		ROptr->next=NewRenderObj;
		ROptr->next->next=NULL;
		ROptr->next->prev=ROptr;
		ROptr=ROptr->next;
	}

	ROptr->Object=(RenderAble *)Obj;
	ROptr->State=true;
	NumObjects++;
}

void Display::Render()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//SDL_WM_SetCaption(buffer,NULL);

	gluLookAt(LookTo.x(),LookTo.y(),LookTo.z(),Position.x(),Position.y(),Position.z(),0,1,0);
/*
	glColor3ub(255,0,0);
	glBegin(GL_LINES);
		glVertex3fv(start);
		glVertex3fv(end);
	glEnd();
	glColor3ub(255,255,255);*/
/*
	Vector3 luakto=LookTo;
	luakto*=10;
	glColor3ub(255,0,0);
	glBegin(GL_LINES);
	glVertex3fv(Position.GetVector());
	glVertex3fv(luakto.GetVector());
	glEnd();
*/
	//Frustum::CalculateFrustum();
/*
	glPushMatrix();
		GLUquadricObj *pObj = gluNewQuadric();
		glTranslatef(ColitionPos[0],ColitionPos[1],ColitionPos[2]);
		gluQuadricDrawStyle(pObj, GLU_LINE);
		gluSphere(pObj,10,30,30);
		gluDeleteQuadric(pObj);
	glPopMatrix();
*/
	RenderObject *ROptr;
	ROptr=Olist;

	if(RenderMode==FILL)
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	else if(RenderMode==LINE)
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	
	while(ROptr!=NULL)
	{
		ROptr->Object->Render();
		ROptr=ROptr->next;
	}


	Now=SDL_GetTicks();
	Accum+=Now-Last;
	FPSAccum++;
	Last=Now;
	
	char buffer[255];
	memset(buffer,0,255);
	sprintf(buffer,"FPS: %d",FPSCounter);
	//SDL_WM_SetCaption(buffer,NULL);
	//ScreenPrint("Arial.ttf",buffer,24,255,0,0,0,535);


	SDL_GL_SwapBuffers();

	glColor3ub(255,255,255);
	if(Accum>1000)
	{
		FPSCounter=FPSAccum;
		Accum=0;
		FPSAccum=0;
	}
}

void Display::PlaceCamera(Player *player)
{
	Position=player->Position;
	LookTo.x((float)cos(player->Rotation[0])*cos(-player->Rotation[1]));
	LookTo.y((float)sin(player->Rotation[0]));
	LookTo.z((float)cos(player->Rotation[0])*sin(-player->Rotation[1]));
	LookTo+=Position;
}

void Display::ScreenPrint(char *fontname,char *text,int size,int R,int G,int B,int x,int y)
{
	SDL_Color color;
	color.r = R;
	color.g = G;
	color.b = B;

	first = TTF_RenderText_Blended(font, text, color);
	int w,h;

//	w=(int)(pow(2,ceil(logl(first->w) / logl(2)))+0.5);
//	h=(int)(pow(2,ceil(logl(first->h) / logl(2)))+0.5);

	second = SDL_CreateRGBSurface(0, w, h, 32, 
			0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

	SDL_BlitSurface(first, NULL, second, NULL);

	IMG_RGBA=new unsigned char[w*h*4];

	for(int i=0;i<h*w;i++)
	{
		Uint32 pixel;

		int xpos;
		int ypos;

		xpos=i%w;
		ypos=i/w;

		pixel=getpixel(second,xpos,ypos);
		Uint8 r,g,b;
		SDL_GetRGB(pixel,second->format,&r,&g,&b);

		IMG_RGBA[i*4]=r;
		IMG_RGBA[i*4+1]=g;
		IMG_RGBA[i*4+2]=b;
		if((IMG_RGBA[i*4]==0)&&(IMG_RGBA[i*4+1]==0)&&(IMG_RGBA[i*4+2]==0))
			IMG_RGBA[i*4+3]=0;
		else
			IMG_RGBA[i*4+3]=255;
		
	}

	unsigned int texture;
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1,&texture);

	glBindTexture(GL_TEXTURE_2D,texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,IMG_RGBA);

	int vPort[4];
	glGetIntegerv(GL_VIEWPORT, vPort);
  
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
  
	glOrtho(0, vPort[2], 0, vPort[3], -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	//glColor3ub(255,255,255);
	glBegin(GL_QUADS);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0.0f, 1.0f);glVertex2d(x    , y);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1.0f, 1.0f);glVertex2d(x + w, y);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1.0f, 0.0f);glVertex2d(x + w, y + h);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0.0f, 0.0f);glVertex2d(x    , y + h);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable( GL_BLEND );
	glEnable(GL_DEPTH_TEST);
	
	glPopMatrix();  
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();	
	glMatrixMode(GL_MODELVIEW);
	
	glDeleteTextures(1,&texture);
	SDL_FreeSurface(first);
	SDL_FreeSurface(second);
	delete IMG_RGBA;
}

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
 
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0; 
    }
}

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}
