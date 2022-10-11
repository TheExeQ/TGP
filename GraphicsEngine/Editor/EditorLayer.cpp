#include "EditorLayer.h"

#include "Core/GraphicsEngine.h"
#include "Scene/SceneSerializer.h"

#include <ImGui/imgui.h>

const char* g_ImGuiWindowNames[ImGuiWindows::COUNT] =
{
	"Hierarchy/Properties",
	"ContentBrowser",
	"Settings"
};

void EditorLayer::Init()
{
	myContentBrowserPanel.Init();
}

void EditorLayer::OnRender()
{
	Dockspace();
	WindowController();
	if (myWindowEnabler[ImGuiWindows::SceneHierarchy])
		mySceneHierarchyPanel.OnImGuiRender();
	if (myWindowEnabler[ImGuiWindows::ContentBrowser])
		myContentBrowserPanel.OnImGuiRender();
	if (myWindowEnabler[ImGuiWindows::Settings])
		mySettingsPanel.OnImGuiRender();
}

void EditorLayer::WindowController()
{
	ImGui::Begin("Windows");
	for (int i = 0; i < ImGuiWindows::COUNT; i++)
	{
		ImGui::Checkbox(g_ImGuiWindowNames[i], &myWindowEnabler[i]);
	}
	ImGui::End();
}

void EditorLayer::Dockspace()
{
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", nullptr, window_flags);
	ImGui::PopStyleVar();
	ImGui::PopStyleVar(2);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			auto originalPath = std::filesystem::current_path();

			OPENFILENAME ofn;
			wchar_t fileName[MAX_PATH] = L"";
			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = GraphicsEngine::Get().GetWindowHandle();
			ofn.lpstrFilter = L"Scene (*.scene)\0*.scene\0";
			ofn.lpstrFile = fileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			ofn.lpstrDefExt = L"txt";

			std::wstring fileNameStr;

			if (ImGui::MenuItem("Open...", NULL))
			{
				if (GetOpenFileName(&ofn))
				{
					fileNameStr = fileName;
					SceneSerializer serializer(Scene::GetActiveScene());
					serializer.Deserialize(std::string(fileNameStr.begin(), fileNameStr.end()).c_str());
					std::filesystem::current_path(originalPath);

					auto activeScene = Scene::GetActiveScene();
					Entity camera;

					activeScene->ForEach([&](entt::entity aEnt)
						{
							Entity ent(aEnt, activeScene);
							if (ent.HasComponent<CameraComponent>() && ent.GetComponent<TagComponent>().name == "EditorCamera")
							{
								camera = ent;
							}
						});

					if (camera.IsValid())
					{
						activeScene->SetMainCamera(camera);
					}
					else
					{
						auto newCam = activeScene->CreateEntity("EditorCamera", activeScene);
						auto& comp = newCam.AddComponent<CameraComponent>();
						comp.camera.SetProjectionValues(90, 9.f / 16.f, 0.1f, 10000.0f);
						activeScene->SetMainCamera(newCam);
					}
				}
			};
			if (ImGui::MenuItem("Save As...", NULL))
			{
				if (GetSaveFileName(&ofn))
				{
					fileNameStr = fileName;
					SceneSerializer serializer(Scene::GetActiveScene());
					serializer.Serialize(std::string(fileNameStr.begin(), fileNameStr.end()).c_str());
					std::filesystem::current_path(originalPath);
				}
			};
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	ImGui::End();
}
