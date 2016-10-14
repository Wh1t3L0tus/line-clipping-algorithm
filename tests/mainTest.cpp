//
// Created by theo on 10/12/16.
//

#include <iostream>
#include "../src/math/Math.h"
#include "unittest.h"

int main(int argc, char** argv) {

    Segment a {
        0.0f, 0.0f,
        10.0f, 10.0f
    };

    Segment b {
        5.0f, 0.0f,
        5.0f, 10.0f
    };

    Segment c {
        6.0f, 0.0f,
        6.0f, 10.0f
    };

    Segment d {
        11.0f, 11.0f,
        11.0f, 12.0f
    };

    Segment e {
        11.0f, 11.0f,
        20.0f, 20.0f
    };

    Segment f {
        0.0f, 0.0f,
        1.0f, 1.1f
    };

    Segment g {
         2.0f, 2.2f,
         3.0f, 3.3f
    };

    Segment h {
        0.0f, 0.0f,
        1.0f, 0.0f
    };

    h.normal = Math::getNormal(h);
    float dotProduct = Math::dotProduct(h.normal, Vec2{h.x2 - h.x1, h.y2 - h.y1});

    IntersectionResult ab = Math::getIntersection(a, b);
    IntersectionResult bc = Math::getIntersection(b, c);
    IntersectionResult ad = Math::getIntersection(a, d);
    IntersectionResult ae = Math::getIntersection(a, e);
    IntersectionResult fg = Math::getIntersection(f, g);

    isTrue(ab.isIntersecting, "Main intersection case", "a and b segment should intersect");
    std::cout << "Intersection between [a] and [b] : " << ab.intersection.x << ", " << ab.intersection.y << std::endl;

    isFalse(bc.isIntersecting, "Parallel case", "a and b segment never intersect");

    isTrue(ad.isIntersecting, "Intersection outside segment case", "a intersect d");
    isTrue(ad.isOutsideA, "Intersection outside segment case", "intersection should be outside the a segment");
    isFalse(ad.isOutsideB, "Intersection outside segment case", "intersection is inside b segment");

    isTrue(ae.isEitherParallelOrMerged, "Either parallel or merged case", "Should be either parallel or merged");
    isTrue(fg.isEitherParallelOrMerged, "Either parallel or merged case 2", "Should be either parallel or merged");

    equals(dotProduct, 0.0f, "Dot product and getNormal test", "The dot product of a vector by its normal should be 0.0f");

    isTrue(Math::isPointVisible(Vec2{0.0f, 0.0f}, h), "Point visibility : on segment case", "Point on segment should be visible");
    isTrue(Math::isPointVisible(Vec2{-10.0f, 0.0f}, h), "Point visibility : on segment case (2)", "Point on segment should be visible");
    isTrue(Math::isPointVisible(Vec2{3.7777f, 0.0f}, h), "Point visibility : on segment case (3)", "Point on segment should be visible");

    Vec2 test{1.0f, 1.0f};
    isTrue(Math::isPointVisible(test, h), "Point visibility : on the left case", "Point should be visible since it is on the left");
    isFalse(Math::isPointVisible(Vec2{1.0f, -1.0f}, h), "Point visibility : on the right case", "Point shouldn't be visible since it is on the right");

    return 0;
}