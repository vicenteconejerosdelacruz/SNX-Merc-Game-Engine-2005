#include "Listener.h"
#include "merc_configfile.h"
#include "SndPlayer.h"

Listener::Listener()
{
	listenerPos.x(0.0f);
	listenerPos.y(0.0f);
	listenerPos.z(0.0f);
	
	listenerVel.x(0.0f);
	listenerVel.y(0.0f);
	listenerVel.z(0.0f);
	
	listenerOri[0] = 0.0f;
	listenerOri[1] = 0.0f;
	listenerOri[2] = 1.0f;
	listenerOri[3] = 0.0f;
	listenerOri[4] = 1.0f;
	listenerOri[5] = 0.0f;

	alListenerfv(AL_POSITION,listenerPos.GetVector());
	alListenerfv(AL_VELOCITY,listenerVel.GetVector());
	alListenerfv(AL_ORIENTATION,listenerOri);
	alListenerf( AL_GAIN, 1.0 );
}

void Listener::setPosXYZ(Vector3 vec)
{
	listenerPos = vec;
	alListenerfv(AL_POSITION,listenerPos.GetVector());
	pls.setALLpos(listenerPos);
}

Vector3 Listener::getPosXYZ()
{
	return listenerPos;
}

ALfloat Listener::getX()
{
	return listenerPos.x();
}

ALfloat Listener::getY()
{
	return listenerPos.y();
}

ALfloat Listener::getZ()
{
	return listenerPos.z();
}

void Listener::setOrientation(Vector3 vec)
{
	listenerOri[0] = -vec.x();
	listenerOri[1] = vec.y();
	listenerOri[2] = -vec.z();
	alListenerfv(AL_ORIENTATION,listenerOri);
}

void Listener::updateX(ALfloat units)
{
    listenerPos.x(listenerPos.x() + units);
	alListenerfv(AL_POSITION,listenerPos.GetVector());
	pls.setALLpos(listenerPos);
}

void Listener::updateY(ALfloat units)
{
    listenerPos.y(listenerPos.y() + units);
	alListenerfv(AL_POSITION,listenerPos.GetVector());
	pls.setALLpos(listenerPos);
}

void Listener::updateZ(ALfloat units)
{
    listenerPos.z(listenerPos.z() + units);
	alListenerfv(AL_POSITION,listenerPos.GetVector());
	pls.setALLpos(listenerPos);
}

int Listener::loadPlayList(char * fileName)
{
	printf("................. Loading Listeners PLAYLIST .................\n");
	Merc::ConfigFile cf("maps.cfg");

	int NumMaps=cf.getInteger("num_maps");
	
	for(int i=0;i<NumMaps;i++){
		char table[255];		
		memset(table,0,255);
		sprintf(table,"map%d.nombre",i);
		if(strcmp(fileName,cf.getString(table))==0){
			memset(table,0,255);
			sprintf(table,"map%d.listener_sounds",i);
			int numsounds=cf.getInteger(table);
			for(int j=0;j<numsounds;j++){
				sprintf(table,"map%d.listener_sound_%d_name",i,j);
				printf("Sound Name: %s\n",cf.getString(table));
				OggAudio *ogg;
				ogg=new OggAudio(cf.getString(table));
				ogg->load();

				sprintf(table,"map%d.listener_sound_%d_attrib",i,j);
				printf("Attrib: %s\n",cf.getString(table));
				if (strcmp("loop",cf.getString(table))==0)
					ogg->setLOOP(1);
					//despues vendran mas atributos
				pls.add(ogg);	
				delete ogg;
			}
		}
	}
	return 1;
}

void Listener::unloadPlayList()
{
	pls.~StaticPlayList();
}

void Listener::PlaySound(char *sound)
{
	OggAudio *ogg = pls.search(sound);
	if (!ogg)
		return;
	SndPlayer *player = SndPlayer::getInstance();
	player->play(ogg);
}

void Listener::PauseSound(char *sound)
{
	OggAudio *ogg = pls.search(sound);
	if (!ogg)
		return;

	SndPlayer *player = SndPlayer::getInstance();
	player->pause(ogg);
}

OggAudio* Listener::getSoundByName(char *sound)
{
	OggAudio *ogg = pls.search(sound);
	if (!ogg)
		return 0;
	return ogg;
}