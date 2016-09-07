#include <string.h>
#include "ServerPlayer.h"

int ServerPlayer::GetMessages(char *Data)
{
	if(offset==0)
		return 0;

	int size=offset;
	memcpy(Data,Messages,offset);
	offset=0;

	return size;
}