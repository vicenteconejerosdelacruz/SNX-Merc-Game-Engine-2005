#ifndef MERC_MAIN_H
#define MERC_MAIN_H

#include "Server.h"
#include "Client.h"

/*Game types*/
#define GAME_SERVER				0x01
#define GAME_CLIENT				0x02

/*Services Flags*/
#define INIT_DISPLAY	0x01
#define INIT_SOUND		0x02
#define INIT_INPUT		0x04
#define INIT_NETWORK	0x08
#define INIT_PHYSICS	0x10
#define INIT_TIMER		0x20
#define INIT_DB			0x40

class Merc
{
private:
	Server ServerFrame;
	Client ClientFrame;
	int GetServices();
	int GameType;

public:
	void Init();
	void Run();
};

#endif