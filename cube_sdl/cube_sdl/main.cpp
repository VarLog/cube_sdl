//
//  main.cpp
//  cube_sdl
//
//  Created by Maxim Fedorenko on 29.03.15.
//  Copyright (c) 2015 Maxim Fedorenko. All rights reserved.
//

#include <memory>

#include "RendererOpenGL2.h"
#include "Controller.h"

using namespace Cube;

int main(int argc, char* args[])
{
    const unsigned int w = 640;
    const unsigned int h = 480;
    
    std::unique_ptr<IRenderer> sp(new RendererOpenGL2(w, h));
    
    Controller controller(std::move(sp));
    
    return controller.run();
}

