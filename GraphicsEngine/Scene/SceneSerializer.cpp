#include "SceneSerializer.h"
#include "Math/Vector.hpp"
#include "Core/DX11.h"
#include "Components.h"
#include "Editor/SettingsPanel.h"

#include <fstream>

namespace YAML
{
	template<>
	struct convert<Vector3f>
	{
		static Node encode(const Vector3f& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, Vector3f& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};
}

void SceneSerializer::SerializeSettings(const char* aFileName)
{
	YAML::Emitter out;

	out << YAML::BeginMap;
	out << YAML::Key << "Preset1" << YAML::Value << SettingsPanel::preset1;
	out << YAML::Key << "Preset2" << YAML::Value << SettingsPanel::preset2;
	out << YAML::Key << "Blend" << YAML::Value << SettingsPanel::blend;
	out << YAML::Key << "BlendActive" << YAML::Value << SettingsPanel::blendActive;
	out << YAML::EndMap;

	std::ofstream fout((std::string(aFileName) + ".settings").c_str());
	fout << out.c_str();
}

void SceneSerializer::SerializePreset(const char* aFileName)
{
	YAML::Emitter out;

	out << YAML::BeginMap;
	out << YAML::Key << "ClearColor" << YAML::Value
		<< YAML::Flow
		<< YAML::BeginSeq
		<< DX11::myClearColor[0]
		<< DX11::myClearColor[1]
		<< DX11::myClearColor[2]
		<< DX11::myClearColor[3]
		<< YAML::EndSeq;
	out << YAML::EndMap;

	std::ofstream fout((std::string(aFileName) + ".preset").c_str());
	fout << out.c_str();
}

void SceneSerializer::Serialize(const char* aFileName)
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Entities";
	out << YAML::Value << YAML::BeginSeq;
	myScene->myRegistry.each([&](entt::entity aEntity)
		{
			Entity ent(aEntity, myScene);
			SerializeEntity(out, ent);
		});
	out << YAML::EndSeq;
	out << YAML::EndMap;

	std::ofstream fout((std::string(aFileName) + ".scene").c_str());
	fout << out.c_str();
}

bool SceneSerializer::DeserializeSettings(const char* aFileName)
{
	std::ifstream stream((std::string(aFileName) + ".settings").c_str());
	std::stringstream strStream;
	strStream << stream.rdbuf();

	YAML::Node data = YAML::Load(strStream.str());

	if (data["Preset1"])
	{
		SettingsPanel::preset1 = data["Preset1"].as<std::string>();
	}
	if (data["Preset2"])
	{
		SettingsPanel::preset2 = data["Preset2"].as<std::string>();
	}
	if (data["Blend"])
	{
		SettingsPanel::blend = data["Blend"].as<float>();
	}
	if (data["BlendActive"])
	{
		SettingsPanel::blendActive = data["BlendActive"].as<bool>();
	}

	return true;
}

bool SceneSerializer::DeserializePreset(const char* aFileName)
{
	std::ifstream stream((std::string(aFileName) + ".preset").c_str());
	std::stringstream strStream;
	strStream << stream.rdbuf();

	YAML::Node data = YAML::Load(strStream.str());

	if (!data["ClearColor"])
	{
		return false;
	}

	DX11::myClearColor = data["ClearColor"].as<std::array<float, 4>>();
	return true;
}

bool SceneSerializer::Deserialize(const char* aFileName)
{
	std::ifstream stream((std::string(aFileName) + ".scene").c_str());
	std::stringstream strStream;
	strStream << stream.rdbuf();

	YAML::Node data = YAML::Load(strStream.str());
	if (!data["Entities"])
	{
		return false;
	}

	auto entities = data["Entities"];
	if (entities)
	{
		for (auto ent : entities)
		{
			Entity DeserializedEntity;
			if (ent["TagComponent"])
			{
				auto tag = ent["TagComponent"]["Tag"].as<std::string>();
				auto uuid = ent["Entity"].as<uint64_t>();
				DeserializedEntity = myScene->CreateEntityWithID(uuid, tag.c_str(), myScene);
			}
			else { continue; }

			if (ent["TransformComponent"])
			{
				auto position = ent["TransformComponent"]["Position"];
				auto rotation = ent["TransformComponent"]["Rotation"];
				auto scale = ent["TransformComponent"]["Scale"];
				auto& comp = DeserializedEntity.AddComponent<TransformComponent>();
				comp.position = position.as<Vector3f>();
				comp.rotation = rotation.as<Vector3f>();
				comp.scale = scale.as<Vector3f>();
			}

			if (ent["ModelComponent"])
			{
				auto& comp = DeserializedEntity.AddComponent<ModelComponent>();
			}

			if (ent["CameraComponent"])
			{
				auto& comp = DeserializedEntity.AddComponent<CameraComponent>();
			}

			if (ent["ParticleSystemComponent"])
			{
				auto& comp = DeserializedEntity.AddComponent<ParticleSystemComponent>();
			}

			if (ent["LightComponent"])
			{
				auto& comp = DeserializedEntity.AddComponent<LightComponent>();
				comp.light.ourLightBuffer.Color = ent["LightComponent"]["Color"].as<Vector3f>();
				comp.light.ourLightBuffer.Intensity = ent["LightComponent"]["Intensity"].as<float>();
				comp.light.ourLightBuffer.Direction = ent["LightComponent"]["Direction"].as<Vector3f>();
				comp.light.ourLightBuffer.Range = ent["LightComponent"]["Range"].as<float>();
				comp.light.ourLightBuffer.Attenuation = ent["LightComponent"]["Attenuation"].as<float>();
				comp.light.ourLightBuffer.SpotInnerRadius = ent["LightComponent"]["SpotInnerRadius"].as<float>();
				comp.light.ourLightBuffer.SpotOuterRadius = ent["LightComponent"]["SpotOuterRadius"].as<float>();
				comp.light.ourLightBuffer.LightType = ent["LightComponent"]["LightType"].as<int>();
			}
		}
	}

	return true;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const Vector2f& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
	return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const Vector3f& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
	return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const Vector4f& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
	return out;
}

void SceneSerializer::SerializeEntity(YAML::Emitter& outEmitter, Entity aEntity)
{
	auto idComp = aEntity.GetComponent<IDComponent>();
	outEmitter << YAML::BeginMap;
	outEmitter << YAML::Key << "Entity" << YAML::Value << (uint64_t)idComp.uuid;

	if (aEntity.HasComponent<TagComponent>())
	{
		outEmitter << YAML::Key << "TagComponent";
		outEmitter << YAML::BeginMap;
		const auto& comp = aEntity.GetComponent<TagComponent>();
		outEmitter << YAML::Key << "Tag" << YAML::Value << comp.name;
		outEmitter << YAML::EndMap;
	}

	if (aEntity.HasComponent<TransformComponent>())
	{
		const auto& comp = aEntity.GetComponent<TransformComponent>();

		outEmitter << YAML::Key << "TransformComponent";
		outEmitter << YAML::BeginMap;
		outEmitter << YAML::Key << "Position" << YAML::Value << comp.position;
		outEmitter << YAML::Key << "Rotation" << YAML::Value << comp.rotation;
		outEmitter << YAML::Key << "Scale" << YAML::Value << comp.scale;
		outEmitter << YAML::EndMap;
	}

	if (aEntity.HasComponent<ModelComponent>())
	{
		const auto& comp = aEntity.GetComponent<ModelComponent>();

		outEmitter << YAML::Key << "ModelComponent";
		outEmitter << YAML::BeginMap;
		// Save data
		outEmitter << YAML::EndMap;
	}

	if (aEntity.HasComponent<CameraComponent>())
	{
		const auto& comp = aEntity.GetComponent<CameraComponent>();

		outEmitter << YAML::Key << "CameraComponent";
		outEmitter << YAML::BeginMap;
		// Save data
		outEmitter << YAML::EndMap;
	}

	if (aEntity.HasComponent<ParticleSystemComponent>())
	{
		const auto& comp = aEntity.GetComponent<ParticleSystemComponent>();

		outEmitter << YAML::Key << "ParticleSystemComponent";
		outEmitter << YAML::BeginMap;
		// Save data
		outEmitter << YAML::EndMap;
	}

	if (aEntity.HasComponent<LightComponent>())
	{
		const auto& comp = aEntity.GetComponent<LightComponent>();

		outEmitter << YAML::Key << "LightComponent";
		outEmitter << YAML::BeginMap;
		outEmitter << YAML::Key << "Color" << YAML::Value << comp.light.ourLightBuffer.Color;
		outEmitter << YAML::Key << "Intensity" << YAML::Value << comp.light.ourLightBuffer.Intensity;
		outEmitter << YAML::Key << "Direction" << YAML::Value << comp.light.ourLightBuffer.Direction;
		outEmitter << YAML::Key << "Range" << YAML::Value << comp.light.ourLightBuffer.Range;
		outEmitter << YAML::Key << "Attenuation" << YAML::Value << comp.light.ourLightBuffer.Attenuation;
		outEmitter << YAML::Key << "SpotInnerRadius" << YAML::Value << comp.light.ourLightBuffer.SpotInnerRadius;
		outEmitter << YAML::Key << "SpotOuterRadius" << YAML::Value << comp.light.ourLightBuffer.SpotOuterRadius;
		outEmitter << YAML::Key << "LightType" << YAML::Value << comp.light.ourLightBuffer.LightType;
		outEmitter << YAML::EndMap;
	}

	outEmitter << YAML::EndMap;
}
