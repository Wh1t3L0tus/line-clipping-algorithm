//
// Created by theo on 10/9/16.
//

#ifndef LINE_CLIPPING_ALGORITHM_SHADER_H
#define LINE_CLIPPING_ALGORITHM_SHADER_H

#include <cstdint>

class Shader {

public:
    Shader(){}
    ~Shader(){}

    bool LoadVertexShader(const char* filename);
    bool LoadFragmentShader(const char* filename);
    uint32_t CompileShader(uint32_t type, const char* code);

    bool CreateProgram();
    void DestroyProgram();
    void GetCompilationLog(uint32_t shaderObject, uint32_t shaderType);

    inline uint32_t GetProgram() { return m_ProgramObject; }

private:
    uint32_t m_ProgramObject;
    uint32_t m_VertexShader;
    uint32_t m_FragmentShader;
};

#endif //LINE_CLIPPING_ALGORITHM_SHADER_H
