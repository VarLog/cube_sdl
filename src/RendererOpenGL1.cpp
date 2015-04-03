//
//  RendererOpenGL1.cpp
//  cube_sdl
//
//  Created by Maxim Fedorenko on 31.03.15.
//

// The template from here: <http://ivan4b.ru/simple-sdl-opengl-program-tutorial>

#include <string>
#include <iostream>

#include "Defs.h"
#include "RendererOpenGL1.h"

#include "shader.h"

using namespace Cube;

void RendererOpenGL1::drawCube(float xrf, float yrf, float zrf) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -7.0f);    // Сдвинуть вглубь экрана
    
    glRotatef(xrf, 1.0f, 0.0f, 0.0f);   // Вращение куба по X, Y, Z
    glRotatef(yrf, 0.0f, 1.0f, 0.0f);   // Вращение куба по X, Y, Z
    glRotatef(zrf, 0.0f, 0.0f, 1.0f);   // Вращение куба по X, Y, Z
    
    glBegin(GL_QUADS);                  // Рисуем куб
    
    glColor3f(0.0f, 1.0f, 0.0f);        // Синяя сторона (Верхняя)
    glVertex3f( 1.0f, 1.0f, -1.0f);     // Верхний правый угол квадрата
    glVertex3f(-1.0f, 1.0f, -1.0f);     // Верхний левый
    glVertex3f(-1.0f, 1.0f,  1.0f);     // Нижний левый
    glVertex3f( 1.0f, 1.0f,  1.0f);     // Нижний правый
    
    glColor3f(1.0f, 0.5f, 0.0f);        // Оранжевая сторона (Нижняя)
    glVertex3f( 1.0f, -1.0f,  1.0f);    // Верхний правый угол квадрата
    glVertex3f(-1.0f, -1.0f,  1.0f);    // Верхний левый
    glVertex3f(-1.0f, -1.0f, -1.0f);    // Нижний левый
    glVertex3f( 1.0f, -1.0f, -1.0f);    // Нижний правый
    
    glColor3f(1.0f, 0.0f, 0.0f);        // Красная сторона (Передняя)
    glVertex3f( 1.0f,  1.0f, 1.0f);     // Верхний правый угол квадрата
    glVertex3f(-1.0f,  1.0f, 1.0f);     // Верхний левый
    glVertex3f(-1.0f, -1.0f, 1.0f);     // Нижний левый
    glVertex3f( 1.0f, -1.0f, 1.0f);     // Нижний правый
    
    glColor3f(1.0f,1.0f,0.0f);          // Желтая сторона (Задняя)
    glVertex3f( 1.0f, -1.0f, -1.0f);    // Верхний правый угол квадрата
    glVertex3f(-1.0f, -1.0f, -1.0f);    // Верхний левый
    glVertex3f(-1.0f,  1.0f, -1.0f);    // Нижний левый
    glVertex3f( 1.0f,  1.0f, -1.0f);    // Нижний правый
    
    glColor3f(0.0f,0.0f,1.0f);          // Синяя сторона (Левая)
    glVertex3f(-1.0f,  1.0f,  1.0f);    // Верхний правый угол квадрата
    glVertex3f(-1.0f,  1.0f, -1.0f);    // Верхний левый
    glVertex3f(-1.0f, -1.0f, -1.0f);    // Нижний левый
    glVertex3f(-1.0f, -1.0f,  1.0f);    // Нижний правый
    
    glColor3f(1.0f,0.0f,1.0f);          // Фиолетовая сторона (Правая)
    glVertex3f( 1.0f,  1.0f, -1.0f);    // Верхний правый угол квадрата
    glVertex3f( 1.0f,  1.0f,  1.0f);    // Верхний левый
    glVertex3f( 1.0f, -1.0f,  1.0f);    // Нижний левый
    glVertex3f( 1.0f, -1.0f, -1.0f);    // Нижний правый
    
    glEnd();                            // Закончили квадраты
    
}

RendererOpenGL1::RendererOpenGL1(const unsigned int screenWidth, const unsigned int screenHeight)
: mScreenWidth(screenWidth), mScreenHeight(screenHeight)
{
    auto f = [this] {
        
        float xrf = 0, yrf = 0, zrf = 0; // углы поворота
        
        // Draw
        while (!mDone) {
            if (!mIsRendering) {
                SDL_Delay(500);
                continue;
            }
            
            if (!mIsInit) {
                init();
            }
            
            SDL_Event event; // события SDL
            
            while ( SDL_PollEvent(&event) ){ // начинаем обработку событий
                switch(event.type){ // смотрим:
                    case SDL_QUIT: // если произошло событие закрытия окна, то завершаем работу программы
                        mDone = true;
                        break;
                        
                    case SDL_KEYDOWN: // если нажата клавиша
                        switch(event.key.keysym.sym){ // смотрим какая
                            case SDLK_ESCAPE: // клавиша ESC
                                mDone = true; // завершаем работу программы
                                break;
                        }
                        break;
                }
            }
            
            // пока программа запущена изменяем углы поворота, тем самым вращая куб
            
            xrf -= 0.5;
            yrf -= 0.5;
            zrf -= 0.5;
            
            drawCube(xrf, yrf, zrf); // рисуем сам куб с текущими углами поворота
            
            // обновляем экран
            
            ASSERT_GL( glFlush() );
            SDL_GL_SwapWindow(mWindow);
        }

        deinit();
    };
    std::thread thread(f);
    mRenderThread = std::move(thread);
}

void RendererOpenGL1::init() {
    initSDL();
    initGL();
    
    mIsInit = true;
}

void RendererOpenGL1::deinit() {
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

RendererOpenGL1::~RendererOpenGL1() {
    mDone = true;
    mRenderThread.join();
}

void RendererOpenGL1::setColor( float r, float g, float b, float a ) {
    
}

void RendererOpenGL1::startRender() {
    mIsRendering = true;
}

void RendererOpenGL1::stopRender() {
    mIsRendering = false;
}

void RendererOpenGL1::initSDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    
    //Create Window and Context
    mWindow = SDL_CreateWindow("SDL with OpenGL", 0, 0, mScreenWidth, mScreenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    
    mContext = SDL_GL_CreateContext(mWindow);
}

void RendererOpenGL1::initGL()
{
    //Init Glew
    glewInit();
    
    ASSERT_GL( glClearColor(0.2f, 0.2f, 0.2f, 1.0f) );
    ASSERT_GL( glClearDepth(1.0) );
    ASSERT_GL( glDepthFunc(GL_LESS) );
    ASSERT_GL( glEnable(GL_DEPTH_TEST) );
    ASSERT_GL( glShadeModel(GL_SMOOTH) );
    ASSERT_GL( glMatrixMode(GL_PROJECTION) );
    ASSERT_GL( glLoadIdentity() );
    ASSERT_GL( gluPerspective(45.0f, (float) mScreenWidth / (float) mScreenHeight, 0.1f, 100.0f) );
    ASSERT_GL( glMatrixMode(GL_MODELVIEW) );
}
