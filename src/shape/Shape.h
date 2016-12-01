//
// Created by theo on 10/10/16.
//

#ifndef LINE_CLIPPING_ALGORITHM_SHAPE_H
#define LINE_CLIPPING_ALGORITHM_SHAPE_H


#include "../utility/utils.h"
#include "../math/Math.h"

class Shader;
struct Vec2;

class Shape {
public:

    Shape();
    ~Shape();

    void Draw(const Shader& shader);
    void AddVertex(const Vec2& vertex);
    void ToggleCloseLine();
    void Reset();

    const Vec2* GetVertices() const;
    unsigned int GetVertexCount() const;
    bool IsClosed() const;

    void SetColor(const Color& color);
    void FillShape();
    void Clear();

    static void ClipShapes(const Shape& window, const Shape &shape, Shape &outputShape);

private:
    Color shapeColor;
    Vec2* vertices;
    int vertexCount;
    bool isClosed;

    Vec2* filledAreaVertices;
    int filledAreaVertexCount;
};


#endif //LINE_CLIPPING_ALGORITHM_SHAPE_H
