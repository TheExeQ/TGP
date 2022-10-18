#include "gepch.h"
#include "SceneHierarchyPanel.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <string>
#include <filesystem>

#include "Core/GraphicsEngine.h"
#include "Scene/Components.h"
#include "Math/Vector.hpp"
#include "Math/Matrix.hpp"
#include "Math/Mathf.hpp"

#include "Renderer/ModelAssetHandler.h"
#include "Renderer/ModelInstance.h"
#include "Renderer/TextureAssetHandler.h"
#include "Renderer/Material.h"

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
				if (entity.IsValid() && !entity.HasParent() &&
					(entt::entity)entity != (entt::entity)GraphicsEngine::Get().myCamera)
				{
					DrawEntityNode(entity);
				}
			});

		if (ImGui::IsMouseDown(0) && !ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsWindowHovered() && !ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
			mySelectionContext.clear();

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				myContext->CreateEntity("Empty Entity", myContext);

			ImGui::EndPopup();
		}

		if (ImGui::BeginDragDropTargetCustom(windowRect, ImGui::GetCurrentWindow()->ID))
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("scene_entity_hierarchy", ImGuiDragDropFlags_AcceptNoDrawDefaultRect);

			if (payload)
			{
				Entity& entity = *(Entity*)payload->Data;
				for (auto selectedEnt : myDragDropEntities)
				{
					myContext->UnparentEntity(selectedEnt);
				}
				myContext->UnparentEntity(entity);
				myDragDropEntities.clear();
			}

			ImGui::EndDragDropTarget();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (!mySelectionContext.empty() && myContext->myRegistry.valid(mySelectionContext[0]))
		{
			DrawComponents(mySelectionContext[0]);
		}

		ImGui::End();
	}
}

void SceneHierarchyPanel::DrawEntityNode(Entity aEntity)
{
	auto tag = aEntity.GetComponent<TagComponent>().name;

	ImGuiTreeNodeFlags flags = 0;

	for (const auto& selectedEnt : mySelectionContext)
	{
		if (selectedEnt == aEntity)
			flags = ImGuiTreeNodeFlags_Selected;
	}
	flags |= ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)aEntity, flags, tag.c_str());

	if (ImGui::IsItemClicked())
	{
		myDragDropEntities = mySelectionContext;
		if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
		{
			auto it = std::find(mySelectionContext.begin(), mySelectionContext.end(), aEntity);
			if (it == mySelectionContext.end())
			{
				mySelectionContext.push_back(aEntity);
			}
			else
			{
				mySelectionContext.erase(it);
			}
		}
		else
		{
			mySelectionContext.clear();
			mySelectionContext.push_back(aEntity);
		}
	}

	bool entityDeleted = false;
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Delete Entity"))
			entityDeleted = true;

		ImGui::EndPopup();
	}

	bool isDragDropping = false;

	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
	{
		ImGui::Text(aEntity.GetComponent<TagComponent>().name.c_str());
		ImGui::SetDragDropPayload("scene_entity_hierarchy", &aEntity, sizeof(Entity));
		ImGui::EndDragDropSource();
		isDragDropping = true;
	}

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("scene_entity_hierarchy", ImGuiDragDropFlags_AcceptNoDrawDefaultRect);

		if (payload)
		{
			Entity& droppedEntity = *(Entity*)payload->Data;
			for (auto selectedEnt : myDragDropEntities)
			{
				myContext->ParentEntity(selectedEnt, aEntity);
			}
			
			myContext->ParentEntity(droppedEntity, aEntity);
			myDragDropEntities.clear();
		}

		ImGui::EndDragDropTarget();
	}

	if (opened)
	{
		for (auto child : aEntity.ChildrenUUIDs())
		{
			Entity e = myContext->GetEntityFromUUID(child);
			if (e.IsValid())
			{
				DrawEntityNode(e);
			}
		}

		ImGui::TreePop();
	}

	if (entityDeleted)
	{
		myContext->DestroyEntity(aEntity);
		if (!mySelectionContext.empty() && mySelectionContext[0] == aEntity)
			mySelectionContext.clear();
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
	if (!mySelectionContext[0].HasComponent<T>())
	{
		if (ImGui::MenuItem(entryName.c_str()))
		{
			mySelectionContext[0].AddComponent<T>();
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
			std::vector<std::string> items;
			std::string path;

			//path = "../Assets/Models/SM";
			//for (const auto& entry : std::filesystem::directory_iterator(path))
			//{
			//	items.push_back(std::string("SM/") + entry.path().filename().string());
			//}

			//path = "../Assets/Models/SK";
			//for (const auto& entry : std::filesystem::directory_iterator(path))
			//{
			//	items.push_back(std::string("SK/") + entry.path().filename().string());
			//}

			//static std::string current_item;

			//if (ImGui::BeginCombo("Model", current_item.c_str()))
			//{
			//	for (int n = 0; n < items.size(); n++)
			//	{
			//		bool is_selected = (current_item == items[n]);
			//		if (ImGui::Selectable(items[n].c_str(), is_selected))
			//		{
			//			current_item = items[n];

			//			Ref<Model>& model = component.modelInstance.GetModel();
			//			ModelAssetHandler::LoadModel(current_item);
			//			model = ModelAssetHandler::GetModel(current_item);
			//		}

			//		if (is_selected)
			//		{
			//			ImGui::SetItemDefaultFocus();
			//		}
			//	}
			//	ImGui::EndCombo();
			//}

			//items.clear();

			path = "../Assets/Textures/";
			for (const auto& entry : std::filesystem::directory_iterator(path))
			{
				items.push_back(entry.path().filename().string());
			}

			static std::string current_item2;

			if (ImGui::BeginCombo("Texture", current_item2.c_str()))
			{
				for (int n = 0; n < items.size(); n++)
				{
					bool is_selected = (current_item2 == items[n]);
					if (ImGui::Selectable(items[n].c_str(), is_selected))
					{
						current_item2 = items[n];

						if (component.modelInstance.GetModel())
						{
							Ref<Material> material = component.modelInstance.GetModel()->GetModelData(0).myMaterial;
							TextureAssetHandler::LoadTexture(current_item2);
							material->SetAlbedoTexture(TextureAssetHandler::GetTexture(current_item2));
						}
					}

					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
		});

	DrawComponent<ParticleSystemComponent>("Particle System", aEntity, [](auto& component)
		{

		});

	DrawComponent<LightComponent>("Light", aEntity, [](auto& component)
		{
			DrawVec3Control("Color", component.light.ourLightBuffer.Color);
			ImGui::InputFloat("Intensity", &component.light.ourLightBuffer.Intensity);
			//DrawVec3Control("Direction", component.light.ourLightBuffer.Direction);
			ImGui::InputFloat("Range", &component.light.ourLightBuffer.Range);

			switch (component.light.ourLightBuffer.LightType)
			{
			case 1:
			{

				break;
			}

			case 2:
			{
				ImGui::InputFloat("SpotInnerRadius", &component.light.ourLightBuffer.SpotInnerRadius);
				ImGui::InputFloat("SpotOuterRadius", &component.light.ourLightBuffer.SpotOuterRadius);
				break;
			}

			default:
			{
				break;
			}
			}
			//ImGui::InputFloat("Attenuation", &component.light.ourLightBuffer.Attenuation);
			ImGui::InputInt("LightType", &component.light.ourLightBuffer.LightType);
		});
}
