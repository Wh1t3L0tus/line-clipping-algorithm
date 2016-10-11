//
// Created by theo on 10/11/16.
//

#include "Math.h"

#define EPSILON 0.000001f

float Math::getDeterminant(const Mat2X2& m) {
    return m.a * m.d - m.b * m.c;
}

Mat2X2 Math::getInverseMatrix(const Mat2X2& m) {
    float inverseDet = 1.0f / getDeterminant(m);
    return Mat2X2{ m.d * inverseDet, -m.b * inverseDet,
                  -m.c * inverseDet,  m.a * inverseDet};
}

bool Math::isIntersecting(const Segment& a, const Segment& b) {

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
    }
    else {
        Col2 params = getInverseMatrix(lambda) * B;
        float t = params.a;
        float s = params.b;

        // Segment intersection found !
        // if it returns false : intersection is on one of the segments' prolongation
        return (t >= 0.0f && t <= 1.0f && s >= 0.0f && s <= 1.0f);
    }

    return false;
}

Vertex Math::getIntersection(const Segment& a, const Segment& b) {
    return Vertex{0.0f, 0.0f};
}

bool Math::isSegmentVisible(const Segment& shapeSeg, const Segment& windowSeg) {
    return false;
}