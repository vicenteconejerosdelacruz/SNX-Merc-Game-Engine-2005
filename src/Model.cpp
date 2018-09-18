#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/GLEXT.h>
#include "Model.h"
#include "Log.h"
#include "Cache.h"
#include "mathlib.h"

extern PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB;
extern PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB;
extern PFNGLMULTITEXCOORD2FPROC		glMultiTexCoord2fARB;

Model::Model()
{
	NumMeshes = 0;
	Meshes = NULL;
	NumMaterials = 0;
	Materials = NULL;
	NumTriangles = 0;
	Triangles = NULL;
	NumVertices = 0;
	Vertices = NULL;
	NumBones = 0;
	Bones = NULL;
	animFPS = 0;
	currentTime = 0;
	nFrames = 0;
	memset(dirname,0,255);
	memset(modelname,0,255);
	textures = NULL;
	loaded = false;
	CurrFrame =0;
}

Model::Model(char *filename)
{
	Model();
	Load(filename);
}

int Model::Load(char *filename)
{
	MS3DHeader *header;
	unsigned char *buffer;
	unsigned char *ptr;
	FILE *fp;
	long size;
	int i;
	char temp[255];
	int f=0;
	int f1=0;

	if(strlen(filename) > 255){
		Log::Output("Error: El nombre de archivo no puede ser mayor a 255 caracteres\n");
		return NULL;
	}
	
	memset(temp,0,sizeof(temp));
	strcat(temp,"models/");
	strcat(temp,filename);
	strcat(temp,"/");
	strcat(temp,filename);
	strcat(temp,".ms3d");

	strcat(modelname,filename);

	//Log::Output("Model: Abriendo: %s\n",temp);
	fp = fopen(temp,"rb");
	if(!fp){
		Log::Output("Model: Error: No se pudo abrir %s\n",filename);
		return NULL;
	}

	fseek(fp,0,SEEK_END);
	size = ftell(fp);
	fseek(fp,0,SEEK_SET);
	//Log::Output("Model: tamaño: %d\n",size);
	
	buffer = new unsigned char[size];
	fread(buffer,size,sizeof(unsigned char),fp);
	fclose(fp);
	
	ptr = buffer;
	header = (MS3DHeader*)ptr;
	ptr+=sizeof(MS3DHeader);
	
	if(strncmp(header->Id,"MS3D000000",10)!=0){
		Log::Output("Model: Error: No es un archivo MS3D valido\n");
		return -1;
	}
	
	if(header->Version < 3){
		Log::Output("Model: Error: Version no soportada\n");
		return -1;
	}
	
	NumVertices = *(unsigned short*)ptr;
	Vertices = new MS3DVertex[NumVertices];
	ptr+=sizeof(unsigned short);

	for(i=0;i<NumVertices;i++){
		MS3DVertex *aux = (MS3DVertex*)ptr;
		memcpy(&(Vertices[i]),aux,sizeof(MS3DVertex));

		ptr+=sizeof(MS3DVertex);
	}

	NumTriangles = *(unsigned short*)ptr;
	Triangles = new MS3DTriangle[NumTriangles];
	ptr+=sizeof(unsigned short);
	//Log::Output("Model: Triangles: %d\n",NumTriangles);
	
	for(i=0;i<NumTriangles;i++){
		MS3DTriangle *aux = (MS3DTriangle*)ptr;
		memcpy(&(Triangles[i]),aux,sizeof(MS3DTriangle));
		ptr+=sizeof(MS3DTriangle);
	}
	
	NumMeshes = *(unsigned short*)ptr;
	Meshes = new MS3DMesh[NumMeshes];
	ptr+=sizeof(unsigned short);
	//Log::Output("Model: Meshes: %d\n",NumMeshes);
	
	for(i=0;i<NumMeshes;i++){
		Meshes[i].Flags = *(unsigned char*)ptr;
		ptr+=sizeof(unsigned char);
		
		memcpy(Meshes[i].Name,ptr,sizeof(char)*32);
		ptr+=32;
		
		Meshes[i].NumTriangles = *(unsigned short*)ptr;
		ptr+=sizeof(unsigned short);
		
		Meshes[i].TriangleIndices = new unsigned short[Meshes[i].NumTriangles];
		memcpy(Meshes[i].TriangleIndices,ptr,sizeof(unsigned short)*Meshes[i].NumTriangles);
		ptr+=sizeof(unsigned short)*(Meshes[i].NumTriangles);
		
		Meshes[i].MaterialIndex = *(char*)ptr;
		ptr+=sizeof(char);
	}

	NumMaterials = *(unsigned short*)ptr;
	Materials = new MS3DMaterial[NumMaterials];
	ptr+=sizeof(unsigned short);

	//Log::Output("Model: Materials: %d\n",NumMaterials);
	for(i=0;i<NumMaterials;i++){
		MS3DMaterial *aux = (MS3DMaterial*)ptr;
		memcpy(&(Materials[i]),aux,sizeof(MS3DMaterial));
		ptr+=sizeof(MS3DMaterial);
	}
	
	// usamos un vector ke guarda los id de cada textura
	textures = new unsigned int[NumMaterials];
	animFPS = *(float*)ptr;
	ptr+=sizeof(float);
	
	currentTime = *(float*)ptr;
	ptr+=sizeof(float);
	
	nFrames = *(int*)ptr;
	ptr+=sizeof(int);
	//Log::Output("Model: Frames: %d\n",nFrames);

	NumBones = *(unsigned short*)ptr;
	ptr+=sizeof(unsigned short);
	Bones = new MS3DBone[NumBones];
	//Log::Output("Model: Bones: %d\n",NumBones);
	
	for(i=0;i<NumBones;i++){
		Bones[i].Flags = *(unsigned char*)ptr;
		ptr+=sizeof(unsigned char);

		memcpy(Bones[i].Name,ptr,sizeof(char)*32);
		ptr+=32;
		
		memcpy(Bones[i].ParentName,ptr,sizeof(char)*32);
		ptr+=32;
		
		memcpy(Bones[i].Rotation,ptr,sizeof(float)*3);
		ptr+=sizeof(float)*3;
		
		memcpy(Bones[i].Position,ptr,sizeof(float)*3);
		ptr+=sizeof(float)*3;
		
		Bones[i].NumRotationKeys = *(unsigned short*)ptr;
		ptr+=sizeof(unsigned short);
		
		Bones[i].NumPositionKeys = *(unsigned short*)ptr;
		ptr+=sizeof(unsigned short);
		
		Bones[i].RotationKeys = new MS3DRotationKey[Bones[i].NumRotationKeys];
		Bones[i].PositionKeys = new MS3DPositionKey[Bones[i].NumPositionKeys];
		
		memcpy(Bones[i].RotationKeys,ptr,sizeof(MS3DRotationKey)*Bones[i].NumRotationKeys);
		ptr+=Bones[i].NumRotationKeys * sizeof(MS3DRotationKey);
		
		memcpy(Bones[i].PositionKeys,ptr,sizeof(MS3DPositionKey)*Bones[i].NumPositionKeys);
		ptr+=Bones[i].NumPositionKeys * sizeof(MS3DPositionKey);
	}
	loaded = true;
	ReloadTextures();
	SetupBones();

	return 0;
}

void Model::SetupBones()
{
	int i;

	MatBones = new Bone[NumBones];
	
	for(i = 0;i < NumBones;i++){
		float Rot[3];
		
		Rot[0] = Bones[i].Rotation[0] * 180 / (float) Q_PI;
		Rot[1] = Bones[i].Rotation[1] * 180 / (float) Q_PI;
		Rot[2] = Bones[i].Rotation[2] * 180 / (float) Q_PI;
		AngleMatrix(Rot,MatBones[i].Relative);
		MatBones[i].Relative[0][3] = Bones[i].Position[0];
		MatBones[i].Relative[1][3] = Bones[i].Position[1];
		MatBones[i].Relative[2][3] = Bones[i].Position[2];

		int ParentBone = GetBoneByName(Bones[i].ParentName);
		if(ParentBone != -1){
			R_ConcatTransforms(MatBones[ParentBone].Absolute,MatBones[i].Relative,MatBones[i].Absolute);
			memcpy(MatBones[i].Final,MatBones[i].Absolute,sizeof(Matrix));
		}else{
			memcpy(MatBones[i].Absolute,MatBones[i].Relative,sizeof(Matrix));
			memcpy(MatBones[i].Final,MatBones[i].Relative,sizeof(Matrix));
		}
	}

	for(i = 0; i < NumVertices;i++){
		if(Vertices[i].BoneId != -1){
			float Tmp[3];

			Vertices[i].Vertex[0] -= MatBones[Vertices[i].BoneId].Absolute[0][3];
			Vertices[i].Vertex[1] -= MatBones[Vertices[i].BoneId].Absolute[1][3];
			Vertices[i].Vertex[2] -= MatBones[Vertices[i].BoneId].Absolute[2][3];

			VectorIRotate(Vertices[i].Vertex,MatBones[Vertices[i].BoneId].Absolute,Tmp);
			VectorCopy(Tmp,Vertices[i].Vertex);
		}
	}
}

int Model::GetBoneByName(char *name)
{
	if(loaded){
		int i;
		unsigned int len = (unsigned int)strlen(name);
		for(i=0;i<NumBones;i++){
			if((strncmp(Bones[i].Name,name,len) == 0) && (strlen(Bones[i].Name) == len)){
				return i;
			}
		}
	}
	return -1;
}

void Model::SetAnim(float startFrame,float endFrame)
{
	Start = (startFrame/animFPS);
	End = (endFrame/animFPS);
	if(startFrame<0){
		Start = 0;
	}
	if(endFrame > nFrames){
		End = (nFrames/animFPS);
	}
	CurrFrame = Start;
}

void Model::SetLoop(bool f)
{
	loop = f;
}

bool Model::IsLooping()
{
	return(loop);
}

void Model::AdvanceFrame()
{
	int i,j;

	float dt=Animation[CurrentAnim].frametime;
	CurrFrame += dt;
	
	if (CurrFrame > (float) End){
		if(loop){
			CurrFrame = Start;
		}else{
			CurrFrame = End;
		}
	}

	for(i = 0; i < NumBones;i++){
		MS3DBone *bone = &(Bones[i]);
		if(bone->NumPositionKeys == 0 && bone->NumRotationKeys == 0){
			memcpy(MatBones[i].Final,MatBones[i].Absolute,sizeof(Matrix));
		}else{
			float Pos[3];
			float Rot[4];

			MS3DPositionKey *LastPosKey = 0,*ThisPosKey = 0;
			for(j = 0; j < bone->NumPositionKeys;j++){
				MS3DPositionKey *PosKey = &(bone->PositionKeys[j]);
				if(PosKey->Time >= CurrFrame){
					ThisPosKey = PosKey;
					break;
				}
				LastPosKey = PosKey;
			}
			if(LastPosKey != 0 && ThisPosKey != 0){
				float d = ThisPosKey->Time - LastPosKey->Time;
				float s = (CurrFrame - LastPosKey->Time) / d;
				Pos[0] = LastPosKey->Position[0] + (ThisPosKey->Position[0] - LastPosKey->Position[0]) * s;
				Pos[1] = LastPosKey->Position[1] + (ThisPosKey->Position[1] - LastPosKey->Position[1]) * s;
				Pos[2] = LastPosKey->Position[2] + (ThisPosKey->Position[2] - LastPosKey->Position[2]) * s;
			}else if(LastPosKey == 0){
				VectorCopy(ThisPosKey->Position,Pos);
			}else if(ThisPosKey == 0){
				VectorCopy(LastPosKey->Position, Pos);
			}
			Matrix m;
			MS3DRotationKey *LastRotKey = 0,*ThisRotKey = 0;
			for(j = 0;j < bone->NumRotationKeys;j++){
				MS3DRotationKey *RotKey = &(bone->RotationKeys[j]);
				if(RotKey->Time >= CurrFrame){
					ThisRotKey = RotKey;
					break;
				}
				LastRotKey = RotKey;
			}
			if(LastRotKey != 0 && ThisRotKey != 0){
				float d = ThisRotKey->Time - LastRotKey->Time;
				float s = (CurrFrame-LastRotKey->Time) / d;
				float q1[4];
				float q2[4];
				float q[4];
		
				AngleQuaternion(LastRotKey->Rotation,q1);
				AngleQuaternion(ThisRotKey->Rotation,q2);
				QuaternionSlerp(q1,q2,s,q);
				QuaternionMatrix(q,m);
			}else if(LastRotKey == 0){
				Rot[0] = ThisRotKey->Rotation[0]*180 / (float) Q_PI;
				Rot[1] = ThisRotKey->Rotation[1]*180 / (float) Q_PI;
				Rot[2] = ThisRotKey->Rotation[2]*180 / (float) Q_PI;
				AngleMatrix(Rot,m);
			}else if(ThisRotKey == 0){
				Rot[0] = LastRotKey->Rotation[0]*180 / (float) Q_PI;
				Rot[1] = LastRotKey->Rotation[1]*180 / (float) Q_PI;
				Rot[2] = LastRotKey->Rotation[2]*180 / (float) Q_PI;
			}
			
			m[0][3] = Pos[0];
			m[1][3] = Pos[1];
			m[2][3] = Pos[2];
			R_ConcatTransforms (MatBones[i].Relative, m, MatBones[i].RelativeFinal);
			int ParentBone = GetBoneByName(bone->ParentName);
			if (ParentBone == -1)
			{
				memcpy (MatBones[i].Final, MatBones[i].RelativeFinal, sizeof (Matrix));
			}
			else
			{
				R_ConcatTransforms (MatBones[ParentBone].Final, MatBones[i].RelativeFinal, MatBones[i].Final);
			}
		}
	}
}

void Model::SetAnim(char *AnimationName){

	for(int i=0;i<NumAnimations;i++)
		if(!strcmp(Animation[i].name,AnimationName))
		{
			SetAnim(Animation[i].begin,Animation[i].end);
			CurrentAnim=i;
			return;
		}

}

int Model::GetTime(char *AnimationName){
	
	for(int i=0;i<NumAnimations;i++)
		if(!strcmp(Animation[i].name,AnimationName))
		{
			return Animation[i].totaltime;
		}

		return -1;
}

void Model::ReloadTextures()
{	
	int i;
	for(i=0;i < NumMaterials;i++){
		char textureFilename[255];
		MS3DMaterial *material;

		material=&Materials[i];

		memset(textureFilename,0,sizeof(textureFilename));
		strcat(textureFilename,"textures/");
		strcat(textureFilename,modelname);
		strcat(textureFilename,"/");
		if(!strncmp(material->Texture,".\\",2))
			strcat(textureFilename,(material->Texture)+2);
		else
			strcat(textureFilename,material->Texture);

		Cache::LoadFile(textureFilename,TEXTURE_FILE,&textures[i]);
	}
}
void Model::Render()
{
	int i,j,k;

	if(loaded){
		for(i = 0;i < NumMeshes;i++){
			char MatIndex = Meshes[i].MaterialIndex;
			
			if(MatIndex >= 0){
				glMaterialfv(GL_FRONT,GL_AMBIENT,Materials[MatIndex].Ambient);
				glMaterialfv(GL_FRONT,GL_DIFFUSE,Materials[MatIndex].Diffuse);
				glMaterialfv(GL_FRONT,GL_SPECULAR,Materials[MatIndex].Specular);
				glMaterialf(GL_FRONT,GL_SHININESS,Materials[MatIndex].Shininess);
				glMaterialfv(GL_FRONT,GL_EMISSION,Materials[MatIndex].Emissive);

				if(textures[MatIndex] > 0){
					glEnable(GL_TEXTURE_2D);
					glActiveTextureARB(GL_TEXTURE0_ARB);
					glBindTexture(GL_TEXTURE_2D,textures[MatIndex]);
				}else{
					glDisable(GL_TEXTURE_2D);
				}
			}else{
				glDisable(GL_TEXTURE_2D);
			}
			glBegin(GL_TRIANGLES);
			for(j=0;j<Meshes[i].NumTriangles;j++){
				int TriIndex = Meshes[i].TriangleIndices[j];
				MS3DTriangle *Triangle = &(Triangles[TriIndex]);
				for(k=0;k<3;k++){
					unsigned short index = Triangle->VertexIndices[k];
					glTexCoord2f(Triangle->s[k],Triangle->t[k]);
					glMultiTexCoord2fARB(GL_TEXTURE0_ARB,Triangle->s[k],Triangle->t[k]);
					glNormal3fv(Triangle->VertexNormals[k]);
					if (Vertices[index].BoneId == -1){
						glVertex3fv(Vertices[index].Vertex);
					}else{
						float Vertex[3];
						VectorRotate(Vertices[index].Vertex,MatBones[Vertices[index].BoneId].Final,Vertex);
						Vertex[0] += MatBones[Vertices[index].BoneId].Final[0][3];
						Vertex[1] += MatBones[Vertices[index].BoneId].Final[1][3];
						Vertex[2] += MatBones[Vertices[index].BoneId].Final[2][3];
						glVertex3fv(Vertex);
					}
				}
			}
			glEnd();
		}//end for i
	}//end if loaded
	glDisable(GL_TEXTURE_2D);
	glColor3f(255,255,255);
}