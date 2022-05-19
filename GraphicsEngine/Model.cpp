#include "GraphicsEngine.pch.h"
#include "Model.h"

void Model::Init(std::vector<ModelData>& someModelData, const std::string& aName)
{
	myName = aName;
	myData = someModelData;
}

void Model::Init(std::vector<ModelData>& someModelData, const std::string& aName, Skeleton& aSkeleton)
{
	myName = aName;
	myData = someModelData;
	mySkeleton = aSkeleton;
}

void Model::AddAnimation(Animation aAnimation)
{
	//mySkeleton.Animations.insert(aAnimation.Name, aAnimation);
}
