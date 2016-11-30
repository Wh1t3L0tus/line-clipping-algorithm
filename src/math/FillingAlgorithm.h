#ifndef LINE_CLIPPING_ALGORITHM_FILLINGALGORITHM_H
#define LINE_CLIPPING_ALGORITHM_FILLINGALGORITHM_H


#include "../utility/utils.h"
#include "Math.h"

class FillingAlgorithm {
public:
    static void FloodFill(int x, int y, const Color &edgeColor, const Color& fillColor, std::vector<Vertex> &filledRegion);

    static void BoundingBoxFill(const Vertex* vertex, unsigned int vertexCount, const Color& fillColor, std::vector<Vertex> & filledRegion);
};


#endif //LINE_CLIPPING_ALGORITHM_FILLINGALGORITHM_H
