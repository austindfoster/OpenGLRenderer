#include <texture.h>

#include <glad/glad.h>
#include <stb/stb_image.h>

#include <iostream>

Texture::Texture(const char *path, bool hasAlpha)
{
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

    if (data)
    {
        int colorMode = GL_RGB;
        if (hasAlpha) {
            colorMode = GL_RGBA;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, colorMode, width, height, 0, colorMode,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, ID);
}