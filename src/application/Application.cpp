//
// Created by theo on 10/9/16.
//

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Application.h"

Application::Application() {
}

Application::~Application() {
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
    appState = ApplicationState::SHAPE_EDIT;

    shader.LoadFragmentShader("./assets/shader/basic.fs.glsl");
    shader.LoadVertexShader("./assets/shader/basic.vs.glsl");
    shader.CreateProgram();
}


void Application::Update() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    window.Draw(shader, Color{0.0f, 0.0f, 1.0f, 1.0f});
    object.Draw(shader, Color{1.0f, 0.0f, 0.0f, 1.0f});

    glutSwapBuffers();
}

void Application::OnMouseClick(int button, int state, int mouseX, int mouseY) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

        Vertex click{lerp(mouseX / (float)width, -1.0f, 1.0f), -lerp(mouseY / (float)height, -1.0f, 1.0f)};
        if (appState == ApplicationState::SHAPE_EDIT) {
            object.AddVertex(click);
        }
        else if (appState == ApplicationState::WINDOW_EDIT) {
            window.AddVertex(click);
        }
    }
}

void Application::OnKeyboardStroke(unsigned char key, int mouseX, int mouseY) {

    if (key == 's') {
        object.Reset();
        appState = ApplicationState ::SHAPE_EDIT;
    }
    else if (key == 'w') {
        window.Reset();
        appState = ApplicationState ::WINDOW_EDIT;
    }
    else if (key == 'S') {
        object.ToggleCloseLine();
    }
    else if (key == 'W') {
        window.ToggleCloseLine();
    }
    else if (key == 27) {
        glutLeaveMainLoop();
    }

}

