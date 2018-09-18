#include "Merc.h"
#include "ConfigFile.h"

ConfigFile Config;

Merc Game;

int main(int argc,char *argv[])
{
	Game.Init();
	Game.Run();
	
	return 0;
}

void Merc::Init()
{
	Config.read("Configs/settings.cfg");

	GameType=0;

	if(Config.getBoolean("server"))
		GameType|=GAME_SERVER;
	if(Config.getBoolean("client"))
		GameType|=GAME_CLIENT;


	if(GameType==(GAME_SERVER|GAME_CLIENT))
		ServerFrame.Init(GameType);
	else if(GameType==GAME_SERVER)
		ServerFrame.Init(GameType);
	else if(GameType==GAME_CLIENT)
		ClientFrame.Init();
}

void Merc::Run()
{
	if((GameType&GAME_SERVER)==GAME_SERVER)
		ServerFrame.AdvanceFrame();
	else if((GameType&GAME_CLIENT)==GAME_CLIENT)
	{
		ClientFrame.Connect();
		ClientFrame.Authentificate();
	}
	else//Should not be here
		return;
	
	while(1){
		if((GameType&GAME_SERVER)==GAME_SERVER)
			ServerFrame.AdvanceFrame();
		else if((GameType&GAME_CLIENT)==GAME_CLIENT)
			ClientFrame.AdvanceFrame();
		else
			break;

		if((GameType&GAME_SERVER)==GAME_SERVER)
		{
			if(ServerFrame.ShutDownPending())
				break;
		}
		else if((GameType&GAME_CLIENT)==GAME_CLIENT)
		{
			if(ClientFrame.ShutDownPending())
				break;
		}
		else
			break;
	}

}
/*
int Merc::GetServices()
{

	return 0;
}*/