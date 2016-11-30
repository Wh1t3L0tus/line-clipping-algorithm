//
// Created by theo on 10/10/16.
//

#ifndef LINE_CLIPPING_ALGORITHM_SHAPE_H
#define LINE_CLIPPING_ALGORITHM_SHAPE_H


#include "../utility/utils.h"
#include "../math/Math.h"

class Shader;

class Shape {
public:

    Shape();
    ~Shape();

    void Draw(const Shader& shader);
    void AddVertex(const Vertex& vertex);
    void ToggleCloseLine();
    void Reset();

    const Vertex* GetVertices() const;
    unsigned int GetVertexCount() const;
    bool IsClosed() const;

    void SetColor(const Color& color);
    void FillShape(const Color& color, const Vertex& mousePos);
    void FillShape();
    void Clear();

    static void ClipShapes(const Shape& window, const Shape &shape, Shape &outputShape);

private:
    Color shapeColor;
    Vertex* vertices;
    int vertexCount;
    bool isClosed;

    Vertex* filledAreaVertices;
    int filledAreaVertexCount;
};


#endif //LINE_CLIPPING_ALGORITHM_SHAPE_H
