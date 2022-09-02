#include "SceneSerializer.h"
#include "Math/Vector.hpp"
#include "Core/DX11.h"

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

void SceneSerializer::Serialize(const char* aFileName)
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

	std::ofstream fout(aFileName);
	fout << out.c_str();
}

bool SceneSerializer::Deserialize(const char* aFileName)
{
	std::ifstream stream(aFileName);
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

void SceneSerializer::SerializeEntity(YAML::Emitter& outEmitter, Entity aEntity)
{

}
