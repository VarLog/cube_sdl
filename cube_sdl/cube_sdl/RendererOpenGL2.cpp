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
            
            
            ASSERT_GL( glDrawArrays(GL_TRIANGLES, 0, 6) );
            
            ASSERT_GL( SDL_GL_SwapWindow(mWindow) );
            
            SDL_Delay(200);
        }

    };
    std::thread thread(f);
    mRenderThread = std::move(thread);
}

void RendererOpenGL2::init() {
    initSDL();
    initGL();
    initShaders();
    initVBO();
    
    ASSERT_GL( glClear(GL_COLOR_BUFFER_BIT) );
    ASSERT_GL( glClear(GL_DEPTH_BUFFER_BIT) );
    
    ASSERT_GL( glUseProgram(mProgram) );
    
    mIsInit = true;
}

RendererOpenGL2::~RendererOpenGL2() {
    mDone = true;
    mRenderThread.join();
    
    freeVBO();
    freeShader();
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void RendererOpenGL2::setColor( float r, float g, float b, float a ) {
    
}

void RendererOpenGL2::startRender() {
    mIsRendering = true;
}

void RendererOpenGL2::stopRender() {
    mIsRendering = false;
}


void RendererOpenGL2::shaderLog(GLuint shader)
{
    int len = 0;
    assert(1);
    
    ASSERT_GL( glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len) );
    
    if(len > 1)
    {
        auto msg = new char[len];
        assert(msg);
        
        int written = 0;
        ASSERT_GL( glGetShaderInfoLog(shader, len, &written, msg) );
        std::cout << "shaderLog " << shader << " : " << msg << "\n\n";
        delete[] msg;
    }
}

void RendererOpenGL2::initSDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    
    //Create Window and Context
    mWindow = SDL_CreateWindow("SDL with OpenGL", 0, 0, mScreenWidth, mScreenHeight, SDL_WINDOW_OPENGL);
    
    //Set Core Context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
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
    //Create Shaders
    mVs = glCreateShader(GL_VERTEX_SHADER);
    mFs = glCreateShader(GL_FRAGMENT_SHADER);
    
    const std::string vertexShaderString =
    "#version 330 \n"
    "in vec3 coord;               \n"
    "in vec4 color;        \n"
    "out vec4 fragColor;\n"
    "void main() {                       \n"
    "    fragColor = color; \n"
    "    gl_Position = vec4(coord, 1.0); \n"
    "}                                   \n";
    
    const std::string fragmentShaderString =
    "#version 330\n"
    "in vec4 fragColor;\n"
    "out vec4 finalColor;\n"
    "void main() {              \n"
    "    finalColor = fragColor;  \n"
    "}                          \n";

    
    //Source Pointers
    const GLchar* vsSource = &vertexShaderString[0];
    const GLchar* fsSource = &fragmentShaderString[0];
    
    //Set Source
    ASSERT_GL( glShaderSource(mVs, 1, &vsSource, NULL) );
    ASSERT_GL( glShaderSource(mFs, 1, &fsSource, NULL) );
    
    //Compile Shaders
    ASSERT_GL( glCompileShader(mVs) );
    ASSERT_GL( glCompileShader(mFs) );
    
    shaderLog(mVs);
    shaderLog(mFs);
    
    //Create Shader Program
    mProgram = glCreateProgram();
    
    //Attach Shaders to Program
    ASSERT_GL( glAttachShader(mProgram, mVs) );
    ASSERT_GL( glAttachShader(mProgram, mFs) );
    
    //Set Attribute Locations
    ASSERT_GL( glBindAttribLocation(mProgram, 0, "coord") );
    ASSERT_GL( glBindAttribLocation(mProgram, 1, "color") );
    
    //Link Program
    ASSERT_GL( glLinkProgram(mProgram) );
    
    GLint status = GL_FALSE;
    ASSERT_GL( glGetProgramiv(mProgram, GL_LINK_STATUS, &status) );
    if (status == GL_FALSE) {
        GLint logLength = 0;
        glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &logLength);
        
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(mProgram, logLength, &logLength, log);
        
        throw std::runtime_error("GL program link error: " + std::string(log));
    }
    
    //No need for shaders anymore
    ASSERT_GL( glDeleteShader(mVs) );
    ASSERT_GL( glDeleteShader(mFs) );
    
}

void RendererOpenGL2::initVBO() {
    //Setup VAO and VBO
    ASSERT_GL( glGenVertexArrays(1, &mVao) );
    ASSERT_GL( glGenBuffers(1, &mVbo) );
    
    ASSERT_GL( glBindVertexArray(mVao) );
    ASSERT_GL( glBindBuffer(GL_ARRAY_BUFFER, mVbo) );
    
    // The vertices
    const GLfloat vertices[] =
    {
        //X   Y     Z     R     G     B     A
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    };
    
    ASSERT_GL( glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 7 * 6, vertices, GL_STATIC_DRAW) );
    
    ASSERT_GL( glEnableVertexAttribArray(0) );
    ASSERT_GL( glEnableVertexAttribArray(1) );
    
    ASSERT_GL( glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), NULL) );
    ASSERT_GL( glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat))) );
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
