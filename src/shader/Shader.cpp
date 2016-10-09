//
// Created by theo on 10/9/16.
//

#include <GL/glew.h>
#include <cstdio>
#include <iostream>
#include "Shader.h"

using namespace std;

void Shader::DestroyProgram() {
    glDetachShader(m_ProgramObject, m_VertexShader);
    glDetachShader(m_ProgramObject, m_FragmentShader);
    glDeleteProgram(m_ProgramObject);
    glDeleteShader(m_VertexShader);
    glDeleteShader(m_FragmentShader);
}

bool Shader::CreateProgram() {
    m_ProgramObject = glCreateProgram();
    glAttachShader(m_ProgramObject, m_VertexShader);
    glAttachShader(m_ProgramObject, m_FragmentShader);
    glLinkProgram(m_ProgramObject);

    return true;
}

uint32_t Shader::CompileShader(uint32_t type, const char *code) {

    uint32_t shaderObject = glCreateShader(type);

    if (shaderObject != 0) {
        glShaderSource(shaderObject, 1, &code, nullptr);
        glCompileShader(shaderObject);
        GetCompilationLog(shaderObject, type);
    }
    return shaderObject;
}

bool Shader::LoadVertexShader(const char *filename) {
    FILE* file = fopen(filename,"rb");
    fseek(file, 0, SEEK_END);
    auto len = ftell(file);
    rewind(file);
    auto buffer = new char[len+1];
    fread(buffer, len, 1, file);
    buffer[len] = '\0';
    m_VertexShader = CompileShader(GL_VERTEX_SHADER, buffer);

    delete[] buffer;

    return (m_VertexShader != 0);
}

bool Shader::LoadFragmentShader(const char *filename) {
    FILE* file = fopen(filename,"rb");
    fseek(file, 0, SEEK_END);
    auto len = ftell(file);
    rewind(file);
    auto buffer = new char[len+1];
    fread(buffer, len, 1, file);
    buffer[len] = '\0';
    m_FragmentShader = CompileShader(GL_FRAGMENT_SHADER, buffer);

    delete[] buffer;

    return (m_FragmentShader != 0);
}

void Shader::GetCompilationLog(uint32_t shaderObject, uint32_t shaderType) {
    GLint isCompilationOk;
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &isCompilationOk);
    string shaderTypeString;

    if (shaderType == GL_VERTEX_SHADER) {
        shaderTypeString = "vertex shader";
    }
    else if (shaderType == GL_FRAGMENT_SHADER) {
        shaderTypeString = "fragment shader";
    }

    if (isCompilationOk == GL_TRUE) {
        cout << "Successful " << shaderTypeString << " compilation" << endl;
    }
    else {
        cout << "Error during " << shaderTypeString << " compilation" << endl;
    }
}
