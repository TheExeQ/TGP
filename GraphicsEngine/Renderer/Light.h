#pragma once
#include <wrl/client.h>
#include <d3d11.h>
#include "Core/Base.h"
#include "Math/Vector.hpp"
#include "Scene/Scene.h"
#include "Math/Matrix.hpp"
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
		float Intensity = 0.f;
		Vector3<float> Direction;
		float Range = 0.f;
		Vector3<float> Position;
		float Attenuation = 0.f;

		float SpotInnerRadius = 0.f;
		float SpotOuterRadius = 0.f;
		int LightType = 0;
		bool CastShadows = true;

		float Near = 0.f;
		float Far = 0.f;

		Vector2f Padding;
	};

	virtual ~Light() = default;
	virtual void Init(Vector3<float> aColor, float anIntensity);

	virtual void SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer);

	void SetColor(Vector3f aColor) { ourLightBuffer.Color = aColor; };
	void SetIntensity(float aIntensity) { ourLightBuffer.Intensity = aIntensity; };
	void SetDirection(Vector3f aDirection) { ourLightBuffer.Direction = aDirection; };

	void SetShadowMapAsDepth(const int& aIndex);
	void SetShadowMapAsResource(const int& aSlot, const int& aAmount);

	_inline Vector4<float> GetColor() const { return Vector4(ourLightBuffer.Color.x, ourLightBuffer.Color.y, ourLightBuffer.Color.z, 1.f); };
	_inline float GetIntensity() const { return ourLightBuffer.Intensity; };
	_inline Vector3f GetDirection() const { return ourLightBuffer.Direction; };

	_inline LightBufferData GetLightBufferData() { return ourLightBuffer; };

	_inline void ClearShadowMap() 
	{
		for (auto& shadowMap : myShadowMaps)
		{
			if (!shadowMap) { continue; }
			shadowMap->Clear();
		}
	};

	void SetLightView(const Matrix4& lightView);

protected:
	LightBufferData ourLightBuffer;
	std::array<Ref<DepthStencil>, 6> myShadowMaps;

private:
	friend class LightAssetHandler;
	friend class SceneHierarchyPanel;
	friend class SceneSerializer;
};