#version 330

attribute vec2 a_Position;

void main(void) {
    gl_Position = vec4(a_Position, 0.0, 1.0);
}