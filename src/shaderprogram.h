#pragma once

#include <string>
#include <glad/gl.h>

class ShaderProgram {
public:
    ShaderProgram(const char* vertexSource, const char* fragmentSource);
    ~ShaderProgram();

    void use() const;
    unsigned int id() const;

    static bool checkCompileErrors(unsigned int shader, const std::string& type);

private:
    unsigned int programID;
};
