//
// Created by theo on 10/10/16.
//

#include <GL/glew.h>
#include <vector>
#include <cmath>
#include <iostream>

#include "Shape.h"
#include "../shader/Shader.h"
#include "../math/FillingAlgorithm.h"

using namespace std;

Shape::Shape() : vertices(nullptr), vertexCount(0), isClosed(false), filledAreaVertices(nullptr), filledAreaVertexCount(0) {
}

Shape::~Shape() {
    Reset();
}

void Shape::Draw(const Shader& shader) {

    if (vertices != nullptr) {
        int colorId = glGetUniformLocation(shader.GetProgram(), "color");
        int positionLocation = glGetAttribLocation(shader.GetProgram(), "a_Position");

        glUseProgram(shader.GetProgram());
        glUniform3f(colorId, shapeColor.r / 255.0f, shapeColor.g / 255.0f, shapeColor.b / 255.0f);

        glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 0, vertices);
        glEnableVertexAttribArray(positionLocation);
        if (isClosed) {
            glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
        }
        else {
            glDrawArrays(GL_LINE_STRIP, 0, vertexCount);
        }
        glDisableVertexAttribArray(positionLocation);

        glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 0, vertices);
        glEnableVertexAttribArray(positionLocation);
        glPointSize(4);
        glDrawArrays(GL_POINTS, 0, vertexCount);
        glPointSize(1);
        glDisableVertexAttribArray(positionLocation);

        if (filledAreaVertices != nullptr) {
            glUseProgram(shader.GetProgram());
            glUniform3f(colorId, shapeColor.r / 255.0f, shapeColor.g / 255.0f, shapeColor.b / 255.0f);

            glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 0, filledAreaVertices);
            glEnableVertexAttribArray(positionLocation);
            glDrawArrays(GL_LINE, 0, filledAreaVertexCount);
            glEnableVertexAttribArray(positionLocation);
        }

        glUseProgram(0);
    }
}

void Shape::AddVertex(const Vec2& vertex) {

    Vec2* newList = new Vec2[vertexCount + 1];

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
    Clear();
    if (vertices != nullptr) {
        delete [] vertices;
        vertices = nullptr;
    }
    vertexCount = 0;
    isClosed = false;
}

const Vec2* Shape::GetVertices() const {
    return vertices;
}

unsigned int Shape::GetVertexCount() const {
    return vertexCount;
}

bool Shape::IsClosed() const {
    return isClosed;
}

Vec2* copyVertices(const vector<Vec2> &list) {

    if (list.size() == 0) {
        return nullptr;
    }

    Vec2* array = new Vec2[list.size()];
    for (int i = 0; i < list.size(); i++) {
        array[i] = Vec2{list[i].x, list[i].y};
    }

    return array;
}

Vec2* copyVertices(const Vec2* list, int size) {
    Vec2* array = new Vec2[size];
    for (int i = 0; i < size; i++) {
        array[i] = list[i];
    }

    return array;
}

void Shape::SetColor(const Color &color) {
    shapeColor = color;
}

void Shape::FillShape() {

    std::vector<Vec2> filledArea;
    FillingAlgorithm::BoundingBoxFill(GetVertices(), GetVertexCount(), shapeColor, filledArea);

    Clear();
    filledAreaVertexCount = int(filledArea.size());
    filledAreaVertices = new Vec2[filledAreaVertexCount];
    for (int i = 0; i < filledAreaVertexCount; i++) {
        filledAreaVertices[i] = filledArea[i];
    }
}

void Shape::FillShape2() {
    std::vector<Segment> filledArea;
    FillingAlgorithm::ScanLineFill(GetVertices(), GetVertexCount(), filledArea);
    filledAreaVertices = new Vec2[filledArea.size() * 2];
    for (int i = 0; i < filledArea.size(); i++) {
        filledAreaVertices[2 * i] = Vec2{filledArea[i].x1, filledArea[i].y1};
        filledAreaVertices[2 * i + 1] = Vec2{filledArea[i].x2, filledArea[i].y2};
    }
}

void Shape::Clear() {
    if (filledAreaVertices != nullptr) {
        filledAreaVertexCount = 0;
        delete[] filledAreaVertices;
        filledAreaVertices = nullptr;
    }
}

void Shape::ClipShapes(const Shape& window, const Shape &shape, Shape &outputShape) {

    vector<Vec2> outputVertices;

    int windowVertexCount = window.GetVertexCount();
    const Vec2* windowVertices = window.GetVertices();

    int shapeVertexCount = shape.GetVertexCount();
    Vec2* shapeVertices = copyVertices(shape.GetVertices(), shapeVertexCount);

    vector<Segment> windowSegments = Math::getSegmentsFromVertices(windowVertexCount, windowVertices);

    for (int i = 0; i < windowSegments.size(); i++) {
        outputVertices.clear();

        Vec2 lastVertex{NAN, NAN};
        Vec2 currentVertex{NAN, NAN};
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

        Vec2* toDelete = shapeVertices;
        shapeVertices = copyVertices(outputVertices);
        shapeVertexCount = int(outputVertices.size());

        delete [] toDelete;
    }

    delete [] shapeVertices;

    // We now have all our vertices for the clipped shape
    outputShape.Reset();
    for (Vec2 vec2: outputVertices) {
        outputShape.AddVertex(vec2);
    }
    outputShape.ToggleCloseLine();
}