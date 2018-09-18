#ifndef MERC_DISPLAY_H
#define MERC_DISPLAY_H

#include <windows.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <GL/GL.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "RenderObject.h"
#include "Player.h"
#include "Vector3.h"

enum MODES{FILL=1,LINE};

class Display
{
private:
	RenderObject *Olist;
	int NumObjects;
	SDL_Surface *Screen;
	int Width;
	int Height;
	Vector3 Position;
	Vector3 LookTo;
	float FOV;
	int RenderMode;
	int FPSCounter;
	int FPSAccum;
	Uint32 Now,Last,Accum;

	unsigned char *IMG_RGBA;
	SDL_Surface *first,*second;
	TTF_Font *font;
public:
	Display();
	void ScreenPrint(char *fontname,char *text,int size,int R,int G,int B,int x,int y);
	void AddObject(RenderAble *Obj);
	void Init();
	void PlaceCamera(Player *player);
	void Render();
	void DeleteObject(int id);
};

#endif