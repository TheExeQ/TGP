#include "ShaderStructs.hlsli"

VertexToPixel main(VertexInput input)
{
    VertexToPixel result;

    const float4 vertexObjectPosition = input.myPosition;

	// Move the vertex from Object Space (Local) to World space.
	// (Incidentally moving it from world to object space is the inverse of this)
    const float4 vertexWorldPosition = mul(OB_ToWorld, vertexObjectPosition);

	// Move the vertex from World to View Space (Camera space)
    const float4 vertexViewPosition = mul(FB_ToView, vertexWorldPosition);

	// And finally move the vertex from View Space to Clip space (Projection space).
    const float4 vertexProjectionPosition = mul(FB_ToProjection, vertexViewPosition);

	// The resulting VX position is the position on screen.
    result.myPosition = vertexProjectionPosition;

	// We have to pass values through from here to use them
	// in the Pixel Shader.
	// Move the vertex colors over from the input.
    result.myVxColor = input.myVxColor;
    result.myVxColor2 = input.myVxColor2;
    result.myVxColor3 = input.myVxColor3;
    result.myVxColor4 = input.myVxColor4;

    return result;
}
