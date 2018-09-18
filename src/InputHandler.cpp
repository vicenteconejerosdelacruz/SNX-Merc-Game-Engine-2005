#include <SDL/SDL.h>
#include "InputHandler.h"
#include "ConfigFile.h"
#include "Log.h"
#include "Action.h"
#include "MercError.h"

bool grab=false;//test variable

InputHandler::InputHandler()
{
	Keylist=NULL;

}

void InputHandler::Init()
{
	SDL_EnableUNICODE(1);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	Keylist=new KeyDefs;

	ReadConfig();	
}

Input InputHandler::CheckInput()
{
	Input input;
	SDL_Event event;
	Uint8 *keys;
	Uint8 MouseState;
	SDL_Surface *PointerSurface;
	int x,y;
	float RotX,RotY;
	
	SDL_PollEvent(&event);


	input.GameState=0;
	input.KeyState=A_STANDING;
	input.Rotation[0]=0;
	input.Rotation[1]=0;

	keys=SDL_GetKeyState(NULL);
	KeyDefs *KDptr=Keylist;

	//Test Shit
	if(keys[SDLK_b])
		grab=(grab==true)?false:true;

	if(grab==true)
		return input;
	//End Test Shit

	if(KDptr==NULL)
		Throw("Fatal error Keylist=NULL");

	while(KDptr!=NULL)
	{
		if(keys[KDptr->Key])
		{
			input.KeyState|=KDptr->Action;
			input.GameState|=KDptr->GameAction;
		}

		KDptr=KDptr->next;
	}

	PointerSurface=SDL_GetVideoSurface();
	MouseState=SDL_GetMouseState(&x,&y);
	SDL_WarpMouse((Uint16)PointerSurface->w/2,(Uint16)PointerSurface->h/2);	
	RotY=(float)((PointerSurface->w/2)-x)/MSensibility;
	RotX=(float)((PointerSurface->h/2)-y)/MSensibility;

	if(RotX > 1.0f)
		RotX=1.0f;
	else if(RotX < -1.0f)
        RotX=-1.0f;

	if(RotX!=0.0f || RotY!=0.0f){
		input.Rotation[0]=RotX;
		//input.Rotation[0]=0;

		input.Rotation[1]=RotY;
	}

	return input;
}

void InputHandler::ReadConfig()
{
	ConfigFile KeyBinds("Configs/KeyBinds.cfg");

	int NumKeys;
	NumKeys=KeyBinds.getInteger("num_keys");
	
	KeyDefs *KDptr=Keylist;

	for(int i=0;i<NumKeys;i++)
	{
		KDptr->Key=KeyBinds.getInteger("key%d.key",i);
		KDptr->Action=KeyBinds.getInteger("key%d.action",i);
		KDptr->GameAction=KeyBinds.getInteger("key%d.gameaction",i);
		KeyDefs *NewKey;

		NewKey=new KeyDefs;
		KDptr->next=NewKey;
		KDptr->next->next=NULL;
		KDptr=KDptr->next;
	}

	MSensibility=KeyBinds.getInteger("mouse_sensibility");
}