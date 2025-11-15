#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "Sector.h"
#include "Logger.h"

using sectors_t = std::vector<Sector>;



class Level {
public:
	Level();
	sectors_t sectors;
	glm::vec3 spawn_point = glm::vec3(0.0f, 0.0f, 0.0f);

	void render();
	void update();
};