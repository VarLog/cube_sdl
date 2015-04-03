cmake_minimum_required(VERSION 2.8)

project(cube)

if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory!" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
        message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
        message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()

find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message(SEND_ERROR "Failed to find OpenGL")
    return()
else()
    include_directories(${OPENGL_INCLUDE_DIR})
endif()

#find_package(SDL REQUIRED)
#if(NOT SDL_FOUND)
#    message(SEND_ERROR "Failed to find SDL")
#    return()
#else()
#    include_directories(${SDL_INCLUDE_DIR})
#endif()

find_package(PkgConfig REQUIRED)
if(NOT PKGCONFIG_FOUND)
    message(SEND_ERROR "Failed to find PkgConfig")
    return()
else()
    pkg_check_modules (SDL2 REQUIRED sdl2)
    include_directories(${SLD2_INCLUDE_DIRS})
endif()


find_package(GLEW REQUIRED)
if(NOT GLEW_FOUND)
    message(SEND_ERROR "Failed to find GLEW")
    return()
else()
    include_directories(${GLEW_INCLUDE_DIR})
endif()

find_package(GLM REQUIRED)
if(NOT GLM_FOUND)
    message(SEND_ERROR "Failed to find GLM")
    return()
else()
    include_directories(${GLM_INCLUDE_DIR})
endif()

include_directories( "include" )
link_directories( ${SDL2_LIBRARY_DIRS} )

set(ALL_LIBS ${OPENGL_LIBRARIES} 
             ${GLEW_LIBRARIES}
             ${SDL2_LIBRARIES} 
)

set(SOURCES "src/Controller.cpp"
            "src/main.cpp"
            "src/RendererOpenGL1.cpp"
            "src/RendererOpenGL2.cpp"
            "src/shader.cpp" )

set(HEADERS "include/Controller.h"
            "include/Defs.h"
            "include/IRenderer.h"
            "include/RendererOpenGL1.h"
            "include/RendererOpenGL2.h"
            "include/shader.h" )

add_executable(cube ${SOURCES} ${HEADERS})
set_property(TARGET cube PROPERTY CXX_STANDARD 11)

target_link_libraries(cube ${ALL_LIBS})

# Xcode 
set_target_properties(cube PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/")


