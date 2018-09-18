#ifndef MERC_MODEL_H
#define MERC_MODEL_H

#pragma pack(push,packing)
#pragma pack(1)

#define MAX_MODELS		25

/*valores maximos*/
#define MAX_VERTICES 	8192
#define MAX_TRIANGLES 	16384
#define MAX_GROUPS 		128
#define MAX_MATERIALS 	128
#define MAX_JOINTS 		128
#define MAX_KEYFRAMES 	216

/*flags*/
#define SELECTED	1
#define HIDDEN		2
#define SELECTED2	4
#define DIRTY		8

struct MS3DHeader{
	char Id[10]; 
	int Version;
};

struct MS3DVertex{
	unsigned char Flags;
	float Vertex[3];
	char BoneId;
	unsigned char RefCount;
};

struct MS3DTriangle{
	unsigned short Flags;
	unsigned short VertexIndices[3];
	float VertexNormals[3][3];
	float s[3];
	float t[3];
	unsigned char SmoothGroup;
	unsigned char GroupIndex;
};

struct MS3DMesh{
	unsigned char Flags;
	char Name[32];
	unsigned short NumTriangles;
	unsigned short *TriangleIndices;
	char MaterialIndex;
};

struct MS3DMaterial{
	char Name[32];
	float Ambient[4];
	float Diffuse[4];
	float Specular[4];
	float Emissive[4];
	float Shininess;
	float Transparency;
	unsigned char Mode;
	char Texture[128];
	char AlphaMap[128];
};

struct MS3DRotationKey{
	float Time;
	float Rotation[3];
};

struct MS3DPositionKey{
	float Time;
	float Position[3];
};

struct MS3DBone{
	unsigned char Flags;
	char Name[32];
	char ParentName[32];
	float Rotation[3];
	float Position[3];
	unsigned short NumRotationKeys;
	unsigned short NumPositionKeys;
	MS3DRotationKey *RotationKeys;
	MS3DPositionKey *PositionKeys;
};

typedef float Matrix[3][4];
struct Bone{
	Matrix Relative;
	Matrix Absolute;
	Matrix RelativeFinal;
	Matrix Final;
};

struct AnimationData{
	char name[32];
	float begin;
	float end;
	float frametime;
	float totaltime;
};

class Model
{
private:
	//variables privadas
	unsigned short NumMeshes;
	unsigned short NumMaterials;
	unsigned short NumTriangles;
	unsigned short NumVertices;
	unsigned short NumBones;
	MS3DMesh *Meshes;
	MS3DMaterial *Materials;
	MS3DTriangle *Triangles;
	MS3DVertex *Vertices;
	MS3DBone *Bones;
	Bone *MatBones;

	float animFPS;
	float currentTime;
	float CurrFrame;
	int nFrames;
	float Start;
	float End;
	char modelname[255];
	char dirname[255]; 
	unsigned int *textures;
	bool loaded;
	bool loop;

	AnimationData *Animation;
	int NumAnimations;
	int CurrentAnim;

	//metodos privados
	void SetupBones();
public:
	Model();
	Model(char *filename);
	int Load(char *filename);
	void Render();
	void ReloadTextures();
	void AdvanceFrame();
	void SetAnim(char *AnimationName);
	void SetAnim(float startFrame,float endFrame);
	int GetBoneByName(char *name);
	void SetLoop(bool f);
	bool IsLooping();
	void AddRotationKeyframe(int BoneIndex,float *Rotation);
	int GetRotKeyIndex(int BoneIndex,int frame);
	void AddPositionKeyframe(int BoneIndex,float *Position);
	int GetPosKeyIndex(int BoneIndex,int frame);
	Model *GetModel();
	int GetTime(char *AnimationName);
};

#pragma pack(pop,packing)
#endif
