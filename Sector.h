#pragma once
#include <iostream>
#include <random>
#include <vector>
#include <memory>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/constants.hpp>
#include <gtc/type_ptr.hpp>

#include <GL/glew.h>

#include "Logger.h"

struct Wall {
	glm::vec2 start; // start point of the wall
	glm::vec2 end;   // end point of the wall
	glm::vec2 normal;
	float r, g, b;
};


class Sector {
public:
	void draw();
	void set_floor(int v) { floor = v; };
	void set_ceeling(int v) { ceeling = v; };

	void compue_and_set_normal(Wall& wall);
	std::vector<Wall> walls;

	// debugging
	Sector();

private:
	int floor = -1;
	int ceeling = 2;
};