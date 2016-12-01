//
// Created by theo on 10/10/16.
//

#include <GL/glew.h>
#include <vector>
#include <cmath>
#include <cstdio>
#include <iostream>


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

const Vertex* Shape::ReverseVertices() const {
	Vertex* reversed = new Vertex[vertexCount];
	for (int i = 0; i < vertexCount; i++)
	{
		reversed[i] = vertices[vertexCount - i - 1];
	}
	return reversed;
}

int Shape::GetVertexCount() const {
    return vertexCount;
}

bool Shape::IsClosed() const {
    return isClosed;
}

bool Shape::isClockwise() const {
	double sum = 0.0;
	for (int i = 0; i < vertexCount; i++) {
		Vertex v1 = vertices[i];
		Vertex v2 = vertices[(i + 1) % vertexCount]; // % is the modulo operator
		sum += (v2.x - v1.x) * (v2.y + v1.y);
	}
	return sum > 0.0; // Clockwise if true
}
bool Shape::isConvex() const {
	if (vertexCount < 4)
	{
		return true;
	}
	bool sign = false;
	for (int i = 0; i < vertexCount; i++)
	{
		Vertex v1 = vertices[i % vertexCount];
		Vertex v2 = vertices[(i + 1) % vertexCount];
		Vertex v3 = vertices[(i + 2) % vertexCount];
		float dx1, dy1, dx2, dy2, crossP;
		dx1 = v2.x - v1.x;
		dy1 = v2.y - v1.y;
		dx2 = v3.x - v2.x;
		dy2 = v3.y - v2.y;
		crossP = dx1*dy2 - dy1*dx2;
		if (i == 0)
		{
			sign = crossP > 0;
		}
		else
		{
			if (sign != (crossP > 0)) // Si le Produit en croix est positif le coin testé est concave
			{
				return false;
			}
		}
	}
	return true;
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
	// Methode pour inverser l'ordre des points si ils ont été entrés dans le mauvais sens (sens horaire)
	static bool checkOrder = window.isClockwise();
	if (checkOrder) // Fenêtre entrée dans le mauvais sens (horaire)
	{
		windowVertices = window.ReverseVertices(); // inverse l'ordre des vertex de la shape
		std::cout << "Fenetre inverse car dans le sens horaire \n";
	}
	else
	{
		std::cout << "Fenetre dans le sens anti-horaire \n";
	}
	// Methode pour vérifier si la fenêtre est convexe
	if (!window.isConvex())
	{
		std::cout << "Fenetre CONCAVE \n";
	}
	else
	{
		std::cout << "Fenetre CONVEXE \n";
	}



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