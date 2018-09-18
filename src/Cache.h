#ifndef MERC_CACHE_H
#define MERC_CACHE_H

#define MAX_FILES		1000

enum File_Types{TEXTURE_FILE=1,MODEL_FILE};

typedef struct File{
	int type;
	char FileName[64];
	void *Data;
}File;

class Cache{
public:
	Cache();
	static void Init();
	static void LoadFile(char *FileName,int Type,...);

private:
	static File Files[MAX_FILES];
	static int NumFiles;
};

#endif
