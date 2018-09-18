#ifndef MERC_INPUTHANDLER_H
#define MERC_INPUTHANDLER_H

#include <SDL/SDL.h>

typedef struct Input
{
	int KeyState;
	int GameState;
	float Rotation[2];
}Input;

typedef struct KeyDefs
{
	struct KeyDefs *next;
	Uint8 Key;
	int Action;
	int GameAction;
}KeyDefs;

class InputHandler
{
private:
	KeyDefs *Keylist;
	int MSensibility;
public:
	InputHandler();
	Input CheckInput();
	void Init();
	void ReadConfig();
};

#endif