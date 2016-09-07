#include <SDL/SDL.h>
#include "InputHandler.h"
#include "ConfigFile.h"
#include "Log.h"
#include "Action.h"
#include "Error.h"

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
	
	SDL_PollEvent(&event);

	input.GameState=0;
	input.KeyState=A_STANDING;
	input.Rotation[0]=0;
	input.Rotation[1]=0;

	keys=SDL_GetKeyState(NULL);
	KeyDefs *KDptr=Keylist;

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

	return input;
}

void InputHandler::ReadConfig()
{
	ConfigFile KeyBinds("KeyBinds.cfg");

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
}