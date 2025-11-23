#pragma once
#include "imgui.h"

#include "Logger.h"
#include "Level.h"
#include "Sector.h"
#include <GLFW/glfw3.h>
#include "triangulate.h"

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
	int world_scale;
	int unit_scale;
	int window_width, window_height;

	void openglRender();
	void render();
	void update(glm::vec3 cam_pos, glm::vec3 cam_dir);
	void ImGuirender();

	void define_wall_start();
	void define_wall_end();
	
	void sellect_sector();

	ImVec2 toImVec2(const glm::vec2& vec) { return ImVec2(vec.x, vec.y); };
	glm::vec2 toglmvec2(const ImVec2& vec) { return glm::vec2(vec.x, vec.y); };
	Level get_Level();


	ImVec2 SnapToGrid(const ImVec2& point, float gridSpacing);
	std::vector<Sector> lvlSectors;

	glm::vec2 intersection;
	glm::vec2 plr_pos;
	glm::vec2 point;

	float ceeling, floor;

private:

	glm::vec2 panOffset = glm::vec2(0.0f, 0.0f);
	Level lvl;
	Wall2D current_wall;

	std::vector<Wall2D> walls;
	bool ghost_wall = false;

};