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

class Math {
public:
    static float getDeterminant(const Mat2X2& matrix);
    static Mat2X2 getInverseMatrix(const Mat2X2& matrix);
    static bool isIntersecting(const Segment& a, const Segment& b);
    static Vertex getIntersection(const Segment& a, const Segment& b);
    static bool isSegmentVisible(const Segment& shapeSeg, const Segment& windowSeg);
};


#endif //LINE_CLIPPING_ALGORITHM_MATH_H
