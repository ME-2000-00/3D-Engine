



#include "LvlEditor.h"


LvlEditor::LvlEditor()
{
	//Sector S;
	//S.points.push_back(glm::vec2(4.0f, 4.0f));
	//S.points.push_back(glm::vec2(8.0f, 4.0f));
	//S.points.push_back(glm::vec2(8.0f, 8.0f));
	//S.points.push_back(glm::vec2(4.0f, 8.0f));
	//S.rebuild();

	//S.portal_ids.push_back({ 0,1 });
	//S.portal_ids.push_back({ 2,3 });

	//lvl.sectors.push_back(std::move(S));



	//UTIL::polygonWall P;

	//P.start   = glm::vec2(4.0f * unit_scale, 4.0f * unit_scale);
	//P.end     = glm::vec2(8.0f * unit_scale, 4.0f * unit_scale);
	//P.normal  = UTIL::compue_normal(P.start, P.end      );
	//P.Inormal = UTIL::compue_normal(P.start, P.end, true);
	//P.center  = (P.start + P.end) * 0.5f;
	//P.ids_in_Sector = { 0, 1 };

	//walls.push_back(std::move(P));
}

void LvlEditor::renderSectors() {

	glLineWidth(3.0f);

	// walls themselves

	glBegin(GL_LINES);
	for (const UTIL::polygonWall& wall : walls) {
		if (wall.is_portal) {
			glColor3f(0.7f, 0.0f, 0.7f);
		}
		else {
			glColor3f(0.7f, 0.0f, 0.0f);
		}
		glVertex2f(wall.start.x, wall.start.y);
		glVertex2f(wall.end.x  , wall.end.y  );
	}
	glEnd();

	// normals (double if portal)

	for (const UTIL::polygonWall& wall : walls) {
		glColor3f(0.0f, 0.8f, 0.2f);
		glBegin(GL_LINES);

		float scale = 10.0f; // in pixels

		glVertex2f(wall.center.x                        , wall.center.y                        );
		glVertex2f(wall.center.x + wall.normal.x * scale, wall.center.y + wall.normal.y * scale);

		if (wall.is_portal) {
			glVertex2f(wall.center.x, wall.center.y);
			glVertex2f(wall.center.x + wall.Inormal.x * scale, wall.center.y + wall.Inormal.y * scale);
		}


		glEnd();


		glPointSize(7);
		glColor3f(0.0f, 0.2f, 0.8f);
		glBegin(GL_POINTS);

		glVertex2f(wall.start.x, wall.start.y);
		glVertex2f(wall.end.x  , wall.end.y  );

		glEnd();
	}

}





// TODO: finish rendering logic
// TODO: finish porting   logic
// TODO: finish lvl      editor
void LvlEditor::openglRender()
{
	glPointSize(3);
	glColor3f(0.2, 0.2, 0.2);
	glBegin(GL_POINTS);

	//Logger::Log(LogLevel::INFO, std::to_string(panOffset.x));
	//Logger::Log(LogLevel::INFO, std::to_string(panOffset.y));

	for (int x = 0; x < WORLD::world_scale; x++) {
		for (int y = 0; y < WORLD::world_scale; y++) {
			glVertex2f(x * WORLD::unit_scale + panOffset.x, y * WORLD::unit_scale + panOffset.y);
		}
	}

	glEnd();

	// render walls
	renderSectors();


	if (LVLEDITOR::draw_portal_selection_line) {
		glLineWidth(1.0f);
		glColor3f(0.7f, 0.0f, 0.7f);
		glBegin(GL_LINES);
		glVertex2f(LVLEDITOR::last_mouse_pos.x, LVLEDITOR::last_mouse_pos.y);
		glVertex2f(mouse.x, mouse.y);
		glEnd();
		glLineWidth(3.0f);


		for (UTIL::polygonWall& wall : walls) {
			glm::vec2 point;
			bool intersection = UTIL::lineLineIntersection(LVLEDITOR::last_mouse_pos, mouse, wall.start, wall.end, point);

			if (intersection) {
				glColor4f(0.7f, 0.0f, 0.7f, 0.6f);
				glBegin(GL_LINES);
				glVertex2f(wall.start.x, wall.start.y);
				glVertex2f(wall.end.x, wall.end.y);
				glEnd();

				if (LVLEDITOR::make_portals && LVLEDITOR::draw_portal_selection_line) {
					wall.is_portal = true;
				}
			}
		}
	}

	// render hovered sector


	// for hovering a sector
	for (Sector& sector : lvlSectors) {
		LVLEDITOR::hovering_sector = UTIL::pointInPolygon(mouse, sector.points);

		if ((LVLEDITOR::hovering_sector && lvlSectors.size() > 0) || sector.selected) {

			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDepthMask(GL_FALSE);

			glColor4f(0.2f, 0.8f, 1.0f, 0.4f);
			glBegin(GL_TRIANGLES);
			for (const auto& t : sector.triangles) {
				glVertex2f(t.a.x * WORLD::unit_scale, t.a.y * WORLD::unit_scale);
				glVertex2f(t.b.x * WORLD::unit_scale, t.b.y * WORLD::unit_scale);
				glVertex2f(t.c.x * WORLD::unit_scale, t.c.y * WORLD::unit_scale);
			}
			glEnd();

			glDepthMask(GL_TRUE);
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);

		}

	}










	glPointSize(5);
	glColor3f(0.8, 0.8, 0.0);
	glBegin(GL_POINTS);
	glVertex2f(plr_pos.x * WORLD::unit_scale, plr_pos.y * WORLD::unit_scale);
	glEnd();
}


void LvlEditor::MouseFunctionalityUtil(const int MouseState)
{
	switch (MouseState) {
	default: 
		LVLEDITOR::draw_portal_selection_line = false;
		break;

		// MIDDLE clock
	case UTIL::MIDDLE:

		break;
		// LEFT click
	case UTIL::LEFT:
		// place walls
		if (LVLEDITOR::is_first_wall) {

			UTIL::polygonWall CW;
			CW.start = UTIL::SnapToGrid(mouse);
			CW.end = CW.start;

			LVLEDITOR::current_wall = CW;

			LVLEDITOR::is_first_wall = false;
		}
		else {
			LVLEDITOR::current_wall.end = UTIL::SnapToGrid(mouse);

			LVLEDITOR::current_wall.normal = UTIL::compute_normal(LVLEDITOR::current_wall.start, LVLEDITOR::current_wall.end);
			LVLEDITOR::current_wall.Inormal = UTIL::compute_normal(LVLEDITOR::current_wall.start, LVLEDITOR::current_wall.end, true);
			LVLEDITOR::current_wall.center = (LVLEDITOR::current_wall.start + LVLEDITOR::current_wall.end) * 0.5f;
			//LVLEDITOR::current_wall.ids_in_Sector = { 0, 1 };
			LVLEDITOR::current_wall.id = walls.size();

			walls.push_back(LVLEDITOR::current_wall);

			LVLEDITOR::is_first_wall = true;

			lvlSectors = get_sectors(walls, WORLD::unit_scale);
		}





		break;
		// RIGHT click
	case UTIL::RIGHT:
		// turn walls into portals
		// 
		// loop through all walls
		// line line intersection algoythm because a line is drawn that intersects walls
		// if positive turn wall into a portal
		// add the ids to sector relative from sector?

		// collision checks will be done in openglrender (yes but only for visuals) and on second click

		// eabling the draw selection line
		if (!LVLEDITOR::draw_portal_selection_line) {
			LVLEDITOR::last_mouse_pos = mouse;
		}
		else {
			LVLEDITOR::last_mouse_pos = mouse;
			// collision check here? idk, or on button press in imgui or on keyboard, we'll see
		}
		LVLEDITOR::draw_portal_selection_line = !LVLEDITOR::draw_portal_selection_line;
		LVLEDITOR::make_portals = !LVLEDITOR::make_portals;

		break;
	}
}



void LvlEditor::ImGuirender() {
	ImGui::Begin("Debug");

	ImGui::InputFloat("Mouse X", &mouse.x);
	ImGui::InputFloat("Mouse Y", &mouse.y);

	ImGui::Separator();

	ImGui::SliderFloat("Plr X", &plr_pos.x, 0, WORLD::world_scale);
	ImGui::SliderFloat("Plr Y", &plr_pos.y, 0, WORLD::world_scale);

	ImGui::Separator();

	ImGui::Text("Sectors need to be build first");

	if (ImGui::Button("turn overlaps into portals")) {

		// TODO: make sure that the pointers are a variable
		// current issue is comparison makes all walls portals  obviously 
		for (UTIL::polygonWall& wall1 : walls) {
			for (UTIL::polygonWall& wall2 : walls) {
				if (UTIL::compare_walls(wall1, wall2)) {
					if (wall1.id == wall2.id) {
						continue;
					}

					Logger::Log(LogLevel::INFO, "merged walls");

					wall1.is_portal = true;
					wall2.is_portal = true;
				}
			
			}
		}
	}



	ImGui::Separator();

	if (ImGui::Button("build sectors")) {
		lvl.sectors = get_sectors(walls, WORLD::unit_scale);
	}

	ImGui::End();
}


void LvlEditor::render()
{
	// Push 2D projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, window_width, 0, window_height, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	openglRender();

	// Restore 3D matrices
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}


Level LvlEditor::get_Level() 
{
	lvl.sectors = get_sectors(walls, WORLD::unit_scale);

	return lvl;
}