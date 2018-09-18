#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <windows.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Cache.h"
#include "Log.h"
#include "Model.h"

File Cache::Files[MAX_FILES];
int Cache::NumFiles=0;

Cache::Cache()
{
	Init();
}

void Cache::Init()
{
	memset(Files,0,sizeof(Files));
}

void Cache::LoadFile(char *FileName,int Type,...)
{
	va_list va;
	va_start(va,Type);
	int i;

	Model *model;

	//Log::Output("Loading %s\n",FileName);

	for(i=0;i<NumFiles;i++)
		if((strcmp(Files[i].FileName,FileName)==0) && (Files[i].type==Type)){
			switch(Files[i].type){
				case TEXTURE_FILE:
					unsigned int *texture;
					texture=va_arg(va,unsigned int *);
					*texture=(unsigned int)Files[i].Data;
					return;
				break;
				case MODEL_FILE:
					model=va_arg(va,Model *);
					memcpy(model,Files[i].Data,sizeof(model));
					//*model=(Model *)&Files[i].Data;
					return;
				break;
				default:
				break;
			}
		}

	for(i=0;i<MAX_FILES;i++)
		if(Files[i].type==0)
			break;

	switch(Type)
	{
	case TEXTURE_FILE:

		unsigned int *texture;

		texture=va_arg(va,unsigned int *);
		SDL_Surface *IMG;
		
		IMG=IMG_Load(FileName);
	
		if(IMG==NULL)
		{
			Log::Output("Texture %s no found\n",FileName);
			return;
		}
		else
		{
			glEnable(GL_TEXTURE_2D);
			glGenTextures(1,texture);

			Files[i].Data=new unsigned int[1];
			memset(Files[i].Data,0,sizeof(unsigned int));
			Files[i].Data=(void *)*texture;
			
			glBindTexture(GL_TEXTURE_2D,*texture);
			glPixelStorei(GL_UNPACK_ALIGNMENT,1);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			gluBuild2DMipmaps(GL_TEXTURE_2D,IMG->format->BytesPerPixel,IMG->w,IMG->h,GL_RGB,GL_UNSIGNED_BYTE,IMG->pixels);
			
			glDisable(GL_TEXTURE_2D);
		}
	break;
	case MODEL_FILE:
		model=va_arg(va,Model *);

		model->Load(FileName);
		Files[i].Data=new Model[1];
		memcpy(Files[i].Data,model,sizeof(model));
	break;
	default:
	break;
	}
	Files[i].type=Type;
	strcat(Files[i].FileName,FileName);
	NumFiles++;
}