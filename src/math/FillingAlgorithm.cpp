#include <vector>
#include <stack>
#include <limits>
#include <algorithm>
#include <iostream>
#include "FillingAlgorithm.h"

struct BoundingBox {
    int xMin;
    int yMin;
    int xMax;
    int yMax;
};

BoundingBox GetScreenSpaceBoundingBox(const Vec2* vertices, unsigned int vertexCount, int windowWidth, int windowHeight) {
    int xMin = std::numeric_limits<int>::max();
    int yMin = std::numeric_limits<int>::max();
    int xMax = std::numeric_limits<int>::min();
    int yMax = std::numeric_limits<int>::min();

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

    return BoundingBox{xMin, yMin, xMax, yMax};
}

int WorldToScreenSpace(float alpha, int maxValue) {
    return iLerp((alpha + 1.0f) / 2.0f, 0, maxValue);
}

Vec2 ScreenToWorldSpace(int x, int y, int windowWidth, int windowHeight) {
    return Vec2{
        fLerp(x / (float) windowWidth, -1.0f, 1.0f),
        fLerp(y / (float) windowHeight, -1.0f, 1.0f)
    };
}

void FillingAlgorithm::BoundingBoxFill(const Vec2* vertices, unsigned int vertexCount, const Color& fillColor, std::vector<Vec2> & filledRegion) {

    int windowWidth = 800;
    int windowHeight = 600;
    BoundingBox bb = GetScreenSpaceBoundingBox(vertices, vertexCount, 800, 600);

    for (int currentX = 0; currentX <= 800; currentX++) {
        for (int currentY = 0; currentY <= 600; currentY++) {
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

struct ScanLineElement {
    int xMin; // screen space
    int yMax; // screen space
    float invertedSlope;
};

float GetMin(float n1, float n2) {
    if (n1 < n2)
        return n1;
    else
        return n2;
}

float GetMax(float n1, float n2) {
    if (n1 > n2)
        return n1;
    else
        return n2;
}

float GetInvertedSlope(const Segment& s) {
    return (s.y2 - s.y1) / (s.x2 - s.x1);
}

void InsertIn(std::vector<ScanLineElement>& list, ScanLineElement sce) {
    std::vector<ScanLineElement>::iterator it = list.begin();
    while (it != list.end() && it->xMin < sce.xMin) {
        it++;
    }
    list.insert(it, sce);
}

void CreateIntermediateStructure(const Vec2* vertices, unsigned int vertexCount, const BoundingBox& bb, int windowWidth, int windowHeight, std::vector<std::vector<ScanLineElement>* >& intermediateStructure ) {

    std::vector<Segment> segments = Math::getSegmentsFromVertices(vertexCount, vertices);

    for (int i = bb.yMin; i < bb.yMax; i++) {
        intermediateStructure.push_back(new std::vector<ScanLineElement>());
    }

    for (Segment segment : segments) {
        if (segment.y1 != segment.y2) {
            int lowerPart = WorldToScreenSpace(GetMin(segment.y1, segment.y2), windowHeight);
            ScanLineElement sce{WorldToScreenSpace(GetMin(segment.x1, segment.x2), windowWidth),
                                WorldToScreenSpace(GetMax(segment.y1, segment.y2), windowHeight),
                                GetInvertedSlope(segment)};

            std::vector<ScanLineElement>* currentHeightList = intermediateStructure[lowerPart];
            std::vector<ScanLineElement>::iterator it = currentHeightList->begin();
            while (it != currentHeightList->end() && it->xMin < sce.xMin) {
                it++;
            }
            currentHeightList->insert(it, sce);
        }
    }

    delete [] vertices;
}

void AddScanLinesTo(std::vector<ScanLineElement>& list, std::vector<ScanLineElement> toAdd) {
    for (ScanLineElement sce : toAdd) {
        InsertIn(list, sce);
    }
}

void FillingAlgorithm::ScanLineFill(const Vec2* vertices, unsigned int vertexCount, std::vector<Segment>& linesToDraw) {
    int windowWidth = 800;
    int windowHeight = 600;
    std::vector<ScanLineElement> activatedEdgesList;
    std::vector<std::vector<ScanLineElement>* > intermediateStruct;
    BoundingBox bb = GetScreenSpaceBoundingBox(vertices, vertexCount, windowWidth, windowHeight);

    CreateIntermediateStructure(vertices, vertexCount, bb, windowWidth, windowHeight, intermediateStruct);

    for (int currentHeight = 0; currentHeight < 600; currentHeight++) {
        AddScanLinesTo(activatedEdgesList, *intermediateStruct[currentHeight]);

        if (!activatedEdgesList.empty()) {

            for (int i = 0; i < activatedEdgesList.size(); i++) {
                Vec2 lineStart = ScreenToWorldSpace(activatedEdgesList[i].xMin, currentHeight, windowWidth, windowHeight);
                Vec2 lineEnd = ScreenToWorldSpace(activatedEdgesList[i + 1].xMin, currentHeight, windowWidth, windowHeight);
                linesToDraw.push_back(Segment{lineStart.x, lineStart.y, lineEnd.x, lineEnd.y});
            }

            // Remove elements lower than current height from activatedEdgesList
            // and update xMin values
            auto it = activatedEdgesList.begin();
            while (it != activatedEdgesList.end()) {
                if (currentHeight >= it->yMax) {
                    activatedEdgesList.erase(it++);
                }
                else {
                    it->xMin += it->invertedSlope;
                    it++;
                }
            }

            // Ensure activatedEdgesList is sorted
            std::vector<ScanLineElement> tmpList;
            it = activatedEdgesList.begin();
            while (it != activatedEdgesList.end()) {
                InsertIn(tmpList, *it);
                activatedEdgesList.erase(it++);
            }
            activatedEdgesList = tmpList;
        }
    }
}