#include <vector>
#include <stack>
#include <iostream>
#include <GL/freeglut.h>
#include <limits>
#include "FillingAlgorithm.h"
#include "../utility/ScreenUtils.h"

struct Vec2i {
    int x;
    int y;
};

void FillingAlgorithm::FloodFill(int xx, int yy, const Color &edgeColor, const Color& fillColor, std::vector<Vertex> &filledRegion) {
    std::stack<Vec2i> coordinateStack;
    Color leftColor;
    Color rightColor;
    Color currentColor;
    int xLeft;
    int xRight;

    coordinateStack.push(Vec2i{xx, yy});

    while (!coordinateStack.empty()) {
        Vec2i currentLocation = coordinateStack.top();
        coordinateStack.pop();

        currentColor = ScreenUtils::ReadColorFromBuffer(currentLocation.x, currentLocation.y);
        xRight = currentLocation.x + 1;
        rightColor = currentColor;
        while (rightColor != currentColor) {
            xRight++;
            rightColor = ScreenUtils::ReadColorFromBuffer(xRight, currentLocation.y);
        }

        xRight--;
        xLeft = currentLocation.x - 1;
        leftColor = currentColor;

        while (leftColor != currentColor) {
            xLeft--;
            leftColor = ScreenUtils::ReadColorFromBuffer(xLeft, currentLocation.y);
        }
        xLeft++;

        for (int i = xLeft; xLeft < xRight; i++) {
            filledRegion.push_back(Vertex{i, currentLocation.y});
        }

        currentLocation.x = xRight;
        currentColor = ScreenUtils::ReadColorFromBuffer(currentLocation.x, currentLocation.y);

        while (currentLocation.x >= xLeft) {
            while ((currentColor == edgeColor || currentColor == fillColor) && currentLocation.x >= xLeft) {
                currentLocation.x--;
                currentColor = ScreenUtils::ReadColorFromBuffer(currentLocation.x, currentLocation.y + 1);
            }

            if (currentLocation.x >= xLeft && currentColor != edgeColor && currentColor != fillColor) {
                coordinateStack.push(Vec2i{currentLocation.x, currentLocation.y + 1});
            }

            while (currentColor != edgeColor && currentLocation.x >= xLeft) {
                currentLocation.x--;
                currentColor = ScreenUtils::ReadColorFromBuffer(currentLocation.x, currentLocation.y + 1);
            }
        }

        currentLocation.x = xRight;
        currentColor = ScreenUtils::ReadColorFromBuffer(currentLocation.x, currentLocation.y - 1);
        while (currentLocation.x >= xLeft) {
            while ((currentColor == edgeColor || currentColor == fillColor) && currentLocation.x >= xLeft) {
                currentLocation.x--;
                currentColor = ScreenUtils::ReadColorFromBuffer(currentLocation.x, currentLocation.y - 1);
            }

            if (currentLocation.x >= xLeft && currentColor != edgeColor && currentColor != fillColor) {
                coordinateStack.push(Vec2i{currentLocation.x, currentLocation.y - 1});
            }

            while (currentColor != edgeColor && currentLocation.x >= xLeft) {
                currentLocation.x--;
                currentColor = ScreenUtils::ReadColorFromBuffer(currentLocation.x, currentLocation.y - 1);
            }
        }
    }
}

void FillingAlgorithm::BoundingBoxFill(const Vertex* vertices, unsigned int vertexCount, const Color& fillColor, std::vector<Vertex> & filledRegion) {
    // First we need to find the bounding box of our shape
    int xMin = std::numeric_limits<int>::max();
    int yMin = std::numeric_limits<int>::max();
    int xMax = std::numeric_limits<int>::min();
    int yMax = std::numeric_limits<int>::min();

    int windowHeight = 600;
    int windowWidth = 800;

    for (int i = 0; i < vertexCount; i++) {
        Vertex vertex = vertices[i];
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
                filledRegion.push_back(Vertex{currentPoint.x, currentPoint.y});
            }
        }
    }


}