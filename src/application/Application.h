//
// Created by theo on 10/9/16.
//

#ifndef LINE_CLIPPING_ALGORITHM_APPLICATION_H
#define LINE_CLIPPING_ALGORITHM_APPLICATION_H

#include "../shader/Shader.h"
#include "../shape/Shape.h"

enum ApplicationState {
    WINDOW_EDIT,
    SHAPE_EDIT,
    RENDER_RESULT
};

struct Vertex;

class Application {

public:
    Application();
    ~Application();

    void Init(int width, int height, int argc, char **argv);

    void Update();

    void OnMouseClick(int button, int state, int mouseX, int mouseY);
    void OnKeyboardStroke(unsigned char key, int mouseX, int mouseY);

private:
    int width;
    int height;

    ApplicationState appState;

    Shape window;
    Shape object;

    Shader shader;
};


#endif //LINE_CLIPPING_ALGORITHM_APPLICATION_H
