#ifndef LINE_CLIPPING_ALGORITHM_SCREENUTILS_H
#define LINE_CLIPPING_ALGORITHM_SCREENUTILS_H


#include "utils.h"

class ScreenUtils {

public:
    static void InitScreenBuffer();
    static void BufferizeScreenPixels();
    static const Color& ReadColorFromBuffer(int x, int y);

private:

    static Color ReadPixel(int x, int y);

    static std::vector<Color> ColorBuffer;
    static int Width;
    static int Height;
};


#endif //LINE_CLIPPING_ALGORITHM_SCREENUTILS_H
