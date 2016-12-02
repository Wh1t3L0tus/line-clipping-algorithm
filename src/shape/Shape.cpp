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

Vertex* Shape::DupVertices() {
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
		Vertex v2 = vertices[(i + 1) % vertexCount];
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

Shape* ClipShape(Shape* window, Shape &shape)
{
	vector<Vec2> outputVertices;

	int windowVertexCount = window->GetVertexCount();
	const Vertex* windowVertices = window->GetVertices();

	int shapeVertexCount = shape.GetVertexCount();
	Vertex* shapeVertices = copyVertices(shape.GetVertices(), shapeVertexCount);

	vector<Segment> windowSegments = Math::getSegmentsFromVertices(windowVertexCount, windowVertices);

	for (int i = 0; i < windowSegments.size(); i++) {
		//outputVertices.clear();

		Vertex lastVertex{ NAN, NAN };
		Vertex currentVertex{ NAN, NAN };
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

			Vec2 point = Vec2{ currentVertex.x, currentVertex.y };
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

		delete[] toDelete;
	}

	delete[] shapeVertices;

	// We now have all our vertices for the clipped shape
	Shape* output = new Shape();
	for (Vec2 vec2 : outputVertices) 
	{
		output->AddVertex(Vertex{ vec2.x, vec2.y });
	}
	output->ToggleCloseLine();

	return output;
}


vector<Shape*> Shape::ClipShapes(Shape& window, Shape &shape) {

	vector<Shape*> clippedShapes;

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
		vector<Shape*> Tris = Triangulate(window);
		for (int i = 0; i < Tris.size(); i++)
		{
			clippedShapes.push_back(ClipShape(Tris[i], shape));
		}
	}
	else
	{
		std::cout << "Fenetre CONVEXE \n";

		clippedShapes.push_back(ClipShape(&window, shape));
	}

	return clippedShapes;
}

bool differentVertex(Vertex a, Vertex b)
{
	if ((a.x != b.x) && (a.y != b.y))
	{
		return true;
	}
	return false;
}

bool isVertexInTriangle(Vertex cur, Vertex prev, Vertex next, Vertex* tab, int count) {	//Permet de connaitre si un des vertices d'une Shape est dans un triangle

	for (int j = 0; j < count; j++) {
		Vertex vtx = tab[j]; // Vertex testé comme interne ou externe au triangle
		Vec2 point{ tab[j].x, tab[j].y };
		Segment segment1{ cur.x, cur.y, next.x, next.y };
		Segment segment2{ next.x, next.y, prev.x, prev.y };
		Segment segment3{ prev.x, prev.y, cur.x, cur.y };
		if (Math::isPointVisible(point, segment1) && Math::isPointVisible(point, segment2) && Math::isPointVisible(point, segment3)) {
			if (differentVertex(vtx, cur) && differentVertex(vtx, next) && differentVertex(vtx, prev)) { //Si le point testé ne fait pas parti des points formant le triangle
				return true;
			}
		}
	}
	return false;
}

vector<Shape*> Shape::Triangulate(Shape &window) 
{
	
	Vertex cur, prev, next;

	int nbTri = 0;

	Vertex*  win = window.DupVertices();
	int sizeWin = window.GetVertexCount();

	vector<Shape*> miniWindows;
	int leftPoints = sizeWin;

	int i = 0;

	while (leftPoints >= 3) {

		Shape* newTriangle = new Shape();

		cur = win[i];
		if (i == 0) {
			prev = win[leftPoints - 1];
			next = win[i + 1];
		}
		else if (i == leftPoints - 1) {
			prev = win[leftPoints - 2];
			next = win[0];
		}
		else {
			prev = win[i - 1];
			next = win[i + 1];
		}
		newTriangle->Reset();
		newTriangle->AddVertex(cur);
		newTriangle->AddVertex(next);
		newTriangle->AddVertex(prev);

		if (newTriangle->isClockwise())// Si le triangle est concave ( sens horaire )
		{
			std::cout << "Clockwise with i = " << i << std::endl;
			i++;	
		}
		else // Triangle convex alors on test la présence d'un point à l'intérieur
		{
			if (isVertexInTriangle(cur, prev, next, win, leftPoints))
			{
				std::cout << "Point inside with i = " << i << std::endl;
				i++;
			}
			else // Pas de points à l'intérieur donc bon pour le découpage en triangle séparé
			{
				newTriangle->ToggleCloseLine();
				miniWindows.push_back(newTriangle);
				leftPoints -= 1;
				Vertex* newList = new Vertex[leftPoints];

				int g = 0;
				for (int j = 0; j < leftPoints + 1; j++)
				{
					//std::cout << " j : " << j << " | leftPoints : " << leftPoints << std::endl;
					
					if (j != i)
					{
						newList[g].x = win[j].x;
						newList[g].y = win[j].y;
						//std::cout << " x : " << newList[g].x << " y : " << newList[g].y << std::endl;
						g++;

					}

				}
				if (win != nullptr)
				{
					delete[] win;
				}

				win = newList;
				//test
				
				std::cout << "i = " << i << std::endl;
				Vertex* test = miniWindows[nbTri]->DupVertices();
				for (int t = 0; t < miniWindows[nbTri]->GetVertexCount(); t++)
				{
					std::cout << "Triangle " << nbTri << " || " << " x : " << test[t].x << " y : " << test[t].y << std::endl;
				}
				
				nbTri++;
				i = 0;
			}
			
		}
	}
	
	std::cout << "Nombre de triangles : " << nbTri << std::endl;
	
	for (int k = 0; k < nbTri; k++)
	{
		std::cout << "Triangle " << k << " : " << std::endl;
		for (int j = 0; j < miniWindows[k]->GetVertexCount(); j++)
		{
			std::cout << " x : " << miniWindows[k]->DupVertices()[j].x << " y : " << miniWindows[k]->DupVertices()[j].y << std::endl;
		}
	}
	
	return miniWindows;
}
