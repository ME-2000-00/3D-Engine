


#include "Sector.h"

void Sector::draw()
{

	for (Wall& wall : walls) {

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


		//glm::vec2 e1 = glm::vec2(wall.start.x, wall.start.y) - glm::vec2(wall.end.x, wall.end.y);
		//glm::vec2 normal = glm::normalize(glm::vec2(-e1.y, e1.x));


		//// normals
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
}

void Sector::compue_and_set_normal(Wall& wall)
{
	glm::vec2 e1 = wall.start - wall.end;
	glm::vec2 normal = glm::normalize(glm::vec2(-e1.y, e1.x));
	wall.normal = normal;
}

Sector::Sector()
{

}
