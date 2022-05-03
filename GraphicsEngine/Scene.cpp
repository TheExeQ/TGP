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

Scene::Scene()
{
	myModelAssetHandler.Init();
}

Scene::~Scene()
{

}

const std::vector<std::shared_ptr<Model>> Scene::CullModels(const std::shared_ptr<Camera>& camera) const
{
	std::vector<std::shared_ptr<Model>> visibleModels;

	visibleModels.push_back(myModelAssetHandler.GetModel("Cube"));

	return visibleModels;
}

void Scene::SetMainCamera(const std::shared_ptr<Camera>& aCamera)
{
	myMainCamera = aCamera;
}
