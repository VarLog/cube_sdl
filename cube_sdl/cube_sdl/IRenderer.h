//
//  IRenderer.h
//  cube_sdl
//
//  Created by Maxim Fedorenko on 30.03.15.
//  Copyright (c) 2015 Maxim Fedorenko. All rights reserved.
//

#ifndef cube_sdl_IRenderer_h
#define cube_sdl_IRenderer_h

namespace Cube {
    class IRenderer {
    public:
        virtual void setColor( float r, float g, float b, float a ) = 0;
        virtual void startRender() = 0;
        virtual void stopRender() = 0;
    };
}

#endif
