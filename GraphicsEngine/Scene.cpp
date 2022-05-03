#include "GraphicsEngine.pch.h"
#include "Camera.h"
#include "Scene.h"

void SceneObject::SetTransform(Transform someTransform)
{
	myTransform = someTransform;
}

void SceneObject::SetPosition(Vector3<float> somePosition)
{
	myTransform.myPosition = somePosition;
}

void SceneObject::SetPosition(float someX, float someY, float someZ)
{
	myTransform.myPosition = Vector3<float>(someX, someY, someZ);
}

void SceneObject::SetRotation(Vector3<float> someRotation)
{
	myTransform.myRotation = someRotation;
}

void SceneObject::SetRotation(float someX, float someY, float someZ)
{
	myTransform.myRotation = Vector3<float>(someX, someY, someZ);
}

void SceneObject::SetScale(Vector3<float> someScale)
{
	myTransform.myScale = someScale;
}

void SceneObject::SetScale(float someX, float someY, float someZ)
{
	myTransform.myScale = Vector3<float>(someX, someY, someZ);
}

void Scene::SetCamera(std::shared_ptr<Camera> aCamera)
{
	myMainCamera = aCamera;
}
