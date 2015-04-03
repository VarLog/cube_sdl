//
//  RendererOpenGL2.h
//  cube_sdl
//
//  Created by Maxim Fedorenko on 30.03.15.
//

#ifndef __cube_sdl__RendererOpenGL2__
#define __cube_sdl__RendererOpenGL2__

#include <thread>

#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "IRenderer.h"

namespace Cube {
    class RendererOpenGL2 : public IRenderer {
        std::thread mRenderThread;
        
        SDL_Window *mWindow = NULL;
        SDL_GLContext mContext = NULL;
        
        // OpenGL Objects
        GLuint mVbo;
        
        // Shader Objects
        GLuint mProgram;
        
        // Get a handle for vertex buffer
        GLuint mVertexPosition;
        
        const GLsizei mScreenWidth = 640;
        const GLsizei mScreenHeight = 480;

        void initSDL();
        void initGL();
        void initShaders();
        void initVBO();
        void freeShader();
        void freeVBO();
        
        bool mIsInit = false;
        void init();
        void deinit();

        bool mIsRendering = false;
        
        bool mDone = false;
        
    public:
        RendererOpenGL2(const unsigned int screenWidth, const unsigned int screenHeight);
        ~RendererOpenGL2();
        
        virtual void setColor( float r, float g, float b, float a );
        virtual void startRender();
        virtual void stopRender();
    };
}

#endif /* defined(__cube_sdl__RendererOpenGL2__) */
