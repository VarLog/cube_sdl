//
//  Controller.cpp
//  cube_sdl
//
//  Created by Maxim Fedorenko on 30.03.15.
//  Copyright (c) 2015 Maxim Fedorenko. All rights reserved.
//

#include <iostream>
#include <string>

#include "Controller.h"

using namespace Cube;

Controller::Controller(std::unique_ptr<IRenderer> &&renderer)
{
    mRenderer = std::move(renderer);
}

int Controller::run() {
    
    mRenderer->startRender();
    
    std::string s;
    while (s != "q") {
        std::cin >> s;
    }
    return 0;
}

