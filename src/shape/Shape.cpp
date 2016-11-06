//
// Created by theo on 10/10/16.
//

#include <GL/glew.h>
#include <vector>
#include <cmath>

#include "Shape.h"
#include "../shader/Shader.h"
#include "../math/Math.h"

using namespace std;

Shape::Shape() : vertices(nullptr), vertexCount(0), isClosed(false) {
}

Shape::~Shape() {
    Reset();
}

void Shape::Draw(const Shader& shader, const Color& color) {

    if (vertices != nullptr) {
        int colorId = glGetUniformLocation(shader.GetProgram(), "color");
        glUseProgram(shader.GetProgram());
        glUniform4f(colorId, color.r, color.g, color.b, color.a);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
        glEnableVertexAttribArray(0);
        if (isClosed) {
            glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
        }
        else {
            glDrawArrays(GL_LINE_STRIP, 0, vertexCount);
        }
        glDisableVertexAttribArray(0);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
        glEnableVertexAttribArray(0);
        glPointSize(4);
        glDrawArrays(GL_POINTS, 0, vertexCount);
        glPointSize(1);
        glDisableVertexAttribArray(0);

        glUseProgram(0);
    }
}

void Shape::AddVertex(const Vertex& vertex) {

    Vertex* newList = new Vertex[vertexCount + 1];

    for (int i = 0; i < vertexCount; i++) {
        newList[i] = vertices[i];
    }
    newList[vertexCount] = vertex;
    vertexCount++;

    if (vertices != nullptr) {
        delete [] vertices;
    }

    vertices = newList;
}

void Shape::ToggleCloseLine() {
    isClosed = !isClosed;
}

void Shape::Reset() {
    if (vertices != nullptr) {
        delete [] vertices;
        vertices = nullptr;
    }
    vertexCount = 0;
    isClosed = false;
}

const Vertex* Shape::GetVertices() const {
    return vertices;
}

int Shape::GetVertexCount() const {
    return vertexCount;
}

bool Shape::IsClosed() const {
    return isClosed;
}

Vertex* copyVertices(const vector<Vec2> &list) {

    if (list.size() == 0) {
        return nullptr;
    }

    Vertex* array = new Vertex[list.size()];
    for (int i = 0; i < list.size(); i++) {
        array[i] = Vertex{list[i].x, list[i].y};
    }

    return array;
}

Vertex* copyVertices(const Vertex* list, int size) {
    Vertex* array = new Vertex[size];
    for (int i = 0; i < size; i++) {
        array[i] = list[i];
    }

    return array;
}

void Shape::ClipShapes(const Shape& window, const Shape &shape, Shape &outputShape) {

    vector<Vec2> outputVertices;

    int windowVertexCount = window.GetVertexCount();
    const Vertex* windowVertices = window.GetVertices();

    int shapeVertexCount = shape.GetVertexCount();
    Vertex* shapeVertices = copyVertices(shape.GetVertices(), shapeVertexCount);

    vector<Segment> windowSegments = Math::getSegmentsFromVertices(windowVertexCount, windowVertices);

    for (int i = 0; i < windowSegments.size(); i++) {
        outputVertices.clear();

        Vertex lastVertex{NAN, NAN};
        Vertex currentVertex{NAN, NAN};
        for (int j = 0; j < shapeVertexCount; j++) {
            if (j == 0) {
                lastVertex = shapeVertices[j];
            }
            else {
                IntersectionResult intersectionResult = Math::getIntersection(Math::makeSegment(currentVertex, shapeVertices[j]), windowSegments[i]);
                if (intersectionResult.isIntersecting) {
                    outputVertices.push_back(intersectionResult.intersection);
                }
            }

            currentVertex = shapeVertices[j];

            Vec2 point = Vec2{currentVertex.x, currentVertex.y};
            if (Math::isPointVisible(point, windowSegments[i])) {
                outputVertices.push_back(point);
            }
        }

        if (outputVertices.size() > 0) {
            IntersectionResult intersectionResult = Math::getIntersection(Math::makeSegment(currentVertex, lastVertex), windowSegments[i]);
            if (intersectionResult.isIntersecting) {
                outputVertices.push_back(intersectionResult.intersection);
            }
        }

        Vertex* toDelete = shapeVertices;
        shapeVertices = copyVertices(outputVertices);
        shapeVertexCount = int(outputVertices.size());

        delete [] toDelete;
    }

    delete [] shapeVertices;

    // We now have all our vertices for the clipped shape
    outputShape.Reset();
    for (Vec2 vec2: outputVertices) {
        outputShape.AddVertex(Vertex{vec2.x, vec2.y});
    }
    outputShape.ToggleCloseLine();
}