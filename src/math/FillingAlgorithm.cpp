#include <vector>
#include <stack>
#include <limits>
#include "FillingAlgorithm.h"

void FillingAlgorithm::BoundingBoxFill(const Vec2* vertices, unsigned int vertexCount, const Color& fillColor, std::vector<Vec2> & filledRegion) {
    int xMin = std::numeric_limits<int>::max();
    int yMin = std::numeric_limits<int>::max();
    int xMax = std::numeric_limits<int>::min();
    int yMax = std::numeric_limits<int>::min();

    int windowHeight = 600;
    int windowWidth = 800;

    for (int i = 0; i < vertexCount; i++) {
        Vec2 vertex = vertices[i];
        int x = iLerp((vertex.x + 1.0f) / 2.0f, 0, windowWidth);
        int y = iLerp((vertex.y + 1.0f) / 2.0f, 0, windowHeight);

        if (x < xMin) {
            xMin = x;
        }

        if (y < yMin) {
            yMin = y;
        }

        if (x > xMax) {
            xMax = x;
        }

        if (y > yMax) {
            yMax = y;
        }
    }

    for (int currentX = xMin; currentX <= xMax; currentX++) {
        for (int currentY = yMin; currentY <= yMax; currentY++) {
            Vec2 currentPoint{
                fLerp((float)currentX / (float)windowWidth, -1.0f, 1.0f),
                fLerp((float)currentY / (float)windowHeight, -1.0f, 1.0f)
            };
            if (Math::isPointInside(&vertices[0], vertexCount, currentPoint)) {
                filledRegion.push_back(currentPoint);
            }
        }
    }


}