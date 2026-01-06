#include "GUISystem.h"
#include "TypeUtils.hpp"

void GUISystem::HandleGUI()
{
	ImGui::Begin("Shape Wars");
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
	{
		/*if (ImGui::BeginTabItem("Game"))
		{

			ImGui::Text("%s%i", "Score:", score);
			int clicked = 0;
			if (ImGui::Button("Replay")) clicked++;

			Entity* player = m_entityManager.FindEntityByID(playerID);

			if (clicked & 1 && player)
			{
				player->Destroy();
				onResetGameBtnClick.Invoke();
				clicked = 0;
			}
			else if (clicked & 1 && !player)
			{
				onResetGameBtnClick.Invoke();
				clicked = 0;
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Entities"))
		{
			int btnId = 0;
			if (ImGui::CollapsingHeader("Entities by Tags"))
			{
				for (auto& [tag, entityVec] : m_entityManager.GetEntityMap())
				{
					std::string headerName = tag;
					if (ImGui::CollapsingHeader(headerName.c_str()))
					{
						bool isDestroyed = false;
						for (auto& e : entityVec)
						{
							Delete btn
							static int clicked = 0;
							isDestroyed = false;
							sf::Color shapeColor = e->Get<CShape>().circle.getFillColor();
							ImGui::PushID(btnId);
							btnId++;
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(static_cast<float>(shapeColor.r) / 255, static_cast<float>(shapeColor.g) / 255, static_cast<float>(shapeColor.b) / 255, 1.0f));
							if (ImGui::Button("D")) isDestroyed = true;
							ImGui::PopStyleColor();
							ImGui::PopID();
							ImGui::SameLine();
							ID
							ImGui::Text("%i", e->Id());
							ImGui::SameLine();
							Position
							ImGui::Text("(%.2f,%.2f)", e->Get<CTransform>().pos.x, e->Get<CTransform>().pos.y);

							if (isDestroyed) e->Destroy();
						}
					}
				}
			}

			if (ImGui::CollapsingHeader("All Entities"))
			{
				bool isDestroyed = false;
				for (auto& e : m_entityManager.GetEntities())
				{
					Delete btn
					static int clicked = 0;
					isDestroyed = false;
					sf::Color shapeColor = e->Get<CShape>().circle.getFillColor();
					ImGui::PushID(btnId);
					btnId++;
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(static_cast<float>(shapeColor.r) / 255, static_cast<float>(shapeColor.g) / 255, static_cast<float>(shapeColor.b) / 255, 1.0f));
					if (ImGui::Button("D")) isDestroyed = true;


					ImGui::PopStyleColor();
					ImGui::PopID();
					ImGui::SameLine();
					ID
					ImGui::Text("%i", e->Id());
					ImGui::SameLine();
					Tag
					ImGui::Text("%s", e->Tag().c_str());
					ImGui::SameLine();
					Position
					ImGui::Text("(%.2f,%.2f)", e->Get<CTransform>().pos.x, e->Get<CTransform>().pos.y);

					if (isDestroyed) e->Destroy();
				}
			}
			ImGui::EndTabItem();
		}




		//For Cells
		if (ImGui::BeginTabItem("Debug"))
		{
			int clicked = 0;
			if (ImGui::Button("Visualize Cells")) clicked++;

			if (clicked& 1)
			{
				if (m_gridSystem.GetRenderCells())m_gridSystem.SetRenderCells(false);
				else m_gridSystem.SetRenderCells(true);
				clicked = 0;
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Units"))
		{
			int clicked = 0;
			if (ImGui::Button("Footman")) clicked++;

			if (clicked & 1)
			{
				clicked = 0;
			}
			ImGui::EndTabItem();
		}*/
		
		DrawEntitiesTab();
		DrawComponentsTab();
		DrawDebugTab();

		ImGui::EndTabBar();
	}
	ImGui::End();
}

void GUISystem::DrawEntitiesTab()
{
	if (ImGui::BeginTabItem("Entities"))
	{
		for (auto& [tag, entityVector] : m_entityManager.GetEntityMap())
		{
			std::string headerName = tag;
			if (ImGui::CollapsingHeader(headerName.c_str()))
			{
				bool isDestroyed = false;
				for (auto& entity : entityVector)
				{
					std::ostringstream oss;
					oss << &entity;
					ImGui::TextColored(idColor, "%i", entity.Id());
					ImGui::SameLine();
					ImGui::Text("%s", "|");
					ImGui::SameLine();
					ImGui::TextColored(addressColor, "%s", oss.str().c_str());
				}
			}
		}
		ImGui::EndTabItem();
	}
}

void GUISystem::DrawComponentsTab()
{
	if (ImGui::BeginTabItem("Components"))
	{
		std::apply([](auto&... componentVector)
			{
				(([&]()
					{
						if (ImGui::CollapsingHeader(GetTypeName<decltype(componentVector[0])>()))
						{
							for (size_t i = 0; i < componentVector.size(); ++i)
							{
								if (!componentVector[i].exists) continue;

								ImGui::TextColored(idColor, "%i", i);
								ImGui::SameLine();
								ImGui::Text("%s %s", GetTypeName<decltype(componentVector[i])>(), "|");
								ImGui::SameLine();
								ImGui::TextColored(sizeColor, "%i", GetTypeSize<decltype(componentVector[i])>());
								ImGui::SameLine();
								ImGui::Text("%s", "Bytes");
								ImGui::SameLine();
								ImGui::Text("%s", "|");
								ImGui::SameLine();
								std::ostringstream oss;
								oss << &componentVector[i];
								ImGui::TextColored(addressColor, "%s", oss.str().c_str());
							}
						}

					}()
						), ...);

			}, EntityMemoryPool::Instance().GetComponentPool());


		ImGui::EndTabItem();
	}
}

void GUISystem::DrawDebugTab()
{
	if (ImGui::BeginTabItem("Debug"))
	{
		int clicked = 0;
		if (ImGui::Button("Visualize Cells")) clicked++;

		if (clicked & 1)
		{
			m_gridSystem.ToggleCellsToRender();
			clicked = 0;
		}

		ImGui::EndTabItem();
	}
}

