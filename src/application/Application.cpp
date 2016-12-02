//
// Created by theo on 10/9/16.
//

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "Application.h"

enum ApplicationState {
	WINDOW_EDIT,
	SHAPE_EDIT,
	RENDER_RESULT,
	FILL_EDIT
};

int width;
int height;

bool showShape = true;
bool showClipping = false;

std::vector<Shape*> objects;
std::vector<Shape*> clippedShapes;
Shape* window = new Shape();
Shape* object = new Shape();
//Shape clippedShape;

Shader shader;

Color FillColor = { 1.0f, 0.0f, 0.0f, 1.0f };

ApplicationState appState;

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

    window->Draw(shader, Color{0.0f, 0.0f, 1.0f, 1.0f});

    if (showShape) 
	{
		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->Draw(shader, Color{ 1.0f, 0.0f, 0.0f, 1.0f });
		}
		object->Draw(shader, Color{ 1.0f, 0.0f, 0.0f, 1.0f });
    }

	if (showClipping)
	{
		for (int i = 0; i < clippedShapes.size(); i++)
		{
			clippedShapes[i]->Draw(shader, Color{ 0.0f, 1.0f, 0.0f, 1.0f });
		}
	}
	
    glutSwapBuffers();
}

void Application::OnMouseClick(int button, int state, int mouseX, int mouseY) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

        Vertex click{lerp(mouseX / (float)width, -1.0f, 1.0f), -lerp(mouseY / (float)height, -1.0f, 1.0f)};
        if (appState == ApplicationState::SHAPE_EDIT) {
            object->AddVertex(click);
        }
        else if (appState == ApplicationState::WINDOW_EDIT) {
            window->AddVertex(click);
        }
		else if((appState == ApplicationState::FILL_EDIT))
		{

		}
    }
}

void Application::OnKeyboardStroke(unsigned char key, int mouseX, int mouseY) {

    if (key == 's') {
        object->Reset();
		showClipping = false;
		showShape = true;
        appState = ApplicationState::SHAPE_EDIT;
    }
    else if (key == 'w') {
        window->Reset();
		showClipping = false;
		showShape = true;
        appState = ApplicationState::WINDOW_EDIT;
    }
    else if (key == 'r') {
        appState = ApplicationState::RENDER_RESULT;
        if (window->IsClosed() && window->GetVertexCount() >= 3 && objects.size()>0) 
		{
            Shape::ClipShapes(*window, *object);
        }
		showClipping = true;
		showShape = false;
    }
    else if (key == 'S') {
        object->ToggleCloseLine();
    }
    else if (key == 'W') {
        window->ToggleCloseLine();
    }
    else if (key == 27) {
        glutLeaveMainLoop();
    }

}

void PushInTab()
{
	std::vector<Shape*> clipped = Shape::ClipShapes(*window, *objects[0]);
	Shape* test = new Shape();
	for (int i = 0; i < clipped.size(); i++)
	{
		test = clipped[i];
		clippedShapes.push_back(test);
		test = new Shape();
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

	clipMenu = glutCreateMenu(clip_Menu);
	glutAddMenuEntry("Clip Shape", 1);
	glutAddMenuEntry("Clear Clipping", 2);

	glutCreateMenu(menu_Selection);
	glutAddSubMenu("Shape", shapeMenu);
	glutAddSubMenu("Window", windowMenu);
	glutAddSubMenu("Clip Shape", clipMenu);
	glutAddSubMenu("Color", colorMenu);
	glutAddMenuEntry("Fill Selection", 2);
	glutAddMenuEntry("Quit", 3);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void menu_Selection(int option)
{
	switch (option)
	{
	case 2:
		//Filling
		appState = ApplicationState::FILL_EDIT;
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
		appState = ApplicationState::SHAPE_EDIT;
		showClipping = false;
		showShape = true;
		break;
	case 2:
		//Close Shape
		object->ToggleCloseLine();
		objects.push_back(object);
		object = new Shape();
		break;
	case 3:
		//Clear Shape
		object->Reset();
		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->Reset();
		}
		break;
	}
}

void window_Menu(int option)
{
	switch (option)
	{
	case 1:
		//New Window
		appState = ApplicationState::WINDOW_EDIT;
		showClipping = false;
		showShape = true;
		window->Reset();
		break;
	case 2:
		//Close Window
		window->ToggleCloseLine();
		break;
	case 3:
		//Clear Window
		window->Reset();
		break;
	}
}

void clip_Menu(int option)
{
	switch (option)
	{
	case 1:
		//Clipping
		showClipping = true;
		showShape = false;
		object->Reset();
		appState = ApplicationState::RENDER_RESULT;
		PushInTab();
		//clippedShapes = Shape::ClipShapes(*window, *object);
		break;
	case 2:
		//Clear Clipping
		showClipping = false;
		showShape = true;
		for (int i = 0; i < clippedShapes.size(); i++)
		{
			clippedShapes[i]->Reset();
		}
		break;
	}
}

void color_Menu(int option)
{
	switch (option)
	{
	case 1:
		//Red
		FillColor = { 1.0f, 0.0f, 0.0f, 1.0f };
		break;
	case 2:
		//Blue
		FillColor = { 0.0f, 0.0f, 1.0f, 1.0f };
		break;
	case 3:
		//Green
		FillColor = { 0.0f, 1.0f, 0.0f, 1.0f };
		break;
	case 4:
		//Black
		FillColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		break;
	case 5:
		//Yellow
		FillColor = { 1.0f, 1.0f, 0.0f, 1.0f };
		break;
	}
}




