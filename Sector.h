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
//#include <GL/GLU.h>

#include "Logger.h"

struct Wall {
	glm::vec2 start; // start point of the wall
	glm::vec2 end;   // end point of the wall
	glm::vec2 normal;
	float r, g, b;

	bool portal = false;
};

struct Wall2D {
	glm::vec2 normal;
	glm::vec2 start;
	glm::vec2 end;
	bool portal = false;
};


class Sector {
public:
	void draw();
	void set_floor(int v) { floor = v; };
	void set_ceeling(int v) { ceeling = v; };

	static void compue_and_set_normal(Wall& wall);
	std::vector<Wall> walls;
	std::vector<Wall2D> walls2D;

	bool selected = false;

	// debugging
	Sector();

	float floor = -1.0f;
	float ceeling = 2.0f;

private:

};