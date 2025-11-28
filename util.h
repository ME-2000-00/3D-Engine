#pragma once
// here will all utility functions be
// please define them with inline

// includes
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include <memory>
#include <string>
#include <array>

// libraries
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/constants.hpp>
#include <gtc/type_ptr.hpp>

// own made classes 

namespace WORLD {
    inline int unit_scale = 40;
    inline int world_scale = 100;
}

namespace UTIL {

    // mouse utils

    inline const int MIDDLE = 0;
    inline const int LEFT   = 1;
    inline const int RIGHT  = 2;

    // triangulation algorythm ----------------------------------------------------------------------------------------------------------------------------------

    struct Tri { glm::vec2 a, b, c; };

    inline bool pointInTri(const glm::vec2& p, const glm::vec2& a,
        const glm::vec2& b, const glm::vec2& c)
    {
        float w1 = (a.x * (c.y - a.y) + (p.y - a.y) * (c.x - a.x) - p.x * (c.y - a.y)) /
            ((b.y - a.y) * (c.x - a.x) - (b.x - a.x) * (c.y - a.y));

        float w2 = (p.y - a.y - w1 * (b.y - a.y)) / (c.y - a.y);

        return w1 >= 0 && w2 >= 0 && (w1 + w2) <= 1;
    }


    inline bool isConvex(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c) {
        glm::vec2 ab = b - a;
        glm::vec2 ac = c - a;
        return (ab.x * ac.y - ab.y * ac.x) > 0.0f;
    }

    inline std::vector<Tri> triangulate(std::vector<glm::vec2> poly) {
        std::vector<Tri> result;

        while (poly.size() > 3) {
            bool earFound = false;

            for (int i = 0; i < poly.size(); i++) {
                glm::vec2 a = poly[(i - 1 + poly.size()) % poly.size()];
                glm::vec2 b = poly[i];
                glm::vec2 c = poly[(i + 1) % poly.size()];

                // Check convex
                if (!isConvex(a, b, c))
                    continue;

                // Check no other vertex lies inside
                bool inside = false;
                for (int j = 0; j < poly.size(); j++) {
                    if (j == i || j == (i - 1 + poly.size()) % poly.size() || j == (i + 1) % poly.size())
                        continue;

                    if (pointInTri(poly[j], a, b, c)) {
                        inside = true;
                        break;
                    }
                }

                if (inside) continue;

                // Found an ear
                result.push_back({ a, b, c });
                poly.erase(poly.begin() + i);
                earFound = true;
                break;
            }

            if (!earFound) {
                //Logger::Log(LogLevel::ERROR, "Triangulation failed: bad polygon.");
                break;
            }
        }

        // Add last triangle
        if (poly.size() == 3) {
            result.push_back({ poly[0], poly[1], poly[2] });
        }

        return result;
    }
    // triangulation algorythm ----------------------------------------------------------------------------------------------------------------------------------



    // normals --------------------------------------------------------------------------------------------------------------------------------------------------

    inline glm::vec2 compute_normal(glm::vec2 p1, glm::vec2 p2, bool invert = false)
    {
        glm::vec2 e = p1 - p2; // vector along the wall

        if (glm::length(e) < 1e-6f) {
            return glm::vec2(0.0f, 0.0f);
        }


        if (invert) {
            glm::vec2 normal(-e.y, e.x);
            normal = glm::normalize(normal);

            return normal;
        }
        else {
            glm::vec2 normal(e.y, -e.x);
            normal = glm::normalize(normal);

            return normal;
        }
    }

    // normals --------------------------------------------------------------------------------------------------------------------------------------------------



    // point in polygon -----------------------------------------------------------------------------------------------------------------------------------------

    struct polygonWall {
        // struct made for polygon checks
        glm::vec2 start;
        glm::vec2 end;

        glm::vec2 normal;
        glm::vec2 Inormal;

        glm::vec2 center;

        // used for portals and identification in sector if polygon wall is used
        std::array<int, 2> ids_in_Sector;
        int id;

        bool is_portal = false;
    };


    inline bool pointInPolygon(const glm::vec2& p, const std::vector<glm::vec2>& polygon) {


        bool inside = false;
        int n = polygon.size();

        for (int i = 0; i < (n + 1 / 2); i++) {
            int i1 = i;
            int i2 = i + 1;

            if (i == (n + 1 / 2) - 1) {
                i2 = 0;
            }


            glm::vec2 a = glm::vec2(polygon[i1].x * WORLD::unit_scale, polygon[i1].y * WORLD::unit_scale);
            glm::vec2 b = glm::vec2(polygon[i2].x * WORLD::unit_scale, polygon[i2].y * WORLD::unit_scale);


            if (a.y == b.y) continue; // skip horizontal edges to avoid divide by zero

            bool intersect = ((a.y > p.y) != (b.y > p.y)) &&
                (p.x < (b.x - a.x) * (p.y - a.y) / (b.y - a.y) + a.x);

            if (intersect) inside = !inside;
        }
        return inside;
    }


    // render grid relative to screen in 2D mode ----------------------------------------------------------------------------------------------------------------

    inline void renderScreenGrid(int windowWidth, int windowHeight)
    {
        float half_world_px = (WORLD::world_scale * WORLD::unit_scale) * 0.5f;

        // Minor lines
        glLineWidth(1.0f);
        glColor3f(0.2f, 0.2f, 0.2f);
        glBegin(GL_LINES);
        for (int i = -WORLD::world_scale / 2; i <= WORLD::world_scale / 2; i++) {
            if (i % 5 != 0) {
                float x = windowWidth / 2 + i * WORLD::unit_scale;
                float y = windowHeight / 2 + i * WORLD::unit_scale;

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
        for (int i = -WORLD::world_scale / 2; i <= WORLD::world_scale / 2; i += 5) {
            float x = windowWidth / 2 + i * WORLD::unit_scale;
            float y = windowHeight / 2 + i * WORLD::unit_scale;

            // vertical
            glVertex2f(x, windowHeight / 2 - half_world_px);
            glVertex2f(x, windowHeight / 2 + half_world_px);

            // horizontal
            glVertex2f(windowWidth / 2 - half_world_px, x);
            glVertex2f(windowWidth / 2 + half_world_px, x);
        }
        glEnd();
    }

    // render grid relative to screen in 2D mode ----------------------------------------------------------------------------------------------------------------



    // snap point to grid ---------------------------------------------------------------------------------------------------------------------------------------

    inline glm::vec2 SnapToGrid(const glm::vec2& point, glm::vec2 panOffset = glm::vec2(0.0f, 0.0f))
    {
        glm::vec2 snapped;
        snapped.x = round((point.x - panOffset.x) / WORLD::unit_scale) * WORLD::unit_scale + panOffset.x;
        snapped.y = round((point.y - panOffset.y) / WORLD::unit_scale) * WORLD::unit_scale + panOffset.y;
        return snapped;
    }

    // snap point to grid ---------------------------------------------------------------------------------------------------------------------------------------


    // find id in vector-- --------------------------------------------------------------------------------------------------------------------------------------

    template<typename T>
    inline int find_index(const std::vector<T>& vector, const T& item) {
        auto it = std::find(vector.begin(), vector.end(), item);

        if (it != vector.end()) {
            return std::distance(vector.begin(), it);
        }
        else {
            return -1;
        }
    }

    // find id in vector-- --------------------------------------------------------------------------------------------------------------------------------------



    // line line intersection

    // Returns true if line segment AB intersects CD.
    // outPoint is set to intersection point if one exists.
    inline bool lineLineIntersection(
        const glm::vec2& A,
        const glm::vec2& B,
        const glm::vec2& C,
        const glm::vec2& D,
        glm::vec2& outPoint
    )
    {
        glm::vec2 r = B - A;
        glm::vec2 s = D - C;

        float denom = r.x * s.y - r.y * s.x;

        // Lines are parallel or collinear
        if (denom == 0.0f)
            return false;

        glm::vec2 diff = C - A;

        float t = (diff.x * s.y - diff.y * s.x) / denom;
        float u = (diff.x * r.y - diff.y * r.x) / denom;

        // Check if intersection is within both line segments
        if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f) {
            outPoint = A + t * r;
            return true;
        }

        return false;
    }

    // only works if the data is the same but not the ids
    inline bool compare_walls(const UTIL::polygonWall w1, const UTIL::polygonWall w2) {
        if (
            w1.start.x == w2.start.x &&
            w1.start.y == w2.start.y &&
            w1.end.x == w2.end.x &&
            w1.end.y == w2.end.y
            ) {
            return true;
        }
        return false;
    }
}


namespace LVLEDITOR {

    inline bool is_first_wall = true;
    inline bool hovering_sector = false;
    inline bool make_portals = false;

    inline UTIL::polygonWall first_wall;
    inline UTIL::polygonWall current_wall;

    inline bool draw_portal_selection_line = false;
    inline glm::vec2 last_mouse_pos = glm::vec2(-1.0f, -1.0f);

    struct Wall {
        glm::vec2 start;
        glm::vec2 end;

        bool is_portal;
    };


}
