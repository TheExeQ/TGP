#include "Core/GraphicsEngine.pch.h"
#include "Model.h"

void Model::Init(std::vector<ModelData>& someModelData, const std::string& aName)
{
	myName = aName;
	myData = someModelData;
	mySkeleton = Skeleton();
}

void Model::Init(std::vector<ModelData>& someModelData, const std::string& aName, Skeleton& aSkeleton)
{
	myName = aName;
	myData = someModelData;
	mySkeleton = aSkeleton;
}

void Model::AddAnimation(Animation aAnimation)
{
	mySkeleton.Animations[aAnimation.Name] = aAnimation;
}

void Model::SetMaterial(Ref<Material> aMaterial)
{
	for (auto& data : myData)
	{
		data.myMaterial = aMaterial;
	}
}
