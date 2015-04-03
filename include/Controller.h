//
//  Controller.h
//  cube_sdl
//
//  Created by Maxim Fedorenko on 30.03.15.
//  Copyright (c) 2015 Maxim Fedorenko. All rights reserved.
//

#ifndef __cube_sdl__Controller__
#define __cube_sdl__Controller__

#include <memory>

#include "IRenderer.h"

namespace Cube {
    class Controller {
        std::unique_ptr<IRenderer> mRenderer;
        
    public:
        Controller(std::unique_ptr<IRenderer> &&renderer);
        int run();
    };
}
    
#endif /* defined(__cube_sdl__Controller__) */
