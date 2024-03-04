#include "pch.h"
#include "Math.h"
#include <float.h>
#include <cmath>

using namespace DirectX;

const float Math::Infinity = FLT_MAX;
const float Math::Pi = 3.1415926535f;

float Math::AngleFromXY(float x, float y)
{
	// Use atan2 to directly get the angle in the range [-pi, pi]
	float theta = atan2(y, x);

	if (theta < 0.0f)
		theta += 2.0f * Pi; // Convert negative angles to the [0, 2*pi) range

	return theta;
	//float theta = 0.0f;

	//// Quadrant I or IV
	//if (x >= 0.0f)
	//{
	//	// If x = 0, then atanf(y/x) = +pi/2 if y > 0
	//	//                atanf(y/x) = -pi/2 if y < 0
	//	theta = atanf(y / x); // in [-pi/2, +pi/2]

	//	if (theta < 0.0f)
	//		theta += 2.0f * Pi; // in [0, 2*pi).
	//}

	//// Quadrant II or III
	//else
	//	theta = atan(y / x) + Pi; // in [0, 2*pi).

	//return theta;
}

XMVECTOR Math::RandUnitVec3()
{
	XMVECTOR One = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR Zero = XMVectorZero();

	// Keep trying until we get a point on/in the hemisphere.
	while (true)
	{
		// Generate random point in the cube [-1,1]^3.
		XMVECTOR v = XMVectorSet(Math::RandF(-1.0f, 1.0f), Math::RandF(-1.0f, 1.0f), Math::RandF(-1.0f, 1.0f), 0.0f);

		// Ignore points outside the unit sphere in order to get an even distribution 
		// over the unit sphere.  Otherwise points will clump more on the sphere near 
		// the corners of the cube.

		if (XMVector3Greater(XMVector3LengthSq(v), One))
			continue;

		return XMVector3Normalize(v);
	}
}

XMVECTOR Math::RandHemisphereUnitVec3(XMVECTOR n)
{
	XMVECTOR One = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR Zero = XMVectorZero();

	// Keep trying until we get a point on/in the hemisphere.
	while (true)
	{
		// Generate random point in the cube [-1,1]^3.
		XMVECTOR v = XMVectorSet(Math::RandF(-1.0f, 1.0f), Math::RandF(-1.0f, 1.0f), Math::RandF(-1.0f, 1.0f), 0.0f);

		// Ignore points outside the unit sphere in order to get an even distribution 
		// over the unit sphere.  Otherwise points will clump more on the sphere near 
		// the corners of the cube.

		if (XMVector3Greater(XMVector3LengthSq(v), One))
			continue;

		// Ignore points in the bottom hemisphere.
		if (XMVector3Less(XMVector3Dot(n, v), Zero))
			continue;

		return XMVector3Normalize(v);
	}
}