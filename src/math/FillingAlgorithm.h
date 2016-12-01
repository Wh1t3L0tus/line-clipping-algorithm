#ifndef LINE_CLIPPING_ALGORITHM_FILLINGALGORITHM_H
#define LINE_CLIPPING_ALGORITHM_FILLINGALGORITHM_H


#include "../utility/utils.h"
#include "Math.h"

class FillingAlgorithm {
public:
    static void BoundingBoxFill(const Vec2* vertex, unsigned int vertexCount, const Color& fillColor, std::vector<Vec2>& filledRegion);
};


#endif //LINE_CLIPPING_ALGORITHM_FILLINGALGORITHM_H
