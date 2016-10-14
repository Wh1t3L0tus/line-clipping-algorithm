//
// Created by theo on 10/12/16.
//

#include <iostream>
#include "../src/math/Math.h"
#include "unittest.h"

int main(int argc, char** argv) {

    Segment a {
        0, 0,
        10, 10
    };

    Segment b {
        5, 0,
        5, 10
    };

    Segment c {
        6, 0,
        6, 10
    };

    Segment d {
        11, 11,
        11, 12
    };

    Segment e {
        11, 11,
        20, 20
    };

    Segment f {
        0, 0,
        1, 1.1f
    };

    Segment g {
         2, 2.2f,
         3, 3.3f
    };

    Vec2 gNormal = Math::getNormal(g);
    Vec2 gVector{g.x2 - g.x1, g.y2 - g.y1};
    float dotProduct = Math::dotProduct(gNormal, gVector);

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

    return 0;
}