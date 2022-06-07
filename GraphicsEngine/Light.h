#pragma once
#include <wrl/client.h>
#include <d3d11.h>
#include "Math/Vector.hpp"
#include "Scene.h"

using namespace CommonUtilities;
using namespace Microsoft::WRL;

class Light : public SceneObject
{
public:
	struct LightBufferData
	{
		Vector3<float> Color;
		float Intensity;
		Vector3<float> Direction;
		float padding;
	};

	virtual ~Light() override = default;
	virtual void Init(Vector3<float> aColor, float anIntensity);

	virtual void SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer) = 0;

	_inline Vector4<float> GetColor() const { return Vector4(ourlightBuffer.Color.x, ourlightBuffer.Color.y, ourlightBuffer.Color.z, 1.f); };
	_inline float GetIntensity() const { return ourlightBuffer.Intensity; };

protected:
	LightBufferData ourlightBuffer;

private:
	friend class LightAssetHandler;

};