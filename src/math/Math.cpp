//
// Created by theo on 10/11/16.
//

#include <cfloat>
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
            result.isIntersecting = true;
            result.isOutsideB = true;
        }
        // Intersection is outside a segment
        else if (s >= 0.0f && s <= 1.0f) {
            result.isIntersecting = true;
            result.isOutsideA = true;
        }
        // No intersection
        else {
            result.isIntersecting = false;
        }
    }

    return result;
}

bool Math::isSegmentVisible(const Segment& shapeSeg, const Segment& windowSeg) {
    return false;
}