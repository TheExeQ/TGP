#pragma once
#include "Scene/Scene.h"
#include "Math/Vector3.hpp"
#include "Math/Vector4.hpp"
#include "Math/Matrix4x4.hpp"

using namespace CommonUtilities;

class Camera
{
public:
	Camera() = default;
	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);
	const Matrix4x4<float>& GetProjectionMatrix() const { return myProjectionMatrix; };
	
	const float& GetNear() const { return myNear; };
	const float& GetFar() const { return myFar; };
	const float& GetFov() const { return myFov; };

private:
	friend class SceneHierarchyPanel;
	Matrix4x4<float> myProjectionMatrix;
	
	float myFov;
	float myNear;
	float myFar;
};

