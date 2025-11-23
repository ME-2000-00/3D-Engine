



#include "LvlEditor.h"

bool pointInPolygon(const glm::vec2& p, const std::vector<Wall2D>& polygon) {
	bool inside = false;
	size_t n = polygon.size();
	for (size_t i = 0, j = n - 1; i < n; j = i++) {
		glm::vec2 a = polygon[i].start;
		glm::vec2 b = polygon[i].end;

		if (a.y == b.y) continue; // skip horizontal edges to avoid divide by zero

		bool intersect = ((a.y > p.y) != (b.y > p.y)) &&
			(p.x < (b.x - a.x) * (p.y - a.y) / (b.y - a.y) + a.x);

		if (intersect) inside = !inside;
	}
	return inside;
}


ImVec2 LvlEditor::SnapToGrid(const ImVec2& point, float gridSpacing)
{
	ImVec2 snapped;
	snapped.x = round((point.x - panOffset.x) / gridSpacing) * gridSpacing + panOffset.x;
	snapped.y = round((point.y - panOffset.y) / gridSpacing) * gridSpacing + panOffset.y;
	return snapped;
}
Wall make_wall(Wall2D& wall2d, int& unit_scale) {
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> dist(0.0f, 1.0f);


	Wall wall;
	wall.start = glm::vec2(wall2d.start.x / unit_scale, wall2d.start.y / unit_scale);
	wall.end = glm::vec2(wall2d.end.x / unit_scale, wall2d.end.y / unit_scale);
	Sector::compue_and_set_normal(wall);

	wall.portal = wall2d.portal;

	wall.r = dist(rng);
	wall.g = dist(rng);
	wall.b = dist(rng);

	return wall;
}

LvlEditor::LvlEditor()
{
	world_scale = 100;
	unit_scale = 40;
}

void renderScreenGrid(int windowWidth, int windowHeight, int world_scale = 100, int unit_scale = 40)
{
	// Push 2D projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, windowWidth, 0, windowHeight, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	float half_world_px = (world_scale * unit_scale) * 0.5f;

	// Minor lines
	glLineWidth(1.0f);
	glColor3f(0.2f, 0.2f, 0.2f);
	glBegin(GL_LINES);
	for (int i = -world_scale / 2; i <= world_scale / 2; i++) {
		if (i % 5 != 0) {
			float x = windowWidth / 2 + i * unit_scale;
			float y = windowHeight / 2 + i * unit_scale;

			// vertical
			glVertex2f(x, windowHeight / 2 - half_world_px);
			glVertex2f(x, windowHeight / 2 + half_world_px);

			// horizontal
			glVertex2f(windowWidth / 2 - half_world_px, x);
			glVertex2f(windowWidth / 2 + half_world_px, x);
		}
	}
	glEnd();

	// Major lines
	glLineWidth(3.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
	for (int i = -world_scale / 2; i <= world_scale / 2; i += 5) {
		float x = windowWidth / 2 + i * unit_scale;
		float y = windowHeight / 2 + i * unit_scale;

		// vertical
		glVertex2f(x, windowHeight / 2 - half_world_px);
		glVertex2f(x, windowHeight / 2 + half_world_px);

		// horizontal
		glVertex2f(windowWidth / 2 - half_world_px, x);
		glVertex2f(windowWidth / 2 + half_world_px, x);
	}
	glEnd();

	// Restore 3D matrices
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

std::vector<Sector> get_sectors(std::vector<Wall2D>& walls2D, int unit_scale) {

	std::vector<Sector> sectors;
	Sector sector;
	Wall first;
	bool start_wall = true;



	for (Wall2D wall2D : walls2D) {
		Wall wall = make_wall(wall2D, unit_scale);
		sector.walls.push_back(wall);
		sector.walls2D.push_back(wall2D);

		if (start_wall) {
			first = wall;
			start_wall = false;
		}

		if (!start_wall && first.start == wall.end) {
			sectors.push_back(sector);
			sector.walls.clear();
			sector.walls2D.clear();

			start_wall = true;
		}
	}

	return sectors;
}

void LvlEditor::sellect_sector() {
	// for hovering a sector
	for (Sector& sector : lvlSectors) {
		bool hover = pointInPolygon(intersection, sector.walls2D);

		if (hover) {
			sector.selected = true;
			ceeling = sector.ceeling;
			floor = sector.floor;
		}
		else if (!hover) {
			sector.selected = false; // optionally deselect if not hovering
		}
	}


}








void LvlEditor::openglRender()
{
	point = toglmvec2(SnapToGrid(ImVec2(intersection.x, intersection.y), unit_scale));
	glDisable(GL_DEPTH_TEST);
	// make the editor 3D xP
	
	// world grid
	renderScreenGrid(window_width, window_height, world_scale, unit_scale);


	for (Wall2D &wall : walls) {
		glColor3f(0.2f, 0.2f, 0.8f);
		glLineWidth(3.0f);

		glBegin(GL_LINES);
			glVertex2f(wall.start.x, wall.start.y);
			glVertex2f(wall.end.x, wall.end.y);
		glEnd();



		glm::vec2 center = (wall.start + wall.end) * 0.5f;
		float normalScale = 10.0f; // adjust for visibility

		glColor3f(0.2f, 0.8f, 0.2f);
		glLineWidth(2.0f);

		glBegin(GL_LINES);
		glVertex2f(center.x, center.y);
		glVertex2f(center.x + wall.normal.x * normalScale, center.y + wall.normal.y * normalScale);
		glEnd();

	}

	if (ghost_wall) {
		glColor3f(0.2f, 0.2f, 0.8f);
		glLineWidth(2.0f);

		glBegin(GL_LINES);
			glVertex2f(current_wall.start.x, current_wall.start.y);
			glVertex2f(point.x, point.y);
		glEnd();

		glm::vec2 e1 = point - current_wall.start; // vector along the wall

		if (glm::length(e1) < 1e-6f) {
			current_wall.normal = glm::vec2(0.0f, 0.0f); // degenerate wall
			return;
		}

		glm::vec2 normal(-e1.y, e1.x); // 90° CCW
		glm::vec2 normal2 = glm::normalize(normal);


		glm::vec2 center = (current_wall.start + point) * 0.5f;
		float normalScale = 10.0f; // adjust for visibility

		glColor3f(0.2f, 0.8f, 0.2f);
		glLineWidth(2.0f);

		glBegin(GL_LINES);
		glVertex2f(center.x, center.y);
		glVertex2f(center.x + normal2.x * normalScale, center.y + normal2.y * normalScale);
		glEnd();
	}

	// for hovering a sector
	for (Sector sector : lvlSectors) {
		bool hover = pointInPolygon(intersection, sector.walls2D);
		
		if ((hover && lvlSectors.size() > 0) || sector.selected) {

			std::vector<glm::vec2> vertices;
			for (Wall2D wall : sector.walls2D)
				vertices.push_back(wall.start);

			std::vector<Tri> tris = triangulate(vertices);

			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDepthMask(GL_FALSE);
			glFrontFace(GL_CW);

			glColor4f(0.2f, 0.8f, 1.0f, 0.4f);
			glBegin(GL_TRIANGLES);
			for (const auto& t : tris) {
				glVertex2f(t.a.x, t.a.y);
				glVertex2f(t.b.x, t.b.y);
				glVertex2f(t.c.x, t.c.y);
			}
			glEnd();

			glDepthMask(GL_TRUE);
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
			glFrontFace(GL_CCW);

		}

	}


	// mouse pos
	glPointSize(10);           // set pixel size of the dot
	glColor3f(1.0f, 1.0f, 0.0f);


	glBegin(GL_POINTS);
		glVertex2f(plr_pos.x, plr_pos.y);
	glEnd();
	glEnable(GL_DEPTH_TEST);
}

void LvlEditor::define_wall_start()
{
	// defines start point of a new wall
	current_wall.start = glm::vec2(point.x, point.y);
	ghost_wall = true;
}

void LvlEditor::ImGuirender() {
	ImGui::Begin("Edit Values");

	ImGui::InputFloat("Floor", &floor);
	ImGui::InputFloat("Ceeling", &ceeling);

	if (ImGui::Button("Set new Sector floor and ceiling")) {
		for (Sector& sector : lvlSectors) { // note the '&' here
			if (sector.selected) {
				sector.set_ceeling(ceeling);
				sector.set_floor(floor);
			}
		}
	}

	ImGui::End();

}




void LvlEditor::define_wall_end()
{
	// defines end point of a new wall
	current_wall.end = glm::vec2(point.x, point.y);
	Wall wall3D = make_wall(current_wall, unit_scale);
	Sector::compue_and_set_normal(wall3D);

	current_wall.normal = wall3D.normal;
	ghost_wall = false;

	walls.push_back(current_wall);

	// update sectors
	lvlSectors = get_sectors(walls, unit_scale);
}



void LvlEditor::render()
{
	openglRender();
}





Level LvlEditor::get_Level() 
{
	lvl.sectors.clear();

	for (Sector sector : lvlSectors) {
		lvl.sectors.push_back(sector);
	}

	return lvl;
}