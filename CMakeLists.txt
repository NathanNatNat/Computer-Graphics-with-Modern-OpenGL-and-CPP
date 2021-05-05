cmake_minimum_required(VERSION 3.20.2)

project (OpenGLCourse)

add_executable(
    ${PROJECT_NAME} 
    src/main.cpp
    src/Window.cpp
    src/Window.h
    src/Camera.cpp
    src/Camera.h
    src/CommonValues.h
    src/DirectionalLight.cpp
    src/DirectionalLight.h
    src/Light.cpp
    src/Light.h
    src/Material.cpp
    src/Material.h
    src/Mesh.cpp
    src/Mesh.h
    src/Model.cpp
    src/Model.h
    src/OmniShadowMap.cpp
    src/OmniShadowMap.h
    src/PointLight.cpp
    src/PointLight.h
    src/Shader.cpp
    src/Shader.h
    src/ShadowMap.cpp
    src/ShadowMap.h
    src/Skybox.cpp
    src/Skybox.h
    src/SpotLight.cpp
    src/SpotLight.h
    src/Texture.cpp
    src/Texture.h)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})

#           DOWNLOAD ALL THE SUBMODULES
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()
