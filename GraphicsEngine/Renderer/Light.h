#pragma once
#include <wrl/client.h>
#include <d3d11.h>
#include "Math/Vector.hpp"
#include "Scene/Scene.h"

using namespace CommonUtilities;
using namespace Microsoft::WRL;

class Light
{
public:
	struct LightBufferData
	{
		Vector3<float> Color;
		float Intensity;
		Vector3<float> Direction;
		float Range;
		Vector3<float> Position;
		float Attenuation;

		float SpotInnerRadius;
		float SpotOuterRadius;
		int LightType;
		float Padding;
	};

	virtual ~Light() = default;
	virtual void Init(Vector3<float> aColor, float anIntensity);

	virtual void SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer);

	void SetColor(Vector3f aColor) { ourlightBuffer.Color = aColor; };
	void SetIntensity(float aIntensity) { ourlightBuffer.Intensity = aIntensity; };
	void SetDirection(Vector3f aDirection) { ourlightBuffer.Direction = aDirection; };

	_inline Vector4<float> GetColor() const { return Vector4(ourlightBuffer.Color.x, ourlightBuffer.Color.y, ourlightBuffer.Color.z, 1.f); };
	_inline float GetIntensity() const { return ourlightBuffer.Intensity; };
	_inline Vector3f GetDirection() const { return ourlightBuffer.Direction; };

	_inline LightBufferData GetLightBufferData() { return ourlightBuffer; };

protected:
	LightBufferData ourlightBuffer;

private:
	friend class LightAssetHandler;
	friend class SceneHierarchyPanel;
	friend class SceneSerializer;

};