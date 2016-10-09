#include <GL/glew.h>
#include <GL/freeglut.h>


void animate() {
    glViewport(0, 0, 800, 600);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glutSwapBuffers();
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(200, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Line clipping algorithm demo");
    glewInit();

    glutDisplayFunc(animate);
    glutMainLoop();

    return 0;
}