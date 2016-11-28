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
	InitMenu();

    glViewport(0, 0, width, height);
    this->width = width;
    this->height = height;
    appState = ApplicationState::WINDOW_EDIT;

    shader.LoadFragmentShader("./assets/shader/basic.fs.glsl");
    shader.LoadVertexShader("./assets/shader/basic.vs.glsl");
    shader.CreateProgram();
}


void Application::Update() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    window.Draw(shader, Color{0.0f, 0.0f, 1.0f, 1.0f});

    if (!clippedShape.IsClosed()) {
        object.Draw(shader, Color{1.0f, 0.0f, 0.0f, 1.0f});
    }

    clippedShape.Draw(shader, Color{0.0f, 1.0f, 0.0f, 1.0f});

    glutSwapBuffers();
}

void Application::OnMouseClick(int button, int state, int mouseX, int mouseY) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

        clippedShape.Reset();

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
    else if (key == 27) {
        glutLeaveMainLoop();
    }

}

void InitMenu()
{
	// Création du menu
	int menu = glutCreateMenu(menu_Selection);

	// Index des sous-menus
	GLint shapeMenu, windowMenu, clipMenu, colorMenu, fillMenu;

	shapeMenu = glutCreateMenu(shape_Menu);
	glutAddMenuEntry("New Shape", 1);
	glutAddMenuEntry("Close Shape", 2);
	glutAddMenuEntry("Clear Shape", 3);

	windowMenu = glutCreateMenu(window_Menu);
	glutAddMenuEntry("New Window", 1);
	glutAddMenuEntry("Close Window", 2);
	glutAddMenuEntry("Clear Window", 3);

	colorMenu = glutCreateMenu(color_Menu);
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Blue", 2);
	glutAddMenuEntry("Green", 3);
	glutAddMenuEntry("Black", 4);
	glutAddMenuEntry("Yellow", 5);

	glutCreateMenu(menu_Selection);
	glutAddSubMenu("Shape", shapeMenu);
	glutAddSubMenu("Window", shapeMenu);
	glutAddMenuEntry("Clip Shape", 1);
	glutAddSubMenu("Color", colorMenu);
	glutAddMenuEntry("Fill Selection", 2);
	glutAddMenuEntry("Quit", 3);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void menu_Selection(int option)
{
	switch (option)
	{
	case 1:
		//Clipping
		break;
	case 2:
		//Filling
		break;
	case 3:
		exit(0);
		break;
	}
}

void shape_Menu(int option)
{
	switch (option)
	{
	case 1:
		//New Shape
		break;
	case 2:
		//Close Shape
		break;
	case 3:
		//Clear Shape
		break;
	}
}

void window_Menu(int option)
{
	switch (option)
	{
	case 1:
		//New Window
		break;
	case 2:
		//Close Window
		break;
	case 3:
		//Clear Window
		break;
	}
}

void color_Menu(int option)
{
	switch (option)
	{
	case 1:
		//Red
		break;
	case 2:
		//Blue
		break;
	case 3:
		//Green
		break;
	case 4:
		//Black
		break;
	case 5:
		//Yellow
		break;
	}
}




