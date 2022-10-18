#include "gepch.h"
#include "ContentBrowserPanel.h"
#include "Renderer/TextureAssetHandler.h"
#include <imgui/imgui.h>

	ContentBrowserPanel::ContentBrowserPanel()
		: myCurrentDirectory(gAssetPath)
	{
	}

	void ContentBrowserPanel::Init()
	{
		TextureAssetHandler::LoadTexture("Resources/Editor/Icons/ContentBrowser/DirectoryIcon.dds");
		TextureAssetHandler::LoadTexture("Resources/Editor/Icons/ContentBrowser/FileIcon.dds");
		myDirectoryIcon = TextureAssetHandler::GetTexture("Resources/Editor/Icons/ContentBrowser/DirectoryIcon.dds");
		myFileIcon = TextureAssetHandler::GetTexture("Resources/Editor/Icons/ContentBrowser/FileIcon.dds");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (myCurrentDirectory != std::filesystem::path(gAssetPath))
		{
			if (ImGui::Button("<-"))
			{
				myCurrentDirectory = myCurrentDirectory.parent_path();
			}
		}

		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(myCurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			std::string filenameString = path.filename().string();

			ImGui::PushID(filenameString.c_str());
			Ref<Texture> icon = directoryEntry.is_directory() ? myDirectoryIcon : myFileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton(icon->mySRV.Get(), { thumbnailSize, thumbnailSize }, { 1, 0 }, { 0, 1 });

			if (ImGui::BeginDragDropSource())
			{
				auto relativePath = std::filesystem::relative(path, gAssetPath);
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
					myCurrentDirectory /= path.filename();

			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);

		ImGui::End();
	}
