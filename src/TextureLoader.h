#pragma once

#include <string>
#include <glad/gl.h>

class TextureLoader {
public:
    static unsigned int LoadTexture(const std::string& path, int targetWidth = 256, int targetHeight = 256);
    static void UnloadTexture(unsigned int textureID);
};