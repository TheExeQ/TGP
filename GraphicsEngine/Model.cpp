#include "GraphicsEngine.pch.h"
#include "Model.h"

void Model::Init(ModelData& someModelData, const std::string& aPath)
{
	myData.myVertexBuffer = someModelData.myVertexBuffer;
	myData.myIndexBuffer = someModelData.myIndexBuffer;
	myData.myVertexCount = someModelData.myVertexCount;
	myData.myIndexCount = someModelData.myIndexCount;
	
	myData.myInputLayout = someModelData.myInputLayout;
	myData.myVS = someModelData.myVS;
	myData.myPS = someModelData.myPS;

	myData.myPrimitiveTopology = someModelData.myPrimitiveTopology;
	myData.myStride = someModelData.myStride;
	myData.myOffset = someModelData.myOffset;
}
