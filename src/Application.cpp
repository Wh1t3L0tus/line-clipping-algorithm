//
// Created by theo on 10/9/16.
//

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "Application.h"


Application::Application() {
}

Application::~Application() {
    if (shapeVertices != nullptr) {
        delete [] shapeVertices;
    }

    if (windowVertices != nullptr) {
        delete [] windowVertices;
    }

    shader.DestroyProgram();
}

void Application::Init(int width, int height, int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(200, 100);
    glutInitWindowSize(width, height);
    glutCreateWindow("Line clipping algorithm demo");
    glewInit();

    glViewport(0, 0, width, height);
    this->width = width;
    this->height = height;
    windowVertices = nullptr;
    shapeVertices = nullptr;
    windowVertexCount = 0;
    shapeVertexCount = 0;
    appState = ApplicationState::SHAPE_EDIT;

    shader.LoadFragmentShader("./assets/shader/basic.fs.glsl");
    shader.LoadVertexShader("./assets/shader/basic.vs.glsl");
    shader.CreateProgram();
}


void Application::RenderShape() {
    if (shapeVertices != nullptr) {
        int colorId = glGetUniformLocation(shader.GetProgram(), "color");
        glUseProgram(shader.GetProgram());
        glUniform4f(colorId, 1.0f, 0.0f, 0.0f, 1.0f);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, shapeVertices);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_LINE_STRIP, 0, shapeVertexCount);
        glDisableVertexAttribArray(0);
        glUseProgram(0);
    }

    if (windowVertices != nullptr) {
        int colorId = glGetUniformLocation(shader.GetProgram(), "color");
        glUseProgram(shader.GetProgram());
        glUniform4f(colorId, 0.0f, 0.0f, 1.0f, 1.0f);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, windowVertices);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_LINE_STRIP, 0, windowVertexCount);
        glDisableVertexAttribArray(0);
        glUseProgram(0);
    }
}


void Application::Update() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    RenderShape();

    glutSwapBuffers();
}

float lerp(float t, float start, float target) {
    return (1 - t) * start + t * target;
}

Vertex* addVertex(const Vertex& vertex, Vertex* verticesList, int& listLength) {
    Vertex* newList = new Vertex[listLength + 1];

    for (int i = 0; i < listLength; i++) {
        newList[i] = verticesList[i];
    }
    newList[listLength] = vertex;
    listLength++;

    if (verticesList != nullptr) {
        delete [] verticesList;
    }

    return newList;
}

void Application::OnMouseClick(int button, int state, int mouseX, int mouseY) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {

        Vertex click{lerp(mouseX / (float)width, -1.0f, 1.0f), -lerp(mouseY / (float)height, -1.0f, 1.0f)};
        if (appState == ApplicationState::SHAPE_EDIT) {
            shapeVertices = addVertex(click, shapeVertices, shapeVertexCount);
        }
        else if (appState == ApplicationState::WINDOW_EDIT) {
            windowVertices = addVertex(click, windowVertices, windowVertexCount);
        }
    }
}

void Application::OnKeyboardStroke(unsigned char key, int mouseX, int mouseY) {

    if (key == 's') {
        if (shapeVertices != nullptr) {
            delete [] shapeVertices;
            shapeVertices = nullptr;
            shapeVertexCount = 0;
        }
        appState = ApplicationState ::SHAPE_EDIT;
    }
    else if (key == 'w') {
        if (windowVertices != nullptr) {
            delete [] windowVertices;
            windowVertices = nullptr;
            windowVertexCount = 0;
        }
        appState = ApplicationState ::WINDOW_EDIT;
    }
    else if (key == 27) {
        glutLeaveMainLoop();
    }

}

