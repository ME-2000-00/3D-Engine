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
#include "util.h"
#include <array>


class Sector {
public:
	void draw();
	void rebuild();

	bool selected = false;

	// debugging
	Sector();

	float floor = -1.0f;
	float ceiling = 2.0f;



	std::vector<glm::vec2> points;

	// ids of points /\ wich are portals
	std::vector<std::array<int,2>> portal_ids;
    std::vector<UTIL::Tri> triangles;
};


// generates sectors from walls
inline std::vector<Sector> get_sectors(std::vector<UTIL::polygonWall>& walls, int unit_scale = 40) {

    std::vector<Sector> sectors;
    Sector sector;
    bool start_wall = true;
    UTIL::polygonWall first;



    for (UTIL::polygonWall& wall : walls) {
        sector.points.push_back(glm::vec2(wall.start.x / unit_scale, wall.start.y / unit_scale));

        if (wall.is_portal) {
            // adds ids of portals to the sector
            // first id is alright
            // second one needs to be 0 if its the last wall of the sector
            // so using wall.size() doesnt work since this array saves all the walls not just the walls per sector
            //    example:
            // wall size: 7  sector1 size: 3 sector2 size: 4

            // TODO: needs to be fixed
            std::array<int, 2> ids = { sector.points.size() - 1, !start_wall && first.start == wall.end ? 0 : sector.points.size() };

            sector.portal_ids.push_back(ids);
        }





        if (start_wall) {
            first = wall;
            start_wall = false;
        }

        if (!start_wall && first.start == wall.end) {
            // triangulates the polygon for floor and ceiling
            sector.rebuild();

            sectors.push_back(sector);
            sector.points.clear();

            start_wall = true;
        }
    }

    return sectors;
}