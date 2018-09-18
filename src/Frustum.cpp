/*#include <math.h>
#include <windows.h>
#include <GL/GL.h>
#include "Constants.h"
#include "Frustum.h"

float Frustum::ViewFrustum[6][4];

void Frustum::CalculateFrustum()
{
	float fPMtrx[16];	//projection matrix
	float fMMtrx[16];	//modelview matrix
	float clip[16];
	float fNorm;

	glGetFloatv( GL_PROJECTION_MATRIX, fPMtrx );


	glGetFloatv( GL_MODELVIEW_MATRIX, fMMtrx );


	clip[ 0]= fMMtrx[ 0]*fPMtrx[ 0] + fMMtrx[ 1]*fPMtrx[ 4] + fMMtrx[ 2]*fPMtrx[ 8] + fMMtrx[ 3]*fPMtrx[12];
	clip[ 1]= fMMtrx[ 0]*fPMtrx[ 1] + fMMtrx[ 1]*fPMtrx[ 5] + fMMtrx[ 2]*fPMtrx[ 9] + fMMtrx[ 3]*fPMtrx[13];
	clip[ 2]= fMMtrx[ 0]*fPMtrx[ 2] + fMMtrx[ 1]*fPMtrx[ 6] + fMMtrx[ 2]*fPMtrx[10] + fMMtrx[ 3]*fPMtrx[14];
	clip[ 3]= fMMtrx[ 0]*fPMtrx[ 3] + fMMtrx[ 1]*fPMtrx[ 7] + fMMtrx[ 2]*fPMtrx[11] + fMMtrx[ 3]*fPMtrx[15];

	clip[ 4]= fMMtrx[ 4]*fPMtrx[ 0] + fMMtrx[ 5]*fPMtrx[ 4] + fMMtrx[ 6]*fPMtrx[ 8] + fMMtrx[ 7]*fPMtrx[12];
	clip[ 5]= fMMtrx[ 4]*fPMtrx[ 1] + fMMtrx[ 5]*fPMtrx[ 5] + fMMtrx[ 6]*fPMtrx[ 9] + fMMtrx[ 7]*fPMtrx[13];
	clip[ 6]= fMMtrx[ 4]*fPMtrx[ 2] + fMMtrx[ 5]*fPMtrx[ 6] + fMMtrx[ 6]*fPMtrx[10] + fMMtrx[ 7]*fPMtrx[14];
	clip[ 7]= fMMtrx[ 4]*fPMtrx[ 3] + fMMtrx[ 5]*fPMtrx[ 7] + fMMtrx[ 6]*fPMtrx[11] + fMMtrx[ 7]*fPMtrx[15];

	clip[ 8]= fMMtrx[ 8]*fPMtrx[ 0] + fMMtrx[ 9]*fPMtrx[ 4] + fMMtrx[10]*fPMtrx[ 8] + fMMtrx[11]*fPMtrx[12];
	clip[ 9]= fMMtrx[ 8]*fPMtrx[ 1] + fMMtrx[ 9]*fPMtrx[ 5] + fMMtrx[10]*fPMtrx[ 9] + fMMtrx[11]*fPMtrx[13];
	clip[10]= fMMtrx[ 8]*fPMtrx[ 2] + fMMtrx[ 9]*fPMtrx[ 6] + fMMtrx[10]*fPMtrx[10] + fMMtrx[11]*fPMtrx[14];
	clip[11]= fMMtrx[ 8]*fPMtrx[ 3] + fMMtrx[ 9]*fPMtrx[ 7] + fMMtrx[10]*fPMtrx[11] + fMMtrx[11]*fPMtrx[15];

	clip[12]= fMMtrx[12]*fPMtrx[ 0] + fMMtrx[13]*fPMtrx[ 4] + fMMtrx[14]*fPMtrx[ 8] + fMMtrx[15]*fPMtrx[12];
	clip[13]= fMMtrx[12]*fPMtrx[ 1] + fMMtrx[13]*fPMtrx[ 5] + fMMtrx[14]*fPMtrx[ 9] + fMMtrx[15]*fPMtrx[13];
	clip[14]= fMMtrx[12]*fPMtrx[ 2] + fMMtrx[13]*fPMtrx[ 6] + fMMtrx[14]*fPMtrx[10] + fMMtrx[15]*fPMtrx[14];
	clip[15]= fMMtrx[12]*fPMtrx[ 3] + fMMtrx[13]*fPMtrx[ 7] + fMMtrx[14]*fPMtrx[11] + fMMtrx[15]*fPMtrx[15];

	
	ViewFrustum[FRUSTUM_RIGHT][0]= clip[ 3] - clip[ 0];
	ViewFrustum[FRUSTUM_RIGHT][1]= clip[ 7] - clip[ 4];
	ViewFrustum[FRUSTUM_RIGHT][2]= clip[11] - clip[ 8];
	ViewFrustum[FRUSTUM_RIGHT][3]= clip[15] - clip[12];

	//normalize the plane
	fNorm= sqrtf( SQR( ViewFrustum[FRUSTUM_RIGHT][0] )+
				  SQR( ViewFrustum[FRUSTUM_RIGHT][1] )+
				  SQR( ViewFrustum[FRUSTUM_RIGHT][2] ) );
	ViewFrustum[FRUSTUM_RIGHT][0]/= fNorm;
	ViewFrustum[FRUSTUM_RIGHT][1]/= fNorm;
	ViewFrustum[FRUSTUM_RIGHT][2]/= fNorm;
	ViewFrustum[FRUSTUM_RIGHT][3]/= fNorm;

	
	ViewFrustum[FRUSTUM_LEFT][0]= clip[ 3] + clip[ 0];
	ViewFrustum[FRUSTUM_LEFT][1]= clip[ 7] + clip[ 4];
	ViewFrustum[FRUSTUM_LEFT][2]= clip[11] + clip[ 8];
	ViewFrustum[FRUSTUM_LEFT][3]= clip[15] + clip[12];

	//normalize the plane
	fNorm= sqrtf( SQR( ViewFrustum[FRUSTUM_LEFT][0] )+
				  SQR( ViewFrustum[FRUSTUM_LEFT][1] )+
				  SQR( ViewFrustum[FRUSTUM_LEFT][2] ) );
	ViewFrustum[FRUSTUM_LEFT][0]/= fNorm;
	ViewFrustum[FRUSTUM_LEFT][1]/= fNorm;
	ViewFrustum[FRUSTUM_LEFT][2]/= fNorm;
	ViewFrustum[FRUSTUM_LEFT][3]/= fNorm;

	
	ViewFrustum[FRUSTUM_BOTTOM][0]= clip[ 3] + clip[ 1];
	ViewFrustum[FRUSTUM_BOTTOM][1]= clip[ 7] + clip[ 5];
	ViewFrustum[FRUSTUM_BOTTOM][2]= clip[11] + clip[ 9];
	ViewFrustum[FRUSTUM_BOTTOM][3]= clip[15] + clip[13];

	//normalize the plane
	fNorm= sqrtf( SQR( ViewFrustum[FRUSTUM_BOTTOM][0] )+
				  SQR( ViewFrustum[FRUSTUM_BOTTOM][1] )+
				  SQR( ViewFrustum[FRUSTUM_BOTTOM][2] ) );
	ViewFrustum[FRUSTUM_BOTTOM][0]/= fNorm;
	ViewFrustum[FRUSTUM_BOTTOM][1]/= fNorm;
	ViewFrustum[FRUSTUM_BOTTOM][2]/= fNorm;
	ViewFrustum[FRUSTUM_BOTTOM][3]/= fNorm;

	
	ViewFrustum[FRUSTUM_TOP][0]= clip[ 3] - clip[ 1];
	ViewFrustum[FRUSTUM_TOP][1]= clip[ 7] - clip[ 5];
	ViewFrustum[FRUSTUM_TOP][2]= clip[11] - clip[ 9];
	ViewFrustum[FRUSTUM_TOP][3]= clip[15] - clip[13];

	//normalize the plane
	fNorm= sqrtf( SQR( ViewFrustum[FRUSTUM_TOP][0] )+
				  SQR( ViewFrustum[FRUSTUM_TOP][1] )+
				  SQR( ViewFrustum[FRUSTUM_TOP][2] ) );
	ViewFrustum[FRUSTUM_TOP][0]/= fNorm;
	ViewFrustum[FRUSTUM_TOP][1]/= fNorm;
	ViewFrustum[FRUSTUM_TOP][2]/= fNorm;
	ViewFrustum[FRUSTUM_TOP][3]/= fNorm;

	
	ViewFrustum[FRUSTUM_FAR][0]= clip[ 3] - clip[ 2];
	ViewFrustum[FRUSTUM_FAR][1]= clip[ 7] - clip[ 6];
	ViewFrustum[FRUSTUM_FAR][2]= clip[11] - clip[10];
	ViewFrustum[FRUSTUM_FAR][3]= clip[15] - clip[14];

	//normalize the plane
	fNorm= sqrtf( SQR( ViewFrustum[FRUSTUM_FAR][0] )+
				  SQR( ViewFrustum[FRUSTUM_FAR][1] )+
				  SQR( ViewFrustum[FRUSTUM_FAR][2] ) );
	ViewFrustum[FRUSTUM_FAR][0]/= fNorm;
	ViewFrustum[FRUSTUM_FAR][1]/= fNorm;
	ViewFrustum[FRUSTUM_FAR][2]/= fNorm;
	ViewFrustum[FRUSTUM_FAR][3]/= fNorm;

	
	ViewFrustum[FRUSTUM_NEAR][0]= clip[ 3] + clip[ 2];
	ViewFrustum[FRUSTUM_NEAR][1]= clip[ 7] + clip[ 6];
	ViewFrustum[FRUSTUM_NEAR][2]= clip[11] + clip[10];
	ViewFrustum[FRUSTUM_NEAR][3]= clip[15] + clip[14];

	//normalize the plane
	fNorm= sqrtf( SQR( ViewFrustum[FRUSTUM_NEAR][0] )+
				  SQR( ViewFrustum[FRUSTUM_NEAR][1] )+
				  SQR( ViewFrustum[FRUSTUM_NEAR][2] ) );
	ViewFrustum[FRUSTUM_NEAR][0]/= fNorm;
	ViewFrustum[FRUSTUM_NEAR][1]/= fNorm;
	ViewFrustum[FRUSTUM_NEAR][2]/= fNorm;
	ViewFrustum[FRUSTUM_NEAR][3]/= fNorm;
}

bool Frustum::isSphereIn(float x, float y, float z, float Radius)
{

	int i;

	for( i=0; i<6; i++ )
	{
		if( ViewFrustum[i][0] * x + 
			ViewFrustum[i][1] * y + 
			ViewFrustum[i][2] * z + 
			ViewFrustum[i][3] < -Radius )
			return false;
	}

	return true;
}

bool Frustum::isCubeIn(float x,float y,float z,float size)
{
	int i;

	for( i=0; i<6; i++ )
	{
		if( ViewFrustum[i][0] * ( x-size )+ViewFrustum[i][1] * 
								( y-size )+ViewFrustum[i][2] * 
								( z-size )+ViewFrustum[i][3] > 0 )
			continue;
		if( ViewFrustum[i][0] * ( x+size )+ViewFrustum[i][1] * 
								( y-size )+ViewFrustum[i][2] * 
								( z-size )+ViewFrustum[i][3] > 0 )
			continue;
		if( ViewFrustum[i][0] * ( x-size )+ViewFrustum[i][1] * 
								( y+size )+ViewFrustum[i][2] * 
								( z-size )+ViewFrustum[i][3] > 0 )
			continue;
		if( ViewFrustum[i][0] * ( x+size )+ViewFrustum[i][1] * 
								( y+size )+ViewFrustum[i][2] * 
								( z-size )+ViewFrustum[i][3] > 0 )
			continue;
		if( ViewFrustum[i][0] * ( x-size )+ViewFrustum[i][1] *
								( y-size )+ViewFrustum[i][2] *
								( z+size )+ViewFrustum[i][3] > 0 )
			continue;
		if( ViewFrustum[i][0] * ( x+size )+ViewFrustum[i][1] *
								( y-size )+ViewFrustum[i][2] * 
								( z+size )+ViewFrustum[i][3] > 0 )
			continue;
		if( ViewFrustum[i][0] * ( x-size )+ViewFrustum[i][1] *
								( y+size )+ViewFrustum[i][2] * 
								( z+size )+ViewFrustum[i][3] > 0 )
			continue;
		if( ViewFrustum[i][0] * ( x+size )+ViewFrustum[i][1] *
								( y+size )+ViewFrustum[i][2] * 
								( z+size )+ViewFrustum[i][3] > 0 )
			continue;

		return false;
	}
	
	return true;
}
*/