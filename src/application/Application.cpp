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
    appState = ApplicationState::WINDOW_EDIT;

    shader.LoadFragmentShader("./assets/shader/basic.fs.glsl");
    shader.LoadVertexShader("./assets/shader/basic.vs.glsl");
    shader.CreateProgram();

    window.SetColor(Color{0, 0, 255});
    clippedShape.SetColor(Color{0, 255, 0});
    object.SetColor(Color{255, 0, 0});
}


void Application::Update() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    window.Draw(shader);
    if (!clippedShape.IsClosed()) {
        object.Draw(shader);
    }
    clippedShape.Draw(shader);
    testShape.Draw(shader);

    glutSwapBuffers();
}

void Application::OnMouseClick(int button, int state, int mouseX, int mouseY) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

        clippedShape.Reset();

        Vec2 click{fLerp(mouseX / (float) width, -1.0f, 1.0f), -fLerp(mouseY / (float) height, -1.0f, 1.0f)};
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
        clippedShape.Reset();
        appState = ApplicationState::SHAPE_EDIT;
    }
    else if (key == 'w') {
        window.Reset();
        clippedShape.Reset();
        appState = ApplicationState::WINDOW_EDIT;
    }
    else if (key == 'r') {
        appState = ApplicationState::RENDER_RESULT;
        if (window.IsClosed() && window.GetVertexCount() >= 3 && object.IsClosed() && object.GetVertexCount() >= 3) {
            Shape::ClipShapes(window, object, clippedShape);
        }
    }
    else if (key == 'S') {
        object.ToggleCloseLine();
    }
    else if (key == 'W') {
        window.ToggleCloseLine();
    }
    else if (key == 'f') {
        window.FillShape();
    }
    else if (key == 'F') {
        window.Clear();
    }
    else if (key == 27) {
        glutLeaveMainLoop();
    }

}

