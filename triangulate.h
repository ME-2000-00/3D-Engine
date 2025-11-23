#pragma once

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include <memory>
#include <string>

#include "Logger.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/constants.hpp>
#include <gtc/type_ptr.hpp>

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
