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
	Config.read("settings.cfg");

	if(Config.getBoolean("server"))
		GameType+=GAME_SERVER;
	if(Config.getBoolean("client"))
		GameType+=GAME_CLIENT;

	if((GameType&GAME_SERVER)==GAME_SERVER)
		ServerFrame.Init();

	if((GameType&GAME_CLIENT)==GAME_CLIENT)
		ClientFrame.Init();


}

void Merc::Run()
{
	if((GameType&GAME_SERVER)==GAME_SERVER)
		ServerFrame.AdvanceFrame();

	if((GameType&GAME_CLIENT)==GAME_CLIENT){
		ClientFrame.Connect();
		ClientFrame.Authentificate();
	}

	while(1){
		if((GameType&GAME_SERVER)==GAME_SERVER)
			ServerFrame.AdvanceFrame();
		if((GameType&GAME_CLIENT)==GAME_CLIENT)
			ClientFrame.AdvanceFrame();

		if(ServerFrame.ShutDownPending())
			break;
		if(ClientFrame.ShutDownPending())
			break;
	}

}

int Merc::GetServices()
{

	return 0;
}