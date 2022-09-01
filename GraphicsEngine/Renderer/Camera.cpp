#include "Core/GraphicsEngine.pch.h"
#include "Camera.h"
#include <cassert>

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	assert(nearZ < farZ);
	assert(nearZ >= 0.01f);

	myFov = fovDegrees;
	myNear = nearZ;
	myFar = farZ;
	
	const float hFOV = fovDegrees * (3.14f / 180);
	const float vFOV = 2 * std::atan(std::tan(hFOV * 0.5f) * aspectRatio);

	const float myXScale = 1 / std::tanf(hFOV * 0.5f);
	const float myYScale = 1 / std::tanf(vFOV * 0.5f);
	const float Q = myFar / (myFar - myNear);
	
	myProjectionMatrix(1, 1) = myXScale;
	myProjectionMatrix(2, 2) = myYScale;
	myProjectionMatrix(3, 3) = Q;
	myProjectionMatrix(3, 4) = 1.f / Q;
	myProjectionMatrix(4, 3) = -Q * myNear;
	myProjectionMatrix(4, 4) = 0.f;
}