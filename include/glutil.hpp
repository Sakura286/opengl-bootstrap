#ifndef GLUTIL_H
#define GLUTIL_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>
#include <GLES3/gl3.h>
#include "stb_image.h"
#include "static_methods.hpp"


namespace mygl
{
    enum COLOR_FORMAT {MYGL_RGBA, MYGL_RGB};
    // actually it is half-float and unsigned-int
    enum DATA_TYPE {MYGL_FLOAT, MYGL_INT};

    GLuint createTexture (const int width, const int height)
    {
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        return texture;
    }

    GLuint createTextureFromPic(const char* path,
            int & width, int & height, int & nrChannels,
            const enum COLOR_FORMAT color_format, const enum DATA_TYPE data_type, bool & tex_create_flag)
    {
        tex_create_flag = true;

        void* data;
        // load pic to mem
        if (data_type == MYGL_INT)
        {
            data = (void*) stbi_load(path, &width, &height, &nrChannels, 0);
        }
        else
        {
            stbi_ldr_to_hdr_scale(1.0f);
            stbi_ldr_to_hdr_gamma(1.0f);
            data = (void*) stbi_loadf(path, &width, &height, &nrChannels, 0);
        }

        if (!data)
        {
            std::cout << "Failed to load picture" << std::endl;
            tex_create_flag = false;
            return 0;
        }

        // create texture
        GLuint texture = createTexture(width, height);

        // transmit pic mem -> opengl
        GLenum gl_internal_format, gl_format, gl_type;

        switch (data_type)
        {
            case MYGL_INT:
                gl_type = GL_UNSIGNED_BYTE;
                break;
            case MYGL_FLOAT:
                gl_type = GL_FLOAT;
                break;
            default:
                tex_create_flag = false;
                printf("In createTextureFromPic, invalid data_type %x.\n", data_type);
                goto epilog;
        }

        switch (color_format)
        {
            case MYGL_RGB:
                gl_format = GL_RGB;
                break;
            case MYGL_RGBA:
                gl_format = GL_RGBA;
                break;
            default:
                tex_create_flag = false;
                printf("In createTextureFromPic, invalid color_format %x.\n", color_format);
                goto epilog;
        }

        if (gl_type == GL_FLOAT)
        {
            if (gl_format == GL_RGBA)
                gl_internal_format = GL_RGBA16F;
            else
                gl_internal_format = GL_RGB16F;
        }
        else if (gl_type == GL_UNSIGNED_BYTE)
        {
            if (gl_format == GL_RGBA)
                gl_internal_format = GL_RGBA;
            else
                gl_internal_format = GL_RGB;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, gl_internal_format, width, height, 0, gl_format, gl_type, data);
        checkError("glTexImage2D");
        glGenerateMipmap(GL_TEXTURE_2D);
        checkError("glGenerateMipmap");

epilog:
        stbi_image_free(data);

        glBindTexture(GL_TEXTURE_2D, 0);
        return texture;
    }

}

#endif