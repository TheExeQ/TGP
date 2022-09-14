#pragma once
#include <wrl/client.h>
#include <d3d11.h>
#include "Core/Base.h"
#include "Math/Vector.hpp"
#include "Scene/Scene.h"
#include "Math/Matrix4x4.hpp"
#include "DepthStencil.h"

using namespace CommonUtilities;
using namespace Microsoft::WRL;

class Light
{
public:
	struct LightBufferData
	{
		Matrix4x4<float> LightView;
		Matrix4x4<float> LightProj;

		Vector3<float> Color;
		float Intensity;
		Vector3<float> Direction;
		float Range;
		Vector3<float> Position;
		float Attenuation;

		float SpotInnerRadius;
		float SpotOuterRadius;
		int LightType;
		bool CastShadows;

		float Near;
		float Far;

		Vector2f Padding;
	};

	virtual ~Light() = default;
	virtual void Init(Vector3<float> aColor, float anIntensity);

	virtual void SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer);

	void SetColor(Vector3f aColor) { ourLightBuffer.Color = aColor; };
	void SetIntensity(float aIntensity) { ourLightBuffer.Intensity = aIntensity; };
	void SetDirection(Vector3f aDirection) { ourLightBuffer.Direction = aDirection; };

	void SetShadowMapAsDepth();

	_inline Vector4<float> GetColor() const { return Vector4(ourLightBuffer.Color.x, ourLightBuffer.Color.y, ourLightBuffer.Color.z, 1.f); };
	_inline float GetIntensity() const { return ourLightBuffer.Intensity; };
	_inline Vector3f GetDirection() const { return ourLightBuffer.Direction; };

	_inline LightBufferData GetLightBufferData() { return ourLightBuffer; };

	_inline void ClearShadowMap() { myShadowMap->Clear(); };

protected:
	LightBufferData ourLightBuffer;
	Ref<DepthStencil> myShadowMap;

private:
	friend class LightAssetHandler;
	friend class SceneHierarchyPanel;
	friend class SceneSerializer;

};