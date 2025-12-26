#pragma once

class Texture
{
public:
    Texture(const char *path, bool hasAlpha = false);

    void bind();

    unsigned int ID;
};