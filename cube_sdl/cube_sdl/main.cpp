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

#include <string>
#include <iostream>

using namespace std;

SDL_Window *window = NULL;
SDL_GLContext context = NULL;
TTF_Font* font = NULL;
SDL_Surface* surface = NULL;

//OpenGL Objects
GLuint vao;
GLuint vbo;
GLuint texture;

//Shader Objects
GLuint program;
GLuint vs;
GLuint fs;

//Sampler Object
GLuint uniformSampler;


//The shaders are identical to yours
const string fragmentShaderString =
"#version 330\n" // My laptop can't do OpenGL 3.3 so 3.0 will have to do
"in vec2 texCoord;\n"
"in vec4 fragColor;\n"
"\n"
"out vec4 finalColor;\n"
"\n"
"uniform sampler2D myTextureSampler;\n"
"void main() {\n"
"  finalColor = texture( myTextureSampler, texCoord ) * fragColor;\n"
"}";

const string vertexShaderString =
"#version 330\n"
"\n"
"in vec3 vert;\n"
"in vec4 color;\n"
"in vec2 texcoord;\n"
"\n"
"out vec4 fragColor;\n"
"out vec2 texCoord;\n"

"void main() {\n"
"  fragColor = color;\n"
"  gl_Position = vec4(vert, 1);\n"
"  texCoord = texcoord;\n"
"}\n";

//Your vertices, but I changed alpha to 1.0f
const GLfloat vertices[] =
{
    //X    Y      Z     R     G     B     A     U    V
    -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.f, 1.f,
    1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.f, 1.f,
    -1.0f, -0.4f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.f, 0.f,
    1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.f, 1.f,
    1.0f, -0.4f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.f, 0.f,
    -1.0f, -0.4f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.f, 0.f
};

int main(int argc, char* args[])
{
    
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    //Create Window and Context
    window = SDL_CreateWindow("SDL Text with OpenG", 0, 0, 640, 480, SDL_WINDOW_OPENGL);
    
    //Set Core Context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    context = SDL_GL_CreateContext(window);
    
    //Simple OpenGL State Settings
    glViewport( 0.f, 0.f, 640.f, 480.f);
    glClearColor( 0.f, 0.f, 0.f, 1.f);
    
    //Init Glew
    //Set glewExperimental for Core Context
    glewExperimental=true;
    glewInit();
    
    //Set Blending
    //Required so that the alpha channels show up from the surface
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_DEPTH);
    
    
    //Create Shaders
    vs = glCreateShader(GL_VERTEX_SHADER);
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    
    //Source Pointers
    const GLchar* vsSource= &vertexShaderString[0];
    const GLchar* fsSource = &fragmentShaderString[0];
    
    //Set Source
    glShaderSource(vs, 1, &vsSource, NULL);
    glShaderSource(fs, 1, &fsSource, NULL);
    
    //Compile Shaders
    glCompileShader(fs);
    glCompileShader(vs);
    
    //Create Shader Program
    program = glCreateProgram();
    
    //Attach Shaders to Program
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    
    //No need for shaders anymore
    glDeleteShader(vs);
    glDeleteShader(fs);
    
    //Set Attribute Locations
    glBindAttribLocation(program, 0, "vert");
    glBindAttribLocation(program, 1, "color");
    glBindAttribLocation(program, 2, "texcoord");
    
    //Link Program
    glLinkProgram(program);
    
    //Setup VAO and VBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9 * 6, vertices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), NULL);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),(GLvoid*)(7*sizeof(GLfloat)));
    
    //Init TTF
    TTF_Init();
    
    //Open Font
    font = TTF_OpenFont("Ubuntu-Regular.ttf", 30);
    
    SDL_Color color = {0, 255, 255, 255};
    
    //Create Surface
    surface = TTF_RenderUTF8_Blended(font, "This is TEXT!", color);
    
    //Your format checker
    GLenum format = (surface->format->BytesPerPixel==3)?GL_RGB:GL_RGBA;
    
    //Create OpenGL Texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D( GL_TEXTURE_2D, 0, format, surface->w, surface->h, 0,
                 format, GL_UNSIGNED_BYTE, surface->pixels);
    
    //Set Some basic parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    //Set up Sampler
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    uniformSampler = glGetUniformLocation(program, "myTextureSampler");
    //It defaults to using GL_TEXTURE0, so it's not necessary to set it
    //in this program it's generally a good idea.
    
    //--------------------------------------------------------------------------------------
    // DRAW STAGE
    //--------------------------------------------------------------------------------------
    
    glUseProgram(program);
    
    //glBindVertexArray(vao); - still in use
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    SDL_GL_SwapWindow(window);
    
    //Sleep for 2s before closing
    SDL_Delay(2000);
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}

