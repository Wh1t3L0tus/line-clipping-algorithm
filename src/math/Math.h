//
// Created by theo on 10/11/16.
//

#ifndef LINE_CLIPPING_ALGORITHM_MATH_H
#define LINE_CLIPPING_ALGORITHM_MATH_H


#include "../utility/utils.h"

struct Segment {
    float x1;
    float y1;
    float x2;
    float y2;
};

struct Col2 {
    float a;
    float b;
};

struct Mat2X2 {

    float a;
    float b;
    float c;
    float d;

    inline Mat2X2 operator*(const Mat2X2& m) {
        return Mat2X2 {
            a * m.a + b * m.c, a * m.b + b * m.d,
            c * m.a + d * m.c, d * m.b + d * m.d
        };
    }

    inline Col2 operator*(const Col2& col) {
        return Col2 {
            a * col.a + b * col.b,
            c * col.b + d * col.b
        };
    }
};

struct Vec2 {
    float x;
    float y;

    Vec2 operator+(const Vec2& vec) const {
        return Vec2{x + vec.x, y + vec.y};
    }

    Vec2 operator-(const Vec2& vec) const {
        return Vec2{x - vec.x, y - vec.y};
    }

    Vec2 operator*(float n) {
        return Vec2{n * x, n * y};
    }
};

struct IntersectionResult {
    bool isIntersecting;
    bool isOutsideA;
    bool isOutsideB;
    bool isEitherParallelOrMerged;
    Vec2 intersection;
};

class Math {
public:
    static float getDeterminant(const Mat2X2& matrix);
    static Mat2X2 getInverseMatrix(const Mat2X2& matrix);
    static IntersectionResult getIntersection(const Segment &a, const Segment &b);
    static bool isSegmentVisible(const Segment& shapeSeg, const Segment& windowSeg);
};


#endif //LINE_CLIPPING_ALGORITHM_MATH_H
