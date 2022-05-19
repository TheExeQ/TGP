#include "GraphicsEngine.pch.h"
#include "Model.h"

void Model::Init(std::vector<ModelData>& someModelData, const std::string& aName)
{
	myName = aName;
	myData = someModelData;
}
