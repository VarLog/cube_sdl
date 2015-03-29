//
//  main.cpp
//  cube_sdl
//
//  Created by Maxim Fedorenko on 29.03.15.
//  Copyright (c) 2015 Maxim Fedorenko. All rights reserved.
//

// The templete from here: <https://gist.github.com/tdgunes/df6fd0fe0fdba0a3cd82>

#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>

#include <glm/glm.hpp>

#include <string>
#include <iostream>

#ifdef DEBUG
#define ASSERT(x) assert(x);
#else
#define ASSERT(x) x
#endif

GLenum __gl_err_code;
#ifndef DEBUG
#define ASSERT_GL( gl_code ) gl_code
#else
#define ASSERT_GL( gl_code ) do \
{ \
    while (glGetError() != GL_NO_ERROR) ; \
        gl_code; \
__gl_err_code = glGetError(); \
ASSERT(__gl_err_code == GL_NO_ERROR); \
} while(0)
#endif

using namespace std;

SDL_Window *window = NULL;
SDL_GLContext context = NULL;

// OpenGL Objects
GLuint vbo;
GLuint vao;

// Shader Objects
GLuint program;
GLuint vs;
GLuint fs;

// Sampler Object
GLuint uniformColor;
GLuint attributeCoord;

// The shaders
const string vertexShaderString =
"#version 330 \n"
"in vec3 coord;               \n"
"in vec4 color;        \n"
"out vec4 fragColor;\n"
"void main() {                       \n"
"    fragColor = color; \n"
"    gl_Position = vec4(coord, 1.0); \n"
"}                                   \n";

const string fragmentShaderString =
"#version 330\n"
"in vec4 fragColor;\n"
"out vec4 finalColor;\n"
"void main() {              \n"
"    finalColor = fragColor;  \n"
"}                          \n";

// The vertices
const GLfloat vertices[] =
{
    //X   Y     Z     R     G     B     A
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
};

const GLsizei screenWidth = 640;
const GLsizei screenHeight = 480;

void shaderLog(GLuint shader)
{
    int len = 0;
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

void initSDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    
    //Create Window and Context
    window = SDL_CreateWindow("SDL with OpenGL", 0, 0, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
    
    //Set Core Context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    context = SDL_GL_CreateContext(window);
}

void initGL()
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
    
    ASSERT_GL( glViewport( 0.f, 0.f, screenWidth, screenHeight) );
    ASSERT_GL( glClearColor( 0.2f, 0.2f, 0.2f, 1.f) );
}

void initShaders() {
    //Create Shaders
    vs = glCreateShader(GL_VERTEX_SHADER);
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    
    //Source Pointers
    const GLchar* vsSource = &vertexShaderString[0];
    const GLchar* fsSource = &fragmentShaderString[0];
    
    //Set Source
    ASSERT_GL( glShaderSource(vs, 1, &vsSource, NULL) );
    ASSERT_GL( glShaderSource(fs, 1, &fsSource, NULL) );
    
    //Compile Shaders
    ASSERT_GL( glCompileShader(vs) );
    ASSERT_GL( glCompileShader(fs) );

    shaderLog(vs);
    shaderLog(fs);
    
    //Create Shader Program
    program = glCreateProgram();
    
    //Attach Shaders to Program
    ASSERT_GL( glAttachShader(program, vs) );
    ASSERT_GL( glAttachShader(program, fs) );
    
    //Set Attribute Locations
    ASSERT_GL( glBindAttribLocation(program, 0, "coord") );
    ASSERT_GL( glBindAttribLocation(program, 1, "color") );
    
    //Link Program
    ASSERT_GL( glLinkProgram(program) );
    
    GLint status = GL_FALSE;
    ASSERT_GL( glGetProgramiv(program, GL_LINK_STATUS, &status) );
    if (status == GL_FALSE) {
        GLint logLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(program, logLength, &logLength, log);
        
        throw std::runtime_error("GL program link error" + string(log));
    }
    
    //No need for shaders anymore
    ASSERT_GL( glDeleteShader(vs) );
    ASSERT_GL( glDeleteShader(fs) );
    
}

void initVBO() {
    //Setup VAO and VBO
    ASSERT_GL( glGenVertexArrays(1, &vao) );
    ASSERT_GL( glGenBuffers(1, &vbo) );

    ASSERT_GL( glBindVertexArray(vao) );
    ASSERT_GL( glBindBuffer(GL_ARRAY_BUFFER, vbo) );
    
    ASSERT_GL( glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 7 * 8, vertices, GL_STATIC_DRAW) );

    ASSERT_GL( glEnableVertexAttribArray(0) );
    ASSERT_GL( glEnableVertexAttribArray(1) );
    
    ASSERT_GL( glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), NULL) );
    ASSERT_GL( glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat))) );
}

void freeShader()
{
    ASSERT_GL( glUseProgram(0) );
    ASSERT_GL( glDeleteProgram(program) );
}

void freeVBO()
{
    ASSERT_GL( glBindBuffer(GL_ARRAY_BUFFER, 0) );
    ASSERT_GL( glDeleteBuffers(1, &vbo) );
}

int main(int argc, char* args[])
{
    initSDL();
    initGL();
    initShaders();
    initVBO();
    
    bool done = 0;
    
    // Draw
    while (!done) {
        ASSERT_GL( glUseProgram(program) );
        
        ASSERT_GL( glClear(GL_COLOR_BUFFER_BIT) );
        ASSERT_GL( glClear(GL_DEPTH_BUFFER_BIT) );
        
        ASSERT_GL( glBindVertexArray(vao) );

        ASSERT_GL( glBindBuffer(GL_ARRAY_BUFFER, vbo) );
        
        ASSERT_GL( glDrawArrays(GL_TRIANGLES, 0, 8) );
        
        ASSERT_GL( glBindBuffer(GL_ARRAY_BUFFER, 0) );
        
        ASSERT_GL( glBindVertexArray(0) );
        
        ASSERT_GL( glUseProgram(0) );
        
        ASSERT_GL( SDL_GL_SwapWindow(window) );
        
        SDL_Delay(20);
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}

