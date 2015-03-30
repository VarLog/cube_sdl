//
//  RendererOpenGL1.h
//  cube_sdl
//
//  Created by Maxim Fedorenko on 31.03.15.
//

#ifndef __cube_sdl__RendererOpenGL1__
#define __cube_sdl__RendererOpenGL1__

#include <thread>

#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>

#include "IRenderer.h"

namespace Cube {
    class RendererOpenGL1 : public IRenderer {
        std::thread mRenderThread;
        
        SDL_Window *mWindow = NULL;
        SDL_GLContext mContext = NULL;
        
        const GLsizei mScreenWidth = 640;
        const GLsizei mScreenHeight = 480;

        void initSDL();
        void initGL();
        
        bool mIsInit = false;
        void init();
        void deinit();
        
        void drawCube(float xrf = 0.f, float yrf = 0.f, float zrf = 0.f);

        bool mIsRendering = false;
        
        bool mDone = false;
        
    public:
        RendererOpenGL1(const unsigned int screenWidth, const unsigned int screenHeight);
        ~RendererOpenGL1();
        
        virtual void setColor( float r, float g, float b, float a );
        virtual void startRender();
        virtual void stopRender();
    };
}

#endif /* defined(__cube_sdl__RendererOpenGL1__) */
