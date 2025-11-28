


#include "Sector.h"

//using namespace UTIL;


Sector::Sector()
{

}

void Sector::draw()
{
	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.6f, 0.6f);

	int n = points.size();

	for (int i = 0; i < n; i++) {
		glm::vec2& p1 = points[i];
		glm::vec2& p2 = points[(i + 1) % n]; // wraps to 0 when i = last index
		bool skip = false;

		for (auto& ids : portal_ids) {
			if (UTIL::find_index(points, p1) == ids[0] && UTIL::find_index(points, p2) == ids[1]) {
				skip = true;
				break;
			}
		}


		if (skip)
			continue;

		glVertex3f(p1.x, ceiling, p1.y);
		glVertex3f(p1.x, floor, p1.y);

		glVertex3f(p2.x, floor, p2.y);
		glVertex3f(p2.x, ceiling, p2.y);

	}

	glEnd();


	if (points.size() >= 3) {
		glFrontFace(GL_CW);
		glColor3f(0.3, 0.3, 0.3);
		glBegin(GL_TRIANGLES);
		for (const auto& t : triangles) {
			glVertex3f(t.a.x, floor, t.a.y);
			glVertex3f(t.b.x, floor, t.b.y);
			glVertex3f(t.c.x, floor, t.c.y);
		}
		glEnd();


		glFrontFace(GL_CCW);
		glColor3f(0.8, 0.8, 0.8);
		glBegin(GL_TRIANGLES);
		for (const auto& t : triangles) {
			glVertex3f(t.a.x, ceiling, t.a.y);
			glVertex3f(t.b.x, ceiling, t.b.y);
			glVertex3f(t.c.x, ceiling, t.c.y);
		}
		glEnd();
	}
}



// helps rebuild the sector if the data is updated
void Sector::rebuild()
{
	triangles = UTIL::triangulate(points);
}























