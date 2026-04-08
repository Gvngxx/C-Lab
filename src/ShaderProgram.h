#pragma once

#include <string>
#include <glad/gl.h>

class ShaderProgram {
public:
    ShaderProgram(const char* vertexSource, const char* fragmentSource);
    ~ShaderProgram();

    void bind() const;
    void unbind() const;
    unsigned int id() const;

    void setFloat(const std::string& name, float value) const;
    void setInt(const std::string& name, int value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setMat4(const std::string& name, const float* mat) const;

    void CleanUp();

    static bool checkCompileErrors(unsigned int shader, const std::string& type);

private:
    unsigned int programID;
};
