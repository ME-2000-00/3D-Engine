


#include "Sector.h"
#include "triangulate.h"



void Sector::draw()
{

	std::vector<glm::vec2> floor_ceeling_vertices;


	for (Wall& wall : walls) {
		if (!wall.portal) {
					// quads
			glColor3f(wall.r, wall.g, wall.b);

			glBegin(GL_QUADS);
			glNormal3f(1, 0, 0);
			glVertex3f(wall.end.x, floor, wall.end.y);
			glVertex3f(wall.end.x, ceeling, wall.end.y);
			glVertex3f(wall.start.x, ceeling, wall.start.y);
			glVertex3f(wall.start.x, floor, wall.start.y);
			glEnd();

			glColor3f(0.0f, 0.0f, 0.6f);
			glLineWidth(2.0f);
		}
		else {
			// add wall logic here later

			//	|||||TOP||||||
			//	|            |
			//  |            |
			//	||||BOTTOM||||
			//	||||||||||||||
		}



		// wall face
		floor_ceeling_vertices.push_back(wall.start);




		glm::vec2 e1 = wall.start - wall.end;
		glm::vec2 normal = glm::normalize(glm::vec2(e1.y, e1.x));


		// normals
		//glm::vec3 center = (
		//	glm::vec3(wall.end.x, floor, wall.end.y) +
		//	glm::vec3(wall.end.x, ceeling, wall.end.y) +
		//	glm::vec3(wall.start.x, ceeling, wall.start.y) +
		//	glm::vec3(wall.start.x, floor, wall.start.y)
		//	) / 4.0f;

		//glBegin(GL_LINES);
		//	glVertex3f(center.x, center.y, center.z);
		//	glVertex3f(normal.x + center.x * 0.1f, center.y, normal.y + center.z * 0.1f);
		//glEnd();
	}


	if (walls.size() >= 3) {
		std::vector<Tri> tris = triangulate(floor_ceeling_vertices);

		glFrontFace(GL_CW);
		glColor3f(0.3, 0.3, 0.3);
		glBegin(GL_TRIANGLES);
		for (const auto& t : tris) {
			glVertex3f(t.a.x, floor , t.a.y);
			glVertex3f(t.b.x, floor , t.b.y);
			glVertex3f(t.c.x, floor , t.c.y);
		}
		glEnd();


		glFrontFace(GL_CCW);
		glColor3f(0.8, 0.8, 0.8);
		glBegin(GL_TRIANGLES);
		for (const auto& t : tris) {
			glVertex3f(t.a.x, ceeling, t.a.y);
			glVertex3f(t.b.x, ceeling, t.b.y);
			glVertex3f(t.c.x, ceeling, t.c.y);
		}
		glEnd();
	}

}

void Sector::compue_and_set_normal(Wall& wall)
{
	glm::vec2 e1 = wall.end - wall.start; // vector along the wall

	if (glm::length(e1) < 1e-6f) {
		wall.normal = glm::vec2(0.0f, 0.0f); // degenerate wall
		return;
	}

	glm::vec2 normal(-e1.y, e1.x); // 90° CCW
	wall.normal = glm::normalize(normal);
}



Sector::Sector()
{

}
