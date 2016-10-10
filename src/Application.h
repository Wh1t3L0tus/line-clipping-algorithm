//
// Created by theo on 10/9/16.
//

#ifndef LINE_CLIPPING_ALGORITHM_APPLICATION_H
#define LINE_CLIPPING_ALGORITHM_APPLICATION_H

#include "shader/Shader.h"

enum ApplicationState {
    WINDOW_EDIT,
    SHAPE_EDIT,
    RENDER_RESULT
};

struct Vertex {
    float x;
    float y;
};

class Application {

public:
    Application();
    ~Application();

    void Init(int width, int height, int argc, char **argv);

    void RenderShape();

    void Update();

    void OnMouseClick(int button, int state, int mouseX, int mouseY);
    void OnKeyboardStroke(unsigned char key, int mouseX, int mouseY);

private:
    int width;
    int height;

    ApplicationState appState;

    Vertex* windowVertices;
    Vertex* shapeVertices;

    int windowVertexCount;
    int shapeVertexCount;

    Shader shader;
};


#endif //LINE_CLIPPING_ALGORITHM_APPLICATION_H
