//
//  main.cpp
//  cube_sdl
//
//  Created by Maxim Fedorenko on 29.03.15.
//

#include <memory>

#include "RendererOpenGL1.h"
#include "RendererOpenGL2.h"
#include "Controller.h"

using namespace Cube;

int main(int argc, char* args[])
{
    const unsigned int w = 640;
    const unsigned int h = 480;
    
    std::unique_ptr<IRenderer> sp_opengl1(new RendererOpenGL1(w, h));
    //std::unique_ptr<IRenderer> sp_opengl2(new RendererOpenGL2(w, h));
    
    Controller controller(std::move(sp_opengl1));
    
    return controller.run();
}

