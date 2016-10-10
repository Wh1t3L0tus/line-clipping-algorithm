#include <GL/freeglut.h>
#include <iostream>
#include "Application.h"

static Application app;

void display() {
    app.Update();
}

void keyboard(unsigned char key, int x, int y) {
    app.OnKeyboardStroke(key, x, y);
}

void mouse(int button, int state, int x, int y) {
    app.OnMouseClick(button, state, x, y);
}

int main(int argc, char** argv) {

    app.Init(800, 600, argc, argv);

    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}