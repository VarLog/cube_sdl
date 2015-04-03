//
//  Defs.h
//  cube_sdl
//
//  Created by Maxim Fedorenko on 30.03.15.
//  Copyright (c) 2015 Maxim Fedorenko. All rights reserved.
//

#ifndef cube_sdl_Defs_h
#define cube_sdl_Defs_h

#include <assert.h>

#ifdef DEBUG
#define ASSERT(x) assert(x);
#else
#define ASSERT(x) x
#endif

#include <GL/glew.h>

#ifndef DEBUG
#define ASSERT_GL( gl_code ) gl_code
#else
#define ASSERT_GL( gl_code ) do \
{ \
while (glGetError() != GL_NO_ERROR) ; \
gl_code; \
ASSERT(glGetError() == GL_NO_ERROR); \
} while(0)
#endif


#endif
