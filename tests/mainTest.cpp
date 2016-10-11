//
// Created by theo on 10/12/16.
//

#include <cassert>
#include "../src/math/Math.h"

int main(int argc, char** argv) {

    Segment a{
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

    assert(Math::isIntersecting(a, b));
    assert(!Math::isIntersecting(b, c));

    return 0;
}