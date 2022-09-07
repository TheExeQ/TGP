#include "SceneHierarchyPanel.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <string>

#include "Core/GraphicsEngine.h"
#include "Scene/Components.h"
#include "Math/Vector.hpp"
#include "Math/Matrix.hpp"
#include "Math/Mathf.hpp"

void SceneHierarchyPanel::OnImGuiRender()
{
	myContext = Scene::GetActiveScene();
	ImGui::Begin("Scene Hierarchy");
	ImRect windowRect = { ImGui::GetWindowContentRegionMin(), ImGui::GetWindowContentRegionMax() };

	if (myContext);
	{
		myContext->myRegistry.each([&](auto entityID)
			{
				Entity entity(entityID, myContext);
				if (entity.IsValid() &&
					(entt::entity)entity != (entt::entity)GraphicsEngine::Get().myCamera)
				{
					DrawEntityNode(entity);
				}
			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			mySelectionContext = {};

		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				myContext->CreateEntity("Empty Entity", myContext);

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (myContext->myRegistry.valid(mySelectionContext))
		{
			DrawComponents(mySelectionContext);
		}

		ImGui::End();
	}
}

void SceneHierarchyPanel::DrawEntityNode(Entity aEntity)
{
	auto tag = aEntity.GetComponent<TagComponent>().name;

	ImGuiTreeNodeFlags flags = ((mySelectionContext == aEntity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)aEntity, flags, tag.c_str());
	if (ImGui::IsItemClicked())
	{
		mySelectionContext = aEntity;
	}

	bool entityDeleted = false;
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Delete Entity"))
			entityDeleted = true;

		ImGui::EndPopup();
	}

	if (opened)
	{
		// Draw Child Nodes
		ImGui::TreePop();
	}

	if (entityDeleted)
	{
		myContext->DestroyEntity(aEntity);
		if (mySelectionContext == aEntity)
			mySelectionContext = {};
	}
}

static void DrawVec3Control(const std::string& label, Vector3f& values, float resetValue = 0.0f, float columnWidth = 100.0f)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
		values.x = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, 1.0f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
		values.y = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, 1.f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Z", buttonSize))
		values.z = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, 1.f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}

template<typename T, typename UIFunction>
static void DrawComponent(const std::string& name, Entity aEntity, UIFunction uiFunction)
{
	const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
	if (aEntity.HasComponent<T>())
	{
		auto& component = aEntity.GetComponent<T>();
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Separator();
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
		ImGui::PopStyleVar(
		);
		if (typeid(T) != typeid(TagComponent) && typeid(T) != typeid(TransformComponent))
		{
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("..", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}
		}

		bool removeComponent = false;
		if (ImGui::BeginPopup("ComponentSettings"))
		{
			if (ImGui::MenuItem("Remove component"))
				removeComponent = true;
			ImGui::EndPopup();
		}

		if (open)
		{
			uiFunction(component);
			ImGui::TreePop();
		}

		if (removeComponent)
			aEntity.RemoveComponent<T>();
	}
}

template<typename T>
void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName) {
	if (!mySelectionContext.HasComponent<T>())
	{
		if (ImGui::MenuItem(entryName.c_str()))
		{
			mySelectionContext.AddComponent<T>();
			ImGui::CloseCurrentPopup();
		}
	}
}

void SceneHierarchyPanel::DrawComponents(Entity aEntity)
{
	if (aEntity.HasComponent<TagComponent>())
	{
		auto& tag = aEntity.GetComponent<TagComponent>().name;

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		std::strncpy(buffer, tag.c_str(), sizeof(buffer));
		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
		{
			tag = std::string(buffer);
		}
	}

	ImGui::SameLine();
	ImGui::PushItemWidth(-1);

	if (ImGui::Button("Add Component"))
		ImGui::OpenPopup("AddComponent");

	if (ImGui::BeginPopup("AddComponent"))
	{
		DisplayAddComponentEntry<ModelComponent>("Model Component");
		//DisplayAddComponentEntry<CameraComponent>("Camera Component");
		DisplayAddComponentEntry<ParticleSystemComponent>("Particle System Component");
		DisplayAddComponentEntry<LightComponent>("Light Component");

		ImGui::EndPopup();
	}

	ImGui::PopItemWidth();

	DrawComponent<TransformComponent>("Transform", aEntity, [](auto& component)
		{
			DrawVec3Control("Position", component.position);
			Vector3f rotation = {
				CommonUtilities::Math<float>::Degrees(component.rotation.x),
				CommonUtilities::Math<float>::Degrees(component.rotation.y),
				CommonUtilities::Math<float>::Degrees(component.rotation.z)
			};
			DrawVec3Control("Rotation", rotation);
			component.rotation = {
				CommonUtilities::Math<float>::Radians(rotation.x),
				CommonUtilities::Math<float>::Radians(rotation.y),
				CommonUtilities::Math<float>::Radians(rotation.z)
			};
			DrawVec3Control("Scale", component.scale, 1.0f);
		});

	DrawComponent<CameraComponent>("Camera", aEntity, [](auto& component)
		{
			bool changesMade = false;
			float fov = component.camera.myFov;
			float nearPlane = component.camera.myNear;
			float farPlane = component.camera.myFar;

			if (ImGui::DragFloat("Fov", &fov, 1.f, 30.f, 120.0f, "%.2f"))
			{
				changesMade = true;
			}
			if (ImGui::DragFloat("Near", &nearPlane, 1.f, 0.1f, 100.0f, "%.2f"))
			{
				changesMade = true;
			}
			if (ImGui::DragFloat("Far", &farPlane, 1.f, 100.f, 10000.0f, "%.2f"))
			{
				changesMade = true;
			}

			if (changesMade)
			{
				component.camera.SetProjectionValues(fov, 16.f / 9.f, nearPlane, farPlane);
			}
		});

	DrawComponent<ModelComponent>("Model", aEntity, [](auto& component)
		{

		});

	DrawComponent<ParticleSystemComponent>("Particle System", aEntity, [](auto& component)
		{
			
		});

	DrawComponent<LightComponent>("Light", aEntity, [](auto& component)
		{
			DrawVec3Control("Color", component.light.ourlightBuffer.Color);
			ImGui::InputFloat("Intensity", &component.light.ourlightBuffer.Intensity);
			DrawVec3Control("Direction", component.light.ourlightBuffer.Direction);
			ImGui::InputFloat("Range", &component.light.ourlightBuffer.Range);
			DrawVec3Control("Position", component.light.ourlightBuffer.Position);
			ImGui::InputFloat("Attenuation", &component.light.ourlightBuffer.Attenuation);
			ImGui::InputFloat("SpotInnerRadius", &component.light.ourlightBuffer.SpotInnerRadius);
			ImGui::InputFloat("SpotOuterRadius", &component.light.ourlightBuffer.SpotOuterRadius);
			ImGui::InputInt("LightType", &component.light.ourlightBuffer.LightType);
		});
}
