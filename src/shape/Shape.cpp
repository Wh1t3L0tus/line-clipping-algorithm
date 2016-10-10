//
// Created by theo on 10/10/16.
//

#include <GL/glew.h>
#include "Shape.h"
#include "../shader/Shader.h"


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