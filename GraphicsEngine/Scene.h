#pragma once
#include <vector>
#include <memory>
#include "Math/Vector3.hpp"

using namespace CommonUtilities;

class Camera;

struct Transform
{
	Vector3<float> myPosition;
	Vector3<float> myRotation;
	Vector3<float> myScale;
};

class SceneObject
{
public:
	SceneObject() = default;
	virtual ~SceneObject() = default;

	void SetTransform(Transform someTransform);

	void SetPosition(Vector3<float> somePosition);
	void SetRotation(Vector3<float> someRotation);
	void SetScale(Vector3<float> someScale);

	void SetPosition(float someX, float someY, float someZ);
	void SetRotation(float someX, float someY, float someZ);
	void SetScale(float someX, float someY, float someZ);

	inline const Transform& GetTransform() const { return myTransform; };
	
protected:
	Transform myTransform;
};

class Scene
{
public:
	template<typename T>
	void AddGameObject(std::shared_ptr<T> aSceneObject)
	{
		mySceneObjects.push_back(std::move(aSceneObject));
	}

	void SetCamera(std::shared_ptr<Camera> aCamera);

private:
	std::vector<std::shared_ptr<SceneObject>> mySceneObjects;
	std::shared_ptr<Camera> myMainCamera;
};

