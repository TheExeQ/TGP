#include "GraphicsEngine.pch.h"
#include "Camera.h"
#include "Scene.h"
#include "ModelInstance.h"
#include "CU/TimeHandler.hpp"

#define PI 3.14f

void SceneObject::SetTransform(Transform someTransform)
{
	myTransform = someTransform;
}

void SceneObject::SetPosition(Vector3<float> somePosition)
{
	SetPosition(somePosition.x, somePosition.y, somePosition.z);
}

void SceneObject::SetPosition(float someX, float someY, float someZ)
{
	myTransform.myPosition = Vector3<float>(someX, someY, someZ);
	myTransform.myMatrix(4, 1) = someX;
	myTransform.myMatrix(4, 2) = someY;
	myTransform.myMatrix(4, 3) = someZ;
}

void SceneObject::SetRotation(Vector3<float> someRotation)
{
	SetRotation(someRotation.x, someRotation.y, someRotation.z);
}

void SceneObject::SetRotation(float someX, float someY, float someZ)
{
	myTransform.myRotation = { someX, someY, someZ};
	myTransform.myMatrix =
		myTransform.myMatrix.CreateRotationAroundX(someX * (PI / 180)) *
		myTransform.myMatrix.CreateRotationAroundY(someY * (PI / 180)) *
		myTransform.myMatrix.CreateRotationAroundZ(someZ * (PI / 180));
}

void SceneObject::SetScale(Vector3<float> someScale)
{
	SetScale(someScale.x, someScale.y, someScale.z);
}

void SceneObject::SetScale(float someX, float someY, float someZ)
{
	myTransform.myScale = Vector3<float>(someX, someY, someZ);
}

Scene::Scene()
{
	mySceneObjects.clear();
}

const std::vector<std::shared_ptr<Model>> Scene::CullModels(const std::shared_ptr<Camera>& camera) const
{
	std::vector<std::shared_ptr<Model>> visibleModels;

	for (auto& sceneObject : mySceneObjects)
	{
		auto mdlInst = dynamic_pointer_cast<ModelInstance>(sceneObject);
		if (mdlInst)
		{
			mdlInst->Update(Time::GetDeltaTime());
			visibleModels.push_back(mdlInst->GetModel());
		}
	}

	return visibleModels;
}

void Scene::SetMainCamera(const std::shared_ptr<Camera>& aCamera)
{
	myMainCamera = aCamera;
}
