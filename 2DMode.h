#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Level.h";
#include "Sector.h";


class Mode2D{
public:

	void render2D();
	Level& get_Level() { return lvl; };
	glm::vec3 get_SpawnPoint() { return glm::vec3(spawnpoint.x / 40.0f, 0 ,spawnpoint.y / 40.0f); };
private:
	Level lvl;
	ImVec2 spawnpoint = ImVec2(0.0f, 0.0f);

};