#include "AudioManager.h"
#include "Log.h"
#include "Configfile.h"
/* esta lista de reproduccion contiene objetos con timer */	
StaticPlayList AudioManager::timerPlayList;

AudioManager* AudioManager::manager;


Uint32 PlayTimer(Uint32 interval, void* param)
{	
	char *snd = (char*)param;
	SndPlayer *player = SndPlayer::getInstance();
	OggAudio *sound = NULL;
	
	sound = (OggAudio*)AudioManager::timerPlayList.search(snd);
	if (!sound){
		Log::Output("[SoundServer] No se encontro el archivo %s\n",snd);
		return interval;
	}
	player->play(sound);	
	return interval;
}

AudioManager *AudioManager::getInstance()
{
	if (manager == 0)
    {  
		manager = new AudioManager;
    }
    return manager;
}

AudioManager::AudioManager()
{
	SDL_Init(SDL_INIT_TIMER);	
}

ALboolean AudioManager::InitEAX()
{	
	Log::Output("Trying EAX 4.0...\t\t");
	g_bEAX = alIsExtensionPresent((ALubyte*)"EAX4.0");
	if (g_bEAX)
		Log::Output("[OK]\n");
	else{
		Log::Output("[FAILED]\n");
		Log::Output("Trying EAX 3.0...\t\t");
		g_bEAX = alIsExtensionPresent((ALubyte*)"EAX3.0");
		if (g_bEAX)
			Log::Output("[OK]\n");
		else{
			Log::Output("[FAILED]\n");
			Log::Output("Trying EAX 2.0...\t\t");
			g_bEAX = alIsExtensionPresent((ALubyte*)"EAX2.0");
			if (g_bEAX)
				Log::Output("[OK]\n");
			else{
				Log::Output("[FAILED]\n");
				Log::Output("Trying EAX...\t\t");
				g_bEAX = alIsExtensionPresent((ALubyte*)"EAX");
				if (g_bEAX)
					Log::Output("[OK]\n");
				else
					Log::Output("[FAILED]\n");
			}
		}
	}
	return  g_bEAX;
}



int AudioManager::InitAudio()
{
#ifdef _WIN32
	alutInit(0,NULL);
	if (alGetError()){
		Log::Output("[SoundServer] Error a iniciar openal\n");
		return 0;
	}

#else

	if ((Device = alcOpenDevice((ALubyte*)"alsa")))
		Log::Output("[SoundServer] ALSA sirviendo sonido\n");
	else
	if ((Device = alcOpenDevice((ALubyte*)"oss")))
		Log::Output("[SoundServer] OSS sirviendo sonido\n");
	else
	if ((Device = alcOpenDevice((ALubyte*)"esd")))
		Log::Output("[SoundServer] ESD sirviendo sonido\n");
	else
	if ((Device = alcOpenDevice((ALubyte*)"SDL")))
		Log::Output("[SoundServer] SDL sirviendo sonido\n");
	else
		Log::Output("[SoundServer] No se pudo encontrar un servidor de sonido.\n");
	
	Context = alcCreateContext(Device,NULL);
	if(!Context){
		alcCloseDevice(Device);
		Log::Output("[SoundServer] No se pudo iniciar el contexto OpenAL.\n");
		return 0;
	}
	alcMakeContextCurrent(Context);

#endif   	
	Log::Output(".................. SOUND MANAGER INFO DRIVE ..................\n");
	Log::Output("Vendor: %s\n",alGetString(AL_VENDOR));
	Log::Output("Render: %s\n",alGetString(AL_RENDERER));
	Log::Output("Version: %s\n",alGetString(AL_VERSION));
	Log::Output("Possible Extensions: %s\n",alGetString(AL_EXTENSIONS));
	Log::Output("..............................................................\n");

	this->InitEAX();
	//set Doppler
	alDopplerFactor( 1.0 );
	alDopplerVelocity( 343.0f );

	alGenSources(1,&source);

	return 1;
}

int AudioManager::QuitAudio()
{
	this->unloadPlayLists();
	alDeleteSources(1,&source);
#ifdef _WIN32
	alutExit();
#else
	Context=alcGetCurrentContext();
	Device=alcGetContextsDevice(Context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(Context);
	alcCloseDevice(Device);
#endif
	printf("[SoundServer] Terminado correctamente.\n");
	return 1;
}

char *AudioManager::getMapName()
{
	return mapName;
}

void AudioManager::setMapName(char *map)
{
	mapName = map;
}

void AudioManager::play(char *snd,int lst)
{
	SndPlayer *player = SndPlayer::getInstance();
	OggAudio *sound = NULL;
		//si la lista a tocar es de static
	if (lst == MERC_STATIC){		
		sound = (OggAudio*)staticList.search(snd);
		if (!sound){
			Log::Output("[SoundServer] No se encontro el archivo %s\n",snd);
			return;
		}
		player->play(sound);
	}else
		//si la lista a tocar es de dynamic
	if (lst == MERC_DYNAMIC){
		sound = (OggAudio*)dynamicList.search(snd);
		if (!sound){
			Log::Output("[SoundServer] No se encontro el archivo %s\n",snd);
			return;
		}
		player->play(sound);
	}else{	//si la lista pertenece al listener		
		escucha.PlaySound(snd);
	}
}

void AudioManager::stop(char *snd,int lst)
{
	SndPlayer *player = SndPlayer::getInstance();
	OggAudio *sound = NULL;
		//si la lista a tocar es de static
	if (lst == MERC_STATIC){
		sound = (OggAudio*)staticList.search(snd);
		if (!sound){
			Log::Output("[SoundServer] No se encontro el archivo %s\n",snd);
			return;
		}
		player->stop(sound);
	}else
		//si la lista a tocar es de dynamic
	if (lst == MERC_DYNAMIC){
		sound = (OggAudio*)dynamicList.search(snd);
		if (!sound){
			Log::Output("[SoundServer] No se encontro el archivo %s\n",snd);
			return;
		}
		player->stop(sound);
	}
}

void AudioManager::pause(char *snd,int lst)
{
	SndPlayer *player = SndPlayer::getInstance();
	OggAudio *sound = NULL;
		//si la lista a tocar es de static
	if (lst == MERC_STATIC){
		sound = (OggAudio*)staticList.search(snd);
		if (!sound){
			Log::Output("[SoundServer] No se encontro el archivo %s\n",snd);
			return;
		}
		player->pause(sound);
	}else
	//si la lista a tocar es de dynamic
	if (lst == MERC_DYNAMIC){
		sound = (OggAudio*)dynamicList.search(snd);
		if (!sound){
			Log::Output("[SoundServer] No se encontro el archivo %s\n",snd);
			return;
		}
		player->pause(sound);
	}else{	//si la lista pertenece al listener
		escucha.PauseSound(snd);
	}
}

int AudioManager::loadPlayLists()
{
	if (this->loadDynamicPlayList() && 
		this->loadStaticPlayList()  &&
		this->loadListenerPlayList())
		return 1;
	else
		return 0;
}

void AudioManager::unloadPlayLists()
{
	dynamicList.~DynamicPlayList();
	staticList.~StaticPlayList();
	escucha.unloadPlayList();
}

//privates playlist loading
int AudioManager::loadDynamicPlayList()
{
	ConfigFile cf("Configs/maps.cfg");

	int NumMaps=cf.getInteger("num_maps");
	
	for(int i=0;i<NumMaps;i++){
		char table[255];
		memset(table,0,255);
		sprintf(table,"map%d.name",i);
		if(strcmp(mapName,cf.getString(table))==0){
			memset(table,0,255);
			sprintf(table,"map%d.dynamic_sounds",i);
			int numsounds=cf.getInteger(table);
			for(int j=0;j<numsounds;j++){
					//leer el nombre
				sprintf(table,"map%d.dynamic_sound_%d_name",i,j);
				printf("2do %s\n",cf.getString(table));	
				OggAudio *ogg;
				ogg=new OggAudio(cf.getString(table));
				ogg->load();
					//leer la posicion
				sprintf(table,"map%d.dynamic_sound_%d_pos",i,j);
				float px,py,pz;
				sscanf(cf.getString(table),"%f | %f | %f",&px,&py,&pz);
				printf("%1.f, %1.f, %1.f\n",px,py,pz);
				Vector3 vec;
				vec.x(px); vec.y(py); vec.z(pz);
				ogg->setPosXYZ(vec);
					//leer los atributos
				sprintf(table,"map%d.dynamic_sound_%d_attrib",i,j);
				printf("Attrib %s\n",cf.getString(table));
				if (strcmp("loop",cf.getString(table))==0)
					ogg->setLOOP(1);
						//mas atributos ya vendran
				dynamicList.add((DynamicAudio*)ogg);
				delete ogg;
			}
		}
	}
	return 1;
}

int AudioManager::loadStaticPlayList()
{
	ConfigFile cf("Configs/maps.cfg");

	int NumMaps=cf.getInteger("num_maps");	
	for(int i=0;i<NumMaps;i++){
		char table[255];
		memset(table,0,255);
		sprintf(table,"map%d.name",i);
		if(strcmp(mapName,cf.getString(table))==0){
			memset(table,0,255);
			sprintf(table,"map%d.static_sounds",i);
			int numsounds=cf.getInteger(table);
			for(int j=0;j<numsounds;j++){
					//leer el nombre
				sprintf(table,"map%d.static_sound_%d_name",i,j);
				printf("Sound Name: %s\n",cf.getString(table));	
				OggAudio *ogg;
				ogg=new OggAudio(cf.getString(table));
				ogg->load();
					//leer su posicion
				sprintf(table,"map%d.static_sound_%d_pos",i,j);
				float px,py,pz;
				sscanf(cf.getString(table),"%f | %f | %f",&px,&py,&pz);
				printf("%1.f, %1.f, %1.f\n",px,py,pz);
				Vector3 vec;
				vec.x(px); vec.y(py); vec.z(pz);
				ogg->setPosXYZ(vec);
					//leer sus atributos
				sprintf(table,"map%d.static_sound_%d_attrib",i,j);
				printf("Attrib: %s\n",cf.getString(table));
				if (strcmp("loop",cf.getString(table))==0)
					ogg->setLOOP(1);
				else if (strcmp("timer",cf.getString(table))==0){
					sprintf(table,"map%d.static_sound_%d_secs",i,j);
					int secs;
					sscanf(cf.getString(table),"%i",&secs);
					printf("segundos: %i\n",secs);
					secs = secs*1000;
					OggAudio *ogg2 = ogg->copyAudio(ogg);					
					timerPlayList.add(ogg2);
					SDL_AddTimer(secs,PlayTimer,ogg2->getKeyName());
				}
						//mas atributos ya vendran
				staticList.add(ogg);
				delete ogg;
			}
		}
	}
	return 1;
}

int AudioManager::loadListenerPlayList()
{	
	return (escucha.loadPlayList(mapName));
}

Listener AudioManager::getListener()
{
	return escucha;
}

void AudioManager::setListenerPos(Vector3 vec)
{
	escucha.setPosXYZ(vec);
}

Vector3 AudioManager::getListenerPos()
{
	return escucha.getPosXYZ();
}

void AudioManager::updateListenerXYZ(Vector3 vec)
{
	if (vec.x() != 0) escucha.updateX(vec.x());
	if (vec.y() != 0) escucha.updateY(vec.y());
	if (vec.z() != 0) escucha.updateZ(vec.z());
	vec = escucha.getPosXYZ();
	Log::Output("(%1.f,%1.f,%1.f)\n",vec.x(),vec.y(),vec.z());
}

void AudioManager::playInXYZ(char *key,ALfloat x, ALfloat y, ALfloat z)
{
	ALfloat pos[] ={x,y,z};
	OggAudio *snd = NULL;
	snd = escucha.getSoundByName(key);
	if (!snd)
		return;
	ALuint buffer = snd->getBuffer();
	alSourcei(source,AL_BUFFER,buffer);
	alSourcefv(source,AL_POSITION,pos);
	SndPlayer *player = SndPlayer::getInstance();
	player->play(source);
}

void AudioManager::setEAXEnviroment(int env)
{
	SndPlayer *player = SndPlayer::getInstance();
		//si no hay EAX palacasa!
	if (!g_bEAX)
		return;
	ALuint Env;
	switch (env){
		case MERC_NORMAL:
			Env = MERC_NORMAL;
			player->setEAXPreset(Env);
			break;
		case MERC_ROOM:
			Env = EAX_ENVIRONMENT_ROOM;
			player->setEAXPreset(Env);
			break;
		case MERC_BATHROOM:
			Env = EAX_ENVIRONMENT_ROOM;
			player->setEAXPreset(Env);
			break;
		case MERC_AUDITORIUM:
			Env = EAX_ENVIRONMENT_AUDITORIUM;
			player->setEAXPreset(Env);
			break;
		case MERC_CONCERTHALL:
			Env = EAX_ENVIRONMENT_CONCERTHALL;
			player->setEAXPreset(Env);
			break;
		case MERC_CAVE:
			Env = EAX_ENVIRONMENT_CAVE;
			player->setEAXPreset(Env);
			break;
		case MERC_ARENA:
			Env = EAX_ENVIRONMENT_ARENA;
			player->setEAXPreset(Env);
			break;
		case MERC_HANGAR:
			Env = EAX_ENVIRONMENT_HANGAR;
			player->setEAXPreset(Env);
			break;
		case MERC_PARKINGLOT:
			Env = EAX_ENVIRONMENT_PARKINGLOT;
			player->setEAXPreset(Env);
			break;
		case MERC_SEWERPIPE:
			Env = EAX_ENVIRONMENT_SEWERPIPE;
			player->setEAXPreset(Env);
			break;
		case MERC_UNDERWATER:
			Env = EAX_ENVIRONMENT_UNDERWATER;
			player->setEAXPreset(Env);
			break;
		case MERC_PSYCHOTIC:
			Env = EAX_ENVIRONMENT_PSYCHOTIC;
			player->setEAXPreset(Env);
			break;
		default:
			Env = MERC_NORMAL;
			player->setEAXPreset(Env);
			break;
	}
}

void AudioManager::createTimers()
{
	
	SDL_AddTimer(4000,PlayTimer,"bomb.ogg");
}
