



#include "LvlEditor.h"


LvlEditor::LvlEditor()
{

}

void LvlEditor::renderSectors() {

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

		break;
		// RIGHT click
	case UTIL::RIGHT:

		break;
	}
}



void LvlEditor::ImGuirender() {
	ImGui::Begin("Debug");

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
	return lvl;
}