//
//  RendererOpenGL2.cpp
//  cube_sdl
//
//  Created by Maxim Fedorenko on 30.03.15.
//  Copyright (c) 2015 Maxim Fedorenko. All rights reserved.
//

#include <string>
#include <iostream>

#include "Defs.h"
#include "RendererOpenGL2.h"

#include "shader.h"

using namespace Cube;


RendererOpenGL2::RendererOpenGL2(const unsigned int screenWidth, const unsigned int screenHeight)
: mScreenWidth(screenWidth), mScreenHeight(screenHeight)
{
    auto f = [this] {
        // Draw
        while (!mDone) {
            if (!mIsRendering) {
                SDL_Delay(500);
                continue;
            }
            
            if (!mIsInit) {
                init();
            }
            
            ASSERT_GL( glClear(GL_COLOR_BUFFER_BIT) );
            ASSERT_GL( glClear(GL_DEPTH_BUFFER_BIT) );
            
            ASSERT_GL( glUseProgram(mProgram) );
            
            ASSERT_GL( glEnableVertexAttribArray(mVertexPosition) );
            
            ASSERT_GL( glBindBuffer(GL_ARRAY_BUFFER, mVbo) );
            ASSERT_GL( glVertexAttribPointer(
                                  mVertexPosition,    // The attribute we want to configure
                                  3,                  // size
                                  GL_FLOAT,           // type
                                  GL_FALSE,           // normalized?
                                  0,                  // stride
                                  (void*)0            // array buffer offset
                                  ) );
            
            // Draw the triangle !
            ASSERT_GL( glDrawArrays(GL_TRIANGLES, 0, 3) ); // 3 indices starting at 0 -> 1 triangle
            
            ASSERT_GL( glDisableVertexAttribArray(mVertexPosition) );
            

            ASSERT_GL( SDL_GL_SwapWindow(mWindow) );
            
            SDL_Delay(200);
        }

        deinit();
    };
    std::thread thread(f);
    mRenderThread = std::move(thread);
}

void RendererOpenGL2::init() {
    initSDL();
    initGL();
    initShaders();
    initVBO();
    
    mIsInit = true;
}

void RendererOpenGL2::deinit() {
    freeVBO();
    freeShader();
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

RendererOpenGL2::~RendererOpenGL2() {
    mDone = true;
    mRenderThread.join();
}

void RendererOpenGL2::setColor( float r, float g, float b, float a ) {
    
}

void RendererOpenGL2::startRender() {
    mIsRendering = true;
}

void RendererOpenGL2::stopRender() {
    mIsRendering = false;
}

void RendererOpenGL2::initSDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    
    //Create Window and Context
    mWindow = SDL_CreateWindow("SDL with OpenGL", 0, 0, mScreenWidth, mScreenHeight, SDL_WINDOW_OPENGL);
    
    //Set Core Context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    
    mContext = SDL_GL_CreateContext(mWindow);
}

void RendererOpenGL2::initGL()
{
    //Init Glew
    //Set glewExperimental for Core Context
    glewExperimental=true;
    glewInit();
    
    //Set Blending
    //Required so that the alpha channels show up from the surface
    ASSERT_GL( glEnable(GL_BLEND) );
    ASSERT_GL( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );
    
    //Z-buffer
    ASSERT_GL( glEnable(GL_DEPTH_TEST) );
    
    ASSERT_GL( glViewport( 0.f, 0.f, mScreenWidth, mScreenHeight) );
    ASSERT_GL( glClearColor( 0.2f, 0.2f, 0.2f, 1.f) );
}

void RendererOpenGL2::initShaders() {
    
    const std::string vertexShaderString =
    "#version 120\n"
    "// Input vertex data.\n"
    "attribute vec3 vertexPosition_modelspace;\n"
    "\n"
    "void main(){\n"
    "\n"
    "    gl_Position = vec4(vertexPosition_modelspace, 1.0);\n"
    "\n"
    "}\n";
    
    const std::string fragmentShaderString =
    "#version 120\n"
    "\n"
    "void main()\n"
    "{\n"
    "\n"
    "    // Output color = red\n"
    "    gl_FragColor = vec4(1,0,0,1);\n"
    "\n"
    "}\n";

    mProgram = LoadShaders(vertexShaderString.c_str(), fragmentShaderString.c_str());
}

void RendererOpenGL2::initVBO() {
    
    mVertexPosition = glGetAttribLocation(mProgram, "vertexPosition_modelspace");
    
    //Setup VBO
    ASSERT_GL( glGenBuffers(1, &mVbo) );
    ASSERT_GL( glBindBuffer(GL_ARRAY_BUFFER, mVbo) );
    
    // The vertices
    const GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
    };
    
    ASSERT_GL( glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW) );
}

void RendererOpenGL2::freeShader()
{
    ASSERT_GL( glUseProgram(0) );
    ASSERT_GL( glDeleteProgram(mProgram) );
}

void RendererOpenGL2::freeVBO()
{
    ASSERT_GL( glBindBuffer(GL_ARRAY_BUFFER, 0) );
    ASSERT_GL( glDeleteBuffers(1, &mVbo) );
}
