#ifndef DEF_RENDERSCENE
#define DEF_RENDERSCENE


// Includes

#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "soil/SOIL.h"


// Classe

class RenderScene
{
    public:

    RenderScene(std::string title, int width, int height);
    ~RenderScene();

    bool initWindow();
    bool initGL();
    void run();


    private:

    void render();

    std::string m_title;
    int m_width;
    int m_height;

    SDL_Window* m_window;
    SDL_GLContext m_glContext;
    SDL_Event m_events;
};

#endif
