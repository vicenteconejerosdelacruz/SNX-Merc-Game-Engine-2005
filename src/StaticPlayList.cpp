#include "StaticPlayList.h"
#include <string.h>

StaticPlayList::StaticPlayList()
{
	list = new OggAudio[MAX_SONGS];
	cant = 0;
}

StaticPlayList::~StaticPlayList()
{
	for (int i=0; i<cant; i++){
		list[i].unload();
	}
	cant = 0;
}

void StaticPlayList::add(OggAudio *a)
{	
	list[cant]= *a;
	cant++;		
}

OggAudio *StaticPlayList::search(char *key)
{
	for (int i=0; i<cant; i++)
		if (strcmp(list[i].getKeyName(),key) == 0)
			return &list[i];
	return 0;
}

void StaticPlayList::setALLpos(Vector3 vec)
{
	for (int i=0; i<cant; i++)
		list[i].setPosXYZ(vec);
}