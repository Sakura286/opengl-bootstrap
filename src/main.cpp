// original code: https://github.com/erick207/GLES3-SDL2-Demos/blob/master/tutorial1/main.cpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>
#include <GLES3/gl3.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "shader.hpp"
#include "stb_image.h"
#include "static_methods.hpp"
#include "glutil.hpp"

using namespace std;

const unsigned int DISP_WIDTH = 1024;
const unsigned int DISP_HEIGHT = 768;

int SDL_main(int argc, char *args[])
{

    // The window
    SDL_Window *window = NULL;

    // The OpenGL context
    SDL_GLContext context = NULL;

    // IMPORTANT! These sets must go BEFORE SDL_Init
    // Request OpenGL ES 3.0
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    // Want double-buffering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        cout << "SDL could not initialize! SDL_Error:\n";
        return EXIT_FAILURE;
    }

    // Setup the exit hook
    atexit(SDL_Quit);

    // Create the window
    window = SDL_CreateWindow("GLES3+SDL2 Test", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, DISP_WIDTH, DISP_HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if(!window)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
            "Couldn't create the main window.", NULL);
        cout << "Couldn't create the main window.\n";
        return EXIT_FAILURE;
    }

    context = SDL_GL_CreateContext(window);
    if(!context)
    {
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR, "Error",
            "Couldn't create an OpenGL context.", NULL);
        cout << "Couldn't create an OpenGL context.\n";
        return EXIT_FAILURE;
    }

    // opengl start
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    Shader ourShader("shaders/001_texture.vs", "shaders/001_texture.fs");
    float vertices[] = {
        // positions         // colors            // texture coord
         1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
         1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
        -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };

    unsigned int indices[] = {
        2, 3, 1, // 第一个三角形
        3, 1, 0  // 第二个三角形
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // unsigned int texture0;
    // glGenTextures(1, &texture0);
    // glBindTexture(GL_TEXTURE_2D, texture0);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // checkError("glTexParameteri");
    

    //GLint swizzleMask[] = {GL_BLUE, GL_GREEN, GL_RED, GL_ALPHA};
    //glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA_EXT, swizzleMask);

    int width, height, nrChannels;
    bool tex_create_flag;

    GLuint tex1 = mygl::createTextureFromPic(
            "./pics/awesomeface.png", width, height, nrChannels,
            mygl::MYGL_RGBA, mygl::MYGL_FLOAT,
            tex_create_flag);

    if (!tex_create_flag)
    {
        printf("Texture create failed.\n");
        return EXIT_FAILURE;
    }

    GLuint fbo1, fbo2;
    glGenFramebuffers(1, &fbo1);
    glGenFramebuffers(1, &fbo2);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo1);

    // Attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex1, 0);
    checkError("glFramebufferTexture2D");
    glBindTexture(GL_TEXTURE_2D, 0);

    const int TEX_EDGE = 1024;
    GLuint tex2 = mygl::createTexture(TEX_EDGE, TEX_EDGE);
    checkError("texcreated");
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, TEX_EDGE, TEX_EDGE);
    checkError("glTexStorage2D");
    glBindFramebuffer(GL_FRAMEBUFFER, fbo2);
    checkError("glBindFramebuffer");
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex2, 0);
    checkError("glFramebufferTexture2D");

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glClear(GL_STENCIL_BUFFER_BIT);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);

    ourShader.use();
    checkError("gluse");
    glBindTexture(GL_TEXTURE_2D, tex1);
    checkError("glBindTexture");
    glBindVertexArray(VAO);
    checkError("glBindVertexArray");
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture0"), 0);
    checkError("glUniform1i");
    glBindTexture(GL_TEXTURE_2D, tex1);
    saveFrameBuff("/usb0/stkimg/fbo1.ppm", fbo1, width, height);
    saveFrameBuff("/usb0/stkimg/fbo2_before.ppm", fbo2, TEX_EDGE, TEX_EDGE);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    checkError("glDrawElements");
    saveFrameBuff("/usb0/stkimg/fbo2_end.ppm", fbo2, TEX_EDGE, TEX_EDGE);

    //SDL_GL_SwapWindow(window);

    // Wait for the user to quit
    bool quit = false;
    while (!quit)
    {
        break;
        ourShader.use();

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);

        SDL_Event event;
        if (SDL_WaitEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                // User wants to quit
                quit = true;
            }
        }
    }

    return EXIT_SUCCESS;
}

int main(int argc, char** argv) {
    return SDL_main(argc, argv);
}