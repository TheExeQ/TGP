#include "gepch.h"
#include "SceneViewportPanel.h"

/*

void SceneViewportPanel::Init()
{
	if (!myInstace)
	{
		myInstace = this;
	}
	else
	{
		return;
	}

}

void SceneViewportPanel::OnImGuiRender(Entity aSelectedEntity)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
	ImGui::Begin("Viewport", &myOpened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

	EditorCamera::myViewportWindowSelected = ImGui::IsWindowHovered();

	auto viewportPanelSize = ImGui::GetContentRegionAvail();
	if (myViewportSize != *(glm::vec2*)&viewportPanelSize)
	{
		myViewportSize = glm::vec2(viewportPanelSize.x, viewportPanelSize.y);
		OnResize();
	}
	ImGui::Image(DX11::Get().myEditorShaderResourceView.Get(), ImVec2(myViewportSize.x, myViewportSize.y));

	// Gizmos
	if (aSelectedEntity.IsValid() && myShowGizmos)
	{
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();
		auto pos = ImGui::GetWindowPos();
		auto size = ImGui::GetWindowSize();
		ImGuizmo::SetRect(pos.x, pos.y, size.x, size.y);

		auto camEntity = Engine::GetEditorScene()->GetPrimaryCameraEntity();
		if (camEntity.IsValid())
		{
			auto& camera = camEntity.GetComponent<CameraComponent>().camera;
			const auto& tranform = camEntity.GetComponent<TransformComponent>();
			glm::mat4 projectionMat = camera.GetProjectionMatrix();
			glm::mat4 viewMat = camera.GetViewMatrix(tranform.position, tranform.rotation);

			auto& comp = aSelectedEntity.GetComponent<TransformComponent>();
			auto oldRot = comp.rotation;
			glm::mat4 entityTransform = aSelectedEntity.GetWorldTransform();

			ImGuizmo::Manipulate(glm::value_ptr(viewMat), glm::value_ptr(projectionMat), myGizmoOperation, myGizmoMode, glm::value_ptr(entityTransform));

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 newPos;
				glm::vec3 newRot;
				glm::vec3 newScale;

				if (aSelectedEntity.HasParent())
				{
					Entity parent(aSelectedEntity.ParentUUID());
					glm::mat4 parentTransform = Engine::GetActiveScene()->GetWorldSpaceTransformMatrix(parent);
					entityTransform = glm::inverse(parentTransform) * entityTransform;
				}

				Snow::Math::DecomposeTransform(entityTransform, newPos, newRot, newScale);

				glm::vec3 deltaRot = newRot - oldRot;

				comp.position = newPos;
				comp.rotation += deltaRot;
				comp.scale = newScale;
			}
		}
	}

	ImGui::End();
	ImGui::PopStyleVar();
}

void SceneViewportPanel::OnResize()
{
	DX11::Get().myEditorTexture->Release();
	DX11::Get().myEditorRenderTargetView->Release();
	DX11::Get().myEditorShaderResourceView->Release();

	DX11::Get().myEditorDepthStencilTexture->Release();
	DX11::Get().myEditorDepthStencilState->Release();
	DX11::Get().myEditorDepthStencilView->Release();

	DX11::Get().CreateEditorRenderTargetView(myViewportSize.x, myViewportSize.y);
	DX11::Get().CreateEditorDepthStencil(myViewportSize.x, myViewportSize.y);
}

*/
