//
// Created by theo on 10/10/16.
//

#ifndef LINE_CLIPPING_ALGORITHM_SHAPE_H
#define LINE_CLIPPING_ALGORITHM_SHAPE_H


#include "../utility/utils.h"

class Shader;

class Shape {
public:

    Shape();
    ~Shape();

    void Draw(const Shader& shader, const Color& color);
    void AddVertex(const Vertex& vertex);
    void ToggleCloseLine();
    void Reset();

    const Vertex* GetVertices() const;
	const Vertex* ReverseVertices() const;
    int GetVertexCount() const;
    bool IsClosed() const;
	bool isClockwise() const;

    static void ClipShapes(const Shape& window, const Shape &shape, Shape &outputShape);

private:
    Vertex* vertices;
    int vertexCount;
    bool isClosed;

};


#endif //LINE_CLIPPING_ALGORITHM_SHAPE_H
