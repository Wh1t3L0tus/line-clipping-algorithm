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
    unsigned char r;
    unsigned char g;
    unsigned char b;

    bool operator!=(const Color& color) {
        return r != color.r || g != color.g || b != color.b;
    }

    bool operator==(const Color& color) {
        return r == color.r && g == color.g && b == color.b;
    }
};

struct ColoredVertex {
    Vertex vertex;
    Color color;
};

inline float fLerp(float t, float start, float target) {
    return (1 - t) * start + t * target;
}

inline int iLerp(float t, int start, int target) {
    return int((1 - t) * start + t * target);
}

#endif //LINE_CLIPPING_ALGORITHM_UTILS_H
