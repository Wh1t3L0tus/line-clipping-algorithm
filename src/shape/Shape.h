//
// Created by theo on 10/10/16.
//

#ifndef LINE_CLIPPING_ALGORITHM_SHAPE_H
#define LINE_CLIPPING_ALGORITHM_SHAPE_H


#include "../utility/utils.h"
#include <vector>

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
	Vertex* Shape::DupVertices();
	Vertex* ReverseVertices();
    int GetVertexCount() const;
    bool IsClosed() const;
	bool isClockwise() const;
	bool isConvex() const;

	void MakeCounterClockwise();

	//static Shape* ClipShape(Shape* window, Shape shape);
	static Shape* Shape::ClipShapes(const Shape& window, const Shape &shape);
	static std::vector<Shape*> Triangulate(const Shape *window);

private:
    Vertex* vertices;
    int vertexCount;
    bool isClosed;

};


#endif //LINE_CLIPPING_ALGORITHM_SHAPE_H
