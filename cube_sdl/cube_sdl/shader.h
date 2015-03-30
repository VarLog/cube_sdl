//
//  shader.h
//  cube_sdl
//
//  Created by Maxim Fedorenko on 31.03.15.
//

#ifndef SHADER_H
#define SHADER_H

namespace Cube {
    GLuint LoadShaders(const char *vertex_shader, const char *fragment_shader);
    GLuint LoadShadersFromFiles(const char *vertex_file_path, const char *fragment_file_path);
}

#endif
