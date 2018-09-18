#ifndef MERC_FRUSTUM_H
#define MERC_FRUSTUM_H

#define FRUSTUM_RIGHT  0
#define FRUSTUM_LEFT   1
#define FRUSTUM_BOTTOM 2
#define FRUSTUM_TOP    3
#define FRUSTUM_FAR    4
#define FRUSTUM_NEAR   5

class Frustum
{
private:	
public:
	static float ViewFrustum[6][4];
	static void CalculateFrustum();
	static bool isSphereIn(float x,float y,float z,float Radius);
	static bool isCubeIn(float x, float y, float z, float size);
	float *operator[](int num)
	{return ViewFrustum[num];	}
};

#endif