#include "OggAudio.h"


size_t VorbisRead(void *ptr, size_t byteSize,size_t sizeToRead,void *datasource){
	
	OggAudio *OggPtr;
	
	OggPtr=(OggAudio*)datasource;
	size_t ActSRead;

	if((sizeToRead*byteSize)<(OggPtr->GetDataSize() - OggPtr->GetDataRead()))
		ActSRead=(sizeToRead*byteSize);
	else
		ActSRead=(OggPtr->GetDataSize() - OggPtr->GetDataRead());

	if (ActSRead)
	{
		memcpy(ptr, (char*)OggPtr->GetMemPtr() + OggPtr->GetDataRead(), ActSRead);
		int Plus=OggPtr->GetDataRead();
		OggPtr->SetDataRead(ActSRead+Plus);
	}

	return ActSRead;
}

int VorbisClose(void *datasource){

	OggAudio *OggPtr;
	
	OggPtr=(OggAudio*)datasource;

	//OggPtr->unload();

	return 1;
}

long VorbisTell(void *datasource)
{
	OggAudio *OggPtr;
	
	OggPtr=(OggAudio*)datasource;

	return OggPtr->GetDataRead();;
}

int VorbisSeek(void *datasource,ogg_int64_t offset,int whence)
{
	size_t				spaceToEOF;
	ogg_int64_t			actualOffset;

	OggAudio *OggPtr;
	
	OggPtr=(OggAudio*)datasource;

	switch (whence)
	{
	case SEEK_SET: 
		if (OggPtr->GetDataSize() >= offset)
			actualOffset = offset;
		else
			actualOffset = OggPtr->GetDataSize();
		
		OggPtr->SetDataRead((int)actualOffset);
		break;
	case SEEK_CUR:
		spaceToEOF = OggPtr->GetDataSize()-OggPtr->GetDataRead();

		if (offset < spaceToEOF)
			actualOffset = (offset);
		else
			actualOffset = spaceToEOF;	
		
		OggPtr->SetDataRead(OggPtr->GetDataRead()+actualOffset);
		break;
	case SEEK_END: 
		OggPtr->SetDataRead(OggPtr->GetDataSize()+1);
		break;
	default:
		//error
		break;
	};

	return 0;
}

OggAudio::OggAudio()
{

}

OggAudio::OggAudio(char* fileName)
{
	size_t size;

	size=strlen(fileName);
	key = new char[size+1];
	memcpy(key,fileName,size);
	key[size]='\0';
	DataRead=0;
	DataSize=0;
	MemPtr=NULL;
    alGenBuffers(1,&buffer);
    alGenSources(1,&source);

    fuentePos.x(0.0f);
    fuentePos.y(0.0f);
    fuentePos.z(0.0f);

	fuenteVel.x(0.0f);
    fuenteVel.y(0.0f);
    fuenteVel.z(0.0f);
}

	/* constructor de copia */
OggAudio* OggAudio::copyAudio(OggAudio *o)
{
	OggAudio *ogg2 = new OggAudio(o->getKeyName());
	ogg2->load();
	ogg2->setPosXYZ(o->getPosXYZ());
	return ogg2;
}

int OggAudio::load()
{
	FILE *fp;
    int format;
    unsigned int size;
	unsigned int freq;

    unsigned char *data;

    OggVorbis_File vf;
    vorbis_info *vi;

	callback.close_func=&VorbisClose;
	callback.read_func=&VorbisRead;
	callback.seek_func=&VorbisSeek;
	callback.tell_func=&VorbisTell;

	fp = fopen(key,"rb");
	if (!fp){
		printf("[SoundServer] no se encontro el archivo: %s\n",key);
        return 0;
	}	
	
	int	sizeOfFile = 0;
	char tempChar;
	while (!feof(fp))
	{
		tempChar = getc(fp);
		sizeOfFile++;
	}

	MemPtr=new char[sizeOfFile];
	rewind(fp);

	int tempArray = 0;
	while (!feof(fp))
	{
		MemPtr[tempArray]=getc(fp);
		tempArray++;
	}

	fclose(fp);
	DataRead=0;
	DataSize=sizeOfFile;

	ov_open_callbacks(this,&vf,NULL,0,callback);
			
	vi = ov_info(&vf,-1);

    if(vi->channels == 1)
		format = AL_FORMAT_MONO16;	
	else
		format=AL_FORMAT_STEREO16;	

	size=(unsigned long)ov_time_total(&vf,-1) * vi->rate * vi->channels * 2;	
	freq = vi->rate;

	int sec=0,ret=1;
	unsigned long pos=0;

    data=new unsigned char[size];
    memset(data,0,size*sizeof(char));
    ov_read(&vf,(char*)data,size,0,2,1,&sec);

    while(ret && pos<size){
		ret=ov_read(&vf,(char*)data+pos,size-pos,0,2,1,&sec);
		pos+=ret;
	}
	alBufferData(buffer,format,data,size,freq);
	ov_clear(&vf);

	alSourcei (source, AL_BUFFER,	buffer);
	alSourcef (source, AL_PITCH,    1.0      );
	alSourcef (source, AL_GAIN,     1.0      );
	alSourcefv(source, AL_POSITION, fuentePos.GetVector());
	alSourcefv(source, AL_VELOCITY, fuenteVel.GetVector());

	return 1;
}

void OggAudio::unload()
{
	alDeleteBuffers(1,&buffer);
	alDeleteSources(1,&source);
}

void OggAudio::setKeyName(char* keyName)
{
    key = keyName;
}

char* OggAudio::getKeyName()
{
    return key;
}

void OggAudio::setPosXYZ(Vector3 vec)
{
    fuentePos = vec;
	alSourcefv(source, AL_POSITION, fuentePos.GetVector());
}

Vector3 OggAudio::getPosXYZ()
{
	return fuentePos;
}

void OggAudio::setLOOP(int val)
{
	if (val)
		alSourcei(source,AL_LOOPING,AL_TRUE);
	else
		alSourcei(source,AL_LOOPING,AL_FALSE);
}

ALuint OggAudio::getSource()
{
    return source;
}

ALuint OggAudio::getBuffer()
{
    return buffer;
}
