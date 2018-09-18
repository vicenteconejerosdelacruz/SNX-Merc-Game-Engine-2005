#include <cmath>
#include "Vector3.h"
#include "Constants.h"

Vector3::Vector3()
{
	for(int i=0;i < 3;i++)
		mVector[i] = 0.0f;
}

Vector3::Vector3(const Vector3 &v)
{
	mVector[0] = v.mVector[0];
	mVector[1] = v.mVector[1];
	mVector[2] = v.mVector[2];
}

Vector3::Vector3(float X,float Y,float Z)
{
	mVector[0] = X;
	mVector[1] = Y;
	mVector[2] = Z;
}

Vector3& Vector3::Cross(const Vector3 &other)
{
	Vector3 *cross = new Vector3();

	cross->x(mVector[1]*other.mVector[2] - mVector[2]*other.mVector[1]);
	cross->y(mVector[2]*other.mVector[0] - mVector[0]*other.mVector[2]);
	cross->z(mVector[0]*other.mVector[1] - mVector[1]*other.mVector[0]);

	return(*cross);
}

float Vector3::Dot(const Vector3 &other)
{
	float result = 0.0f;

	for(int i=0;i < 3;i++)
		result+=other.mVector[i]*mVector[i];
	return(result);
}

float Vector3::Length()
{
	float result = 0.0f;

	for(int i=0;i < 3;i++)
		result += mVector[i]*mVector[i];
	result = sqrt(result); //ARREGLAR
	return(result);
}

void Vector3::Normalize()
{
	float length = Length();

	if(length != 0){
		for(int i=0;i < 3;i++)
			mVector[i] /= length;
	}
}

Vector3 &Vector3::operator+(const Vector3 &v2)
{	
	Vector3 *suma = new Vector3();

	for(int i=0;i < 3;i++)
		suma->mVector[i] = mVector[i]+v2.mVector[i];
	return(*suma);
}
	

Vector3 &Vector3::operator-(const Vector3 &v2)
{
	Vector3 *suma = new Vector3();

	for(int i=0;i < 3;i++)
		suma->mVector[i] = mVector[i]-v2.mVector[i];
	return(*suma);
}

/*cmnt
Vector3 &operator=(const Vector3 &v1,const Vector3 &v2)
{
	if(v1
	for(int i=0;i < 3;i++)
		v1.mVector[i] = v2.mVector[i];
}
*/

bool Vector3::operator==(const Vector3 &v2)
{
	bool result = true;

	for(int i=0;i < 3;i++){
		if(fabs(mVector[i] - v2.mVector[i]) > EPSILON){
			result = false;
		}
	}
	
	return(result);
}

void Vector3::operator *=(const float escalar){

	for(int i=0;i<3;i++)
		mVector[i]*=escalar;
}

void Vector3::operator +=(const Vector3 &v2)
{
	for(int i=0;i<3;i++)
		mVector[i]+=v2.mVector[i];
}

void Vector3::operator -=(const Vector3 &v2)
{
	for(int i=0;i<3;i++)
		mVector[i]-=v2.mVector[i];
}
