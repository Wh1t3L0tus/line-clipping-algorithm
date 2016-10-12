//
// Created by theo on 10/12/16.
//

#include <cassert>
#include <iostream>
#include "../src/math/Math.h"

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

    IntersectionResult ab = Math::getIntersection(a, b);
    IntersectionResult bc = Math::getIntersection(b, c);
    assert(ab.isIntersecting);
    std::cout << "Intersection between [a] and [b] : " << ab.intersection.x << ", " << ab.intersection.y << std::endl;
    assert(!bc.isIntersecting);

    return 0;
}