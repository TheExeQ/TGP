#include "GraphicsEngine.pch.h"
#include "Camera.h"
#include "Scene.h"
#include "ModelInstance.h"
#include "GraphicsEngine.h"
#include "ParticleSystem.h"

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

void SceneObject::AdjustPosition(float someX, float someY, float someZ)
{
	SetPosition(myTransform.myPosition.x + someX, myTransform.myPosition.y + someY, myTransform.myPosition.z + someZ);
}

void SceneObject::SetRotation(Vector3<float> someRotation)
{
	SetRotation(someRotation.x, someRotation.y, someRotation.z);
}

void SceneObject::SetRotation(float someX, float someY, float someZ)
{
	if (someX > 360)
	{
		someX -= 360;
	}
	if (someX < 0)
	{
		someX = 360;
	}
	if (someY > 360)
	{
		someY -= 360;
	}
	if (someY < 0)
	{
		someY = 360;
	}
	if (someZ > 360)
	{
		someZ -= 360;
	}
	if (someZ < 0)
	{
		someZ = 360;
	}
	myTransform.myRotation = { someX, someY, someZ};
	myTransform.myMatrix =
		myTransform.myMatrix.CreateRotationAroundX(someX * (PI / 180)) *
		myTransform.myMatrix.CreateRotationAroundY(someY * (PI / 180)) *
		myTransform.myMatrix.CreateRotationAroundZ(someZ * (PI / 180));
	SetPosition(myTransform.myPosition);
}

void SceneObject::AdjustRotation(float someX, float someY, float someZ)
{
	SetRotation(myTransform.myRotation.x + someX, myTransform.myRotation.y + someY, myTransform.myRotation.z + someZ);
}

void SceneObject::SetScale(Vector3<float> someScale)
{
	SetScale(someScale.x, someScale.y, someScale.z);
}

void SceneObject::SetScale(float someX, float someY, float someZ)
{
	myTransform.myScale = Vector3<float>(someX, someY, someZ);
}

void SceneObject::AdjustScale(float someX, float someY, float someZ)
{
	SetScale(myTransform.myScale.x + someX, myTransform.myScale.y + someY, myTransform.myScale.z + someZ);
}

void SceneObject::MoveForward(float someSpeed)
{
	CommonUtilities::Vector3<float> forward;
	forward.x = myTransform.myMatrix(3, 1);
	forward.y = myTransform.myMatrix(3, 2);
	forward.z = myTransform.myMatrix(3, 3);
	AdjustPosition(forward.x * someSpeed, forward.y * someSpeed, forward.z * someSpeed);
}

void SceneObject::MoveUp(float someSpeed)
{
	CommonUtilities::Vector3<float> up;
	up.x = myTransform.myMatrix(2, 1);
	up.y = myTransform.myMatrix(2, 2);
	up.z = myTransform.myMatrix(2, 3);
	AdjustPosition(up.x * someSpeed, up.y * someSpeed, up.z * someSpeed);
}

void SceneObject::MoveRight(float someSpeed)
{
	CommonUtilities::Vector3<float> right;
	right.x = myTransform.myMatrix(1, 1);
	right.y = myTransform.myMatrix(1, 2);
	right.z = myTransform.myMatrix(1, 3);
	AdjustPosition(right.x * someSpeed, right.y * someSpeed, right.z * someSpeed);
}

Scene::Scene()
{
	mySceneObjects.clear();
}

const std::vector<std::shared_ptr<ModelInstance>> Scene::CullModels(const std::shared_ptr<Camera>& camera) const
{
	std::vector<std::shared_ptr<ModelInstance>> visibleModels;

	for (auto& sceneObject : mySceneObjects)
	{
		auto mdlInst = dynamic_pointer_cast<ModelInstance>(sceneObject);
		if (mdlInst)
		{
			mdlInst->Update();
			visibleModels.push_back(mdlInst);
		}
	}

	return visibleModels;
}

const std::vector<std::shared_ptr<ParticleSystem>> Scene::CullParticles(const std::shared_ptr<Camera>& camera) const
{
	std::vector<std::shared_ptr<ParticleSystem>> visibleParticles;

	for (auto& sceneObject : mySceneObjects)
	{
		auto particleSystem = dynamic_pointer_cast<ParticleSystem>(sceneObject);
		if (particleSystem)
		{
			particleSystem->Update(GraphicsEngine::GetTimer().GetDeltaTime());
			visibleParticles.push_back(particleSystem);
		}
	}

	return visibleParticles;
}

void Scene::SetMainCamera(const std::shared_ptr<Camera>& aCamera)
{
	myMainCamera = aCamera;
}
