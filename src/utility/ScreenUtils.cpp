#include <GL/glew.h>
#include <vector>
#include <GL/freeglut.h>
#include <iostream>
#include "ScreenUtils.h"

std::vector<Color> ScreenUtils::ColorBuffer;
int ScreenUtils::Width;
int ScreenUtils::Height;

void ScreenUtils::InitScreenBuffer() {
    ColorBuffer.clear();
    Width = glutGet(GLUT_WINDOW_WIDTH);
    Height = glutGet(GLUT_WINDOW_HEIGHT);
    ColorBuffer.reserve(Width * Height);
    for (int i = 0; i < Width * Height; i++) {
        ColorBuffer.push_back(Color{});
    }
}

Color ScreenUtils::ReadPixel(int x, int y) {
    Color color;
    glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &color);

    return color;
}

void ScreenUtils::BufferizeScreenPixels() {
//    int oldMilliSecs = glutGet(GLUT_ELAPSED_TIME);
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
    int windowWidth = glutGet(GLUT_WINDOW_WIDTH);

//    std::cout << "Start screen reading..." << std::endl;
    for (int height = 0; height < windowHeight; height++) {
        for (int width = 0; width < windowWidth; width++) {
            ColorBuffer[height * windowWidth + width] = ScreenUtils::ReadPixel(width, height);
        }
    }
//    std::cout << "Took " << (glutGet(GLUT_ELAPSED_TIME) - oldMilliSecs) / 1000.0f << " seconds to read the screen" << std::endl;
}

const Color& ScreenUtils::ReadColorFromBuffer(int x, int y) {
    return ColorBuffer[y * Width + x];
}