#pragma once
#include "imgui.h"

#include "Logger.h"
#include "Level.h"
#include "Sector.h"

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include <memory>
#include <string>


#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/constants.hpp>
#include <gtc/type_ptr.hpp>

struct Wall2D {
	glm::vec2 normal;
	glm::vec2 start;
	glm::vec2 end;
};


class LvlEditor {
public:
	LvlEditor();

	void render();
	ImVec2 toImVec2(const glm::vec2& vec) { return ImVec2(vec.x, vec.y); };
	glm::vec2 toglmvec2(const ImVec2& vec) { return glm::vec2(vec.x, vec.y); };


	ImVec2 SnapToGrid(const ImVec2& point, float gridSpacing) {
		ImVec2 snapped;
		snapped.x = round((point.x - panOffset.x) / gridSpacing) * gridSpacing + panOffset.x;
		snapped.y = round((point.y - panOffset.y) / gridSpacing) * gridSpacing + panOffset.y;
		return snapped;
	}

	Level get_Level() { return lvl; }

	glm::vec2 plr_pos = glm::vec2(2.0f, 2.0f);

private:
	int worldScale;
	int windowUnitSize;

	ImVec2 panOffset = ImVec2(0, 0);
	bool panning = false;
	ImVec2 panStart;

	bool placingLine = false;

	Level lvl;

	std::vector<Wall2D> walls;
	int wallp1x, wallp1y;
	int wallp2x, wallp2y;
};