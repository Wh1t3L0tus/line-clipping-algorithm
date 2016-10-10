//
// Created by theo on 10/10/16.
//

#ifndef LINE_CLIPPING_ALGORITHM_UTILS_H
#define LINE_CLIPPING_ALGORITHM_UTILS_H

struct Vertex {
    float x;
    float y;
};

struct Color {
    float r;
    float g;
    float b;
    float a;
};

inline float lerp(float t, float start, float target) {
    return (1 - t) * start + t * target;
}


#endif //LINE_CLIPPING_ALGORITHM_UTILS_H
