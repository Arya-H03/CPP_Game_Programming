#include "GUISystem.h"

void GUISystem::HandleGUI()
{
	ImGui::Begin("Shape Wars");
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("Game"))
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
							//Delete btn
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
							//ID
							ImGui::Text("%i", e->Id());
							ImGui::SameLine();
							//Position
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
					//Delete btn
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
					//ID
					ImGui::Text("%i", e->Id());
					ImGui::SameLine();
					//Tag
					ImGui::Text("%s", e->Tag().c_str());
					ImGui::SameLine();
					//Position
					ImGui::Text("(%.2f,%.2f)", e->Get<CTransform>().pos.x, e->Get<CTransform>().pos.y);

					if (isDestroyed) e->Destroy();
				}
			}
			ImGui::EndTabItem();
		}
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
		ImGui::EndTabBar();
	}
	ImGui::End();
}

void GUISystem::ResetPlayer(Entity* player)
{
	playerID = playerID;
}
