//
// Created by theo on 10/11/16.
//

#include <cfloat>
#include <cassert>
#include "Math.h"


#define EPSILON FLT_EPSILON

float Math::getDeterminant(const Mat2X2& m) {
    return m.a * m.d - m.b * m.c;
}

Mat2X2 Math::getInverseMatrix(const Mat2X2& m) {
    float inverseDet = 1.0f / getDeterminant(m);
    return Mat2X2{ m.d * inverseDet, -m.b * inverseDet,
                  -m.c * inverseDet,  m.a * inverseDet};
}

IntersectionResult Math::getIntersection(const Segment &a, const Segment &b) {

    IntersectionResult result{false, false, false, false};

    Mat2X2 lambda {
            a.x2 - a.x1, b.x1 - b.x2,
            a.y2 - a.y1, b.y1 - b.y2
    };

    Col2 B {
        b.x1 - a.x1,
        b.y1 - a.y1
    };

    float lambdaDet = getDeterminant(lambda);
    if (lambdaDet >= -EPSILON && lambdaDet <= EPSILON) {
        // special case : either there is no collision between a and b or they are on the same line
        // until it causes trouble, let's assume there is no intersection
        result.isEitherParallelOrMerged = true;
    }
    else {
        Col2 params = getInverseMatrix(lambda) * B;
        float t = params.a;
        float s = params.b;

        result.intersection = Vec2{a.x1, a.y1} + (Vec2{a.x2, a.y2} - Vec2{a.x1, a.y1}) * t;

        // Segment intersection found !
        if (t >= 0.0f && t <= 1.0f && s >= 0.0f && s <= 1.0f) {
            result.isIntersecting = true;
        }
        // Intersection is outside b segment
        else if (t >= 0.0f && t <= 1.0f) {
            result.isIntersecting = false;
            result.isOutsideB = true;
        }
        // Intersection is outside a segment
        else if (s >= 0.0f && s <= 1.0f) {
            result.isIntersecting = false;
            result.isOutsideA = true;
        }
        // No intersection
        else {
            result.isIntersecting = false;
        }
    }

    return result;
}

bool Math::isPointVisible(const Vec2 &point, const Segment &windowSeg) {

    Vec2 tmp{point.x - windowSeg.x1, point.y - windowSeg.y1};
    float dotProduct = Math::dotProduct(windowSeg.normal, tmp);

    // dotProduct > 0 : point is on the right
    // dotProduct < 0 : point is on the left
    // dotProduct == 0.0f : point is on the segment

    return dotProduct <= 0.0f;
}

float Math::dotProduct(const Vec2& a, const Vec2& b) {
    return a.x * b.x + a.y * b.y;
}

Vec2 Math::getNormal(const Segment &segment) {
    return Vec2{segment.y1 - segment.y2, segment.x2 - segment.y1} * -1.0f;
}

Segment Math::makeSegment(const Vertex& a, const Vertex& b) {
    return Segment{a.x, a.y, b.x, b.y};
}

std::vector<Segment> Math::getSegmentsFromVertices(int vertexCount, const Vertex *vertices) {

    std::vector<Segment> segments;

    for (int i = 0; i < vertexCount - 1; i++) {
        Segment segment{vertices[i].x, vertices[i].y, vertices[i + 1].x, vertices[i + 1].y};
        segment.normal = getNormal(segment);
        segments.push_back(segment);
    }
    Segment lastSegment{vertices[vertexCount - 1].x, vertices[vertexCount - 1].y,
                        vertices[0].x, vertices[0].y
    };
    lastSegment.normal = getNormal(lastSegment);

    segments.push_back(lastSegment);

    return segments;
}