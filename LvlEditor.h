#pragma once
#include "imgui.h"

#include "Logger.h"
#include "Level.h"
#include "Sector.h"
#include "util.h"

#include <GLFW/glfw3.h>

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

class LvlEditor {
public:
	LvlEditor();
	int window_width, window_height;

	glm::vec2 panOffset = glm::vec2(0.0f, 0.0f);
	glm::vec2 panStartPoint = glm::vec2(0.0f, 0.0f);
	bool start_panning = false;

	void openglRender();
	void render();
	void ImGuirender();
	void renderSectors();
	void MouseFunctionalityUtil(const int MouseState);

	ImVec2 toImVec2(const glm::vec2& vec) { return ImVec2(vec.x, vec.y); };
	glm::vec2 toglmvec2(const ImVec2& vec) { return glm::vec2(vec.x, vec.y); };
	Level get_Level();


	std::vector<Sector> lvlSectors;
	std::vector<UTIL::polygonWall> walls;

	glm::vec2 mouse;
	glm::vec2 plr_pos;

	float ceeling, floor;

private:

	Level lvl;

};



// updates all the sectors
inline void update_sectors(std::vector<Sector>& sectors) {
	for (Sector& sector : sectors) {
		sector.rebuild();
	}
}