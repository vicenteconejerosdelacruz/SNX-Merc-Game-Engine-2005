#include <math.h>
#include "Constants.h"
#include "Matrix44.h"

Matrix44::Matrix44()
{
	for(int x=0;x < 4;x++){
		for(int y=0;y < 4;y++){
			mMatrix[x][y] = 0.0f;
		}
	}
}

Matrix44::Matrix44(const Matrix44 &other)
{
	for(int x=0;x < 4;x++){
		for(int y=0;y < 4;y++){
			mMatrix[x][y] = other.mMatrix[x][y];
		}
	}
}

void Matrix44::LoadIdentity()
{
	for(int x=0;x < 4;x++){
		for(int y=0;y < 4;y++){
			if(x == y){
				mMatrix[x][y] = 1.0f;
			}else{
				mMatrix[x][y] = 0.0f;
			}
		}
	}
}

void Matrix44::BuildRotation(float rotx, float roty, float rotz)
{
	float angle;
	float sx, sy, sz, cx, cy, cz;

	LoadIdentity();
	angle = rotz * (PI / 180);
	sz = sin(angle);
	cz = cos(angle);

	angle = roty * (PI / 180);
	sy = sin(angle);
	cy = cos(angle);

	angle = rotx * (PI / 180);
	sx = sin(angle);
	cx = cos(angle);

	mMatrix[0][0] = cy*cz;
	mMatrix[1][0] = cy*sz;
	mMatrix[2][0] = -sy;

	mMatrix[0][1] = sx*sy*cz+cx*-sz;
	mMatrix[1][1] = sx*sy*sz+cx*cz;
	mMatrix[2][1] = sx*cy;

	mMatrix[0][2] = (cx*sy*cz+-sx*-sz);
	mMatrix[1][2] = (cx*sy*sz+-sx*cz);
	mMatrix[2][2] = cx*cy;

	mMatrix[0][3] = 0.0;
	mMatrix[1][3] = 0.0;
	mMatrix[2][3] = 0.0;
}

void Matrix44::BuildIRotation(float rotx, float roty, float rotz)
{
	float angle;
	float sx, sy, sz, cx, cy, cz;
	
	LoadIdentity();
	angle = rotz * (PI / 180);
	sz = sin(angle);
	cz = cos(angle);

	angle = roty * (PI / 180);
	sy = sin(angle);
	cy = cos(angle);

	angle = rotx * (PI / 180);
	sx = sin(angle);
	cx = cos(angle);

	mMatrix[0][0] = cy*cz;
	mMatrix[0][1] = cy*sz;
	mMatrix[0][2] = -sy;

	mMatrix[1][0] = sx*sy*cz+cz*-sz;
	mMatrix[1][1] = sx*sy*sz+cx*cz;
	mMatrix[1][2] = sx*cy;

	mMatrix[2][0] = (cx*sy*cz+-sx*-sz);
	mMatrix[2][1] = (cx*sy*sz+-sx*cz);
	mMatrix[2][2] = cx*cy;

	mMatrix[0][3] = 0.0;
	mMatrix[1][3] = 0.0;
	mMatrix[2][3] = 0.0;
}

void Matrix44::BuildTranslation(float x, float y, float z)
{
	LoadIdentity();
	mMatrix[0][3] = x;
	mMatrix[1][3] = y;
	mMatrix[2][3] = z;
}

void Matrix44::BuildTransformation(float rotx, float roty, float rotz, float x, float y, float z)
{
	BuildRotation(rotx,roty,rotz);
	BuildTranslation(x,y,z);
}

Vector3 &Matrix44::Transform(Vector3 &v)
{
	float x,y,z;

	Vector3 v1(mMatrix[0][0],mMatrix[0][1],mMatrix[0][2]);
	Vector3 v2(mMatrix[1][0],mMatrix[1][1],mMatrix[1][2]);
	Vector3 v3(mMatrix[2][0],mMatrix[2][1],mMatrix[2][2]);
	
	x = v.Dot(v1) + mMatrix[0][3];
	y = v.Dot(v2) + mMatrix[1][3];
	z = v.Dot(v3) + mMatrix[2][3];

	Vector3 *result = new Vector3(x,y,z);
	return(*result);
}

Vector3 &Matrix44::InverseTransform(Vector3 &v)
{
	Vector3 result = v;
	
	result.x(result.x() - mMatrix[0][3]);
	result.y(result.y() - mMatrix[1][3]);
	result.z(result.z() - mMatrix[2][3]);
	
	return(InverseRotate(result));
}

Vector3 &Matrix44::Rotate(Vector3 &v)
{
	float x,y,z;

	Vector3 v1(mMatrix[0][0],mMatrix[0][1],mMatrix[0][2]);
	Vector3 v2(mMatrix[1][0],mMatrix[1][1],mMatrix[1][2]);
	Vector3 v3(mMatrix[2][0],mMatrix[2][1],mMatrix[2][2]);
	
	x = v.Dot(v1);
	y = v.Dot(v2);
	z = v.Dot(v3);

	Vector3 *result = new Vector3(x,y,z);
	return(*result);
}

Vector3 &Matrix44::InverseRotate(Vector3 &v)
{
	float x,y,z;
	Vector3 v1(mMatrix[0][0],mMatrix[1][0],mMatrix[2][0]);
	Vector3 v2(mMatrix[0][1],mMatrix[1][1],mMatrix[2][1]);
	Vector3 v3(mMatrix[0][2],mMatrix[1][2],mMatrix[2][2]);
	
	x = v.Dot(v1);
	y = v.Dot(v2);
	z = v.Dot(v3);
	
	Vector3 *result = new Vector3(x,y,z);
	return(*result);
}

Matrix44 &Matrix44::Concat(Matrix44 &m)
{
	Matrix44 *result = new Matrix44();

	for(int i = 0;i < 4;i++){
		for(int j = 0;j < 4;j++){
			float sum = 0.0f;
			for(int k = 0;k < 4;k++){
				sum += mMatrix[i][k]*m.mMatrix[k][j];
			}
			result->mMatrix[i][j] = sum;
		}
	}

	return(*result);
}
