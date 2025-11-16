#include "LvlEditor.h"

LvlEditor::LvlEditor()
{
	worldScale = 100;
	windowUnitSize = 40;

	wallp1x = 3;
	wallp1y = 3;

	Wall2D wall;
	wall.start = glm::vec2(1, 1);
	wall.end = glm::vec2(4, 4);
	
	walls.push_back(std::move(wall));
}





void LvlEditor::render()
{
	// begin window
	ImGui::Begin("Level Editor", nullptr, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			// for saving the level
			if (ImGui::MenuItem("Save")) {
				Sector sector;

				for (Wall2D &wall2d : walls) {

					Wall wall;
					wall.start = wall2d.start;
					wall.end = wall2d.end;
					sector.compue_and_set_normal(wall);

					sector.walls.push_back(std::move(wall));
				}

				lvl.sectors.push_back(std::move(sector));
				Logger::Log(LogLevel::INFO,  "Sectors count: " + std::to_string(lvl.sectors.size()));


			}

			
			// more opetions here
			// - New
			// - Load from path


			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}





	//input logic
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImVec2 origin = ImGui::GetCursorScreenPos();   // top-left of window content

	// mouse position relative to window
	ImVec2 mouse = ImGui::GetIO().MousePos;
	bool hovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

	// some simple panning logic
	if (hovered && ImGui::IsMouseDown(ImGuiMouseButton_Middle))
	{
		if (!panning) {
			panning = true;
			panStart = mouse;
		}

		ImVec2 delta = ImVec2(mouse.x - panStart.x, mouse.y - panStart.y);
		panOffset.x += delta.x;
		panOffset.y += delta.y;
		panStart = mouse; // update for next frame
	}
	else {
		panning = false;
	}

	// zooming could be added here with mouse wheel
	float wheel = ImGui::GetIO().MouseWheel;
	if (wheel != 0.0f) {
		ImVec2 origin = ImGui::GetCursorScreenPos();

		ImVec2 before = ImVec2(
			(mouse.x - origin.x - panOffset.x) / windowUnitSize,
			(mouse.y - origin.y - panOffset.y) / windowUnitSize
		);


		// apply zoom change
		windowUnitSize *= (1.0f + wheel * 0.1f);
		windowUnitSize = std::clamp(windowUnitSize, 10, 80);

		// mouse position after zoom (world space)
		ImVec2 after = ImVec2(
			(mouse.x - origin.x - panOffset.x) / windowUnitSize,
			(mouse.y - origin.y - panOffset.y) / windowUnitSize
		);

		// adjust panning so zoom centers on the mouse
		panOffset.x += (after.x - before.x) * windowUnitSize;
		panOffset.y += (after.y - before.y) * windowUnitSize;
	}


	// adding walls
	static bool placing_line = false;
	static ImVec2 line_start;

	// area outlines

	// left
	draw_list->AddLine(
		ImVec2(0 + origin.x + panOffset.x, 0							  + origin.y + panOffset.y), 
		ImVec2(0 + origin.x + panOffset.x, (worldScale * windowUnitSize) + origin.y + panOffset.y), 
		IM_COL32(200, 200, 200, 255), 2.0f);

	// right
	draw_list->AddLine(
		ImVec2((worldScale * windowUnitSize) + origin.x + panOffset.x, 0							 + origin.y + panOffset.y),
		ImVec2((worldScale * windowUnitSize) + origin.x + panOffset.x, (worldScale * windowUnitSize) + origin.y + panOffset.y),
		IM_COL32(200, 200, 200, 250), 2.0f);

	// top
	draw_list->AddLine(
		ImVec2(0							 + origin.x + panOffset.x, (worldScale * windowUnitSize) + origin.y + panOffset.y),
		ImVec2((worldScale * windowUnitSize) + origin.x + panOffset.x, (worldScale * windowUnitSize) + origin.y + panOffset.y),
		IM_COL32(200, 200, 200, 250), 2.0f);

	// bottom
	draw_list->AddLine(
		ImVec2(0							 + origin.x + panOffset.x, 0 + origin.y + panOffset.y),
		ImVec2((worldScale * windowUnitSize) + origin.x + panOffset.x, 0 + origin.y + panOffset.y),
		IM_COL32(200, 200, 200, 250), 2.0f);

	// grid points
	for (int x = 1; x <= worldScale - 1; x++) {
		for (int y = 1; y <= worldScale - 1; y++) {
			draw_list->AddCircleFilled(
				ImVec2((x * windowUnitSize) + panOffset.x + origin.x, (y * windowUnitSize) + panOffset.y + origin.y),
				2.0f,
				IM_COL32(200, 200, 200, 230),
				12
				);
		}
	}

	// draw walls
	for (Wall2D wall : walls) {
		draw_list->AddLine(
			ImVec2(wall.start.x * windowUnitSize + panOffset.x + origin.x, wall.start.y * windowUnitSize + panOffset.y + origin.y),
			ImVec2(wall.end.x * windowUnitSize + panOffset.x + origin.x, wall.end.y * windowUnitSize + panOffset.y + origin.y),
			IM_COL32(250, 100, 100, 255),
			3.0f
		);

		draw_list->AddCircleFilled(
			ImVec2((wall.start.x * windowUnitSize) + panOffset.x + origin.x, (wall.start.y * windowUnitSize) + panOffset.y + origin.y),
			4.0f,
			IM_COL32(100, 100, 250, 230),
			12
		);


		draw_list->AddCircleFilled(
			ImVec2((wall.end.x * windowUnitSize) + panOffset.x + origin.x, (wall.end.y * windowUnitSize) + panOffset.y + origin.y),
			4.0f,
			IM_COL32(100, 100, 250, 230),
			12
		);
	}


	draw_list->AddLine(
		ImVec2((wallp2x * windowUnitSize) + origin.x + panOffset.x, (wallp2y * windowUnitSize) + origin.y + panOffset.y),
		ImVec2((wallp1x * windowUnitSize) + origin.x + panOffset.x, (wallp1y * windowUnitSize) + origin.y + panOffset.y),
		IM_COL32(255, 100, 255, 100), 1.0f);


	//Logger::Log(LogLevel::INFO,  "Walls count: " + std::to_string(walls.size()));


	draw_list->AddCircleFilled(
		ImVec2((plr_pos.x * windowUnitSize) + panOffset.x + origin.x, (plr_pos.y* windowUnitSize) + panOffset.y + origin.y),
		6.0f,
		IM_COL32(250, 250, 100, 250),
		12
	);

	draw_list->AddCircleFilled(
		ImVec2((wallp2x* windowUnitSize) + panOffset.x + origin.x, (wallp2y* windowUnitSize) + panOffset.y + origin.y),
		4.0f,
		IM_COL32(255, 250, 200, 250),
		12
	);

	draw_list->AddCircleFilled(
		ImVec2((wallp1x* windowUnitSize) + panOffset.x + origin.x, (wallp1y* windowUnitSize) + panOffset.y + origin.y),
		4.0f,
		IM_COL32(255, 250, 200, 250),
		12
	);


	ImGui::End();
	ImGui::Begin("Level Editor  (Wall Options)");

	ImGui::InputInt("P1 X", &wallp2x);
	ImGui::InputInt("P1 Y", &wallp2y);
	ImGui::Separator();
	ImGui::InputInt("P2 X", &wallp1x);
	ImGui::InputInt("P2 Y", &wallp1y);
	ImGui::Separator();
	int px = static_cast<int>(plr_pos.x);
	int py = static_cast<int>(plr_pos.y);

	ImGui::InputInt("Plr X", &px);
	ImGui::InputInt("Plr Y", &py);

	plr_pos.x = static_cast<float>(px);
	plr_pos.y = static_cast<float>(py);


	if (ImGui::Button("Add Wall")) {
		Wall2D wall;
		wall.start = glm::vec2(wallp1x, wallp1y);
		wall.end = glm::vec2(wallp2x, wallp2y);

		walls.push_back(std::move(wall));
	}

	ImGui::End();
}
