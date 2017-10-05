#include "RenderScene.h"

// Constructeur de Destucteur

RenderScene::RenderScene(std::string title, int width, int height) : m_title(title), m_width(width), m_height(height), m_window(0), m_glContext(0){}

RenderScene::~RenderScene(){
    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}


// M�thodes

bool RenderScene::initWindow(){
    // Initialisation de la SDL

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << std::endl;
        SDL_Quit();

        return false;
    }


    // Version d'OpenGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);


    // Double Buffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);



    m_window = SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL); // Cr�ation de la fen�tre

    if(m_window == 0){
        std::cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << std::endl;
        SDL_Quit();

        return false;
    }




    m_glContext = SDL_GL_CreateContext(m_window); // Cr�ation du contexte OpenGL

    if(m_glContext == 0){
        std::cout << SDL_GetError() << std::endl;
        SDL_DestroyWindow(m_window);
        SDL_Quit();

        return false;
    }

    return true;
}


bool RenderScene::initGL(){

    GLenum glewInitState = glewInit();// On initialise GLEW

    if(glewInitState != GLEW_OK){
        std::cout << "Erreur d'initialisation de GLEW : " << glewGetErrorString(glewInitState) << std::endl;

        SDL_GL_DeleteContext(m_glContext);
        SDL_DestroyWindow(m_window);
        SDL_Quit();

        return false;
    }

    return true;
}


void RenderScene::run(){


    Shader shader("res/shaders/2dshader.vs","res/shaders/2dshader.fs");

    // Generates a really hard-to-read matrix, but a normal, standard 4x4 matrix nonetheless
    glm::mat4 projectionMatrix = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);

    /*
    const float *pSource = (const float*)glm::value_ptr(projectionMatrix);
    for (int i = 0; i < 4; ++i){
        for (int j = 0; j < 4; ++j){
            std::cout << pSource[i*4+j] << "\t";
        }
        std::cout << std::endl;
    }
    */

    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(4,3,3),glm::vec3(0,0,0),glm::vec3(0,1,0));

    shader.bindProjectionMatrix(projectionMatrix);
    shader.bindViewMatrix(viewMatrix);
    //shader.bindProjectionMatrix(glm::mat4(1.0f));

    GLuint texture = SOIL_load_OGL_texture(
      "res/textures/tex1.png",
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_INVERT_Y
    );

    glUseProgram(shader.getProgramID());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    while(m_events.window.event != SDL_WINDOWEVENT_CLOSE){

        glUseProgram(shader.getProgramID());

        render();

        glUseProgram(0);

        SDL_GL_SwapWindow(m_window);// Actualisation de la fen�tre
        SDL_WaitEvent(&m_events);
    }
}

//TOP, BOTTOM, LEFT, RIGHT, FRONT, BACK

static const GLfloat vertices[] = {
  -1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f, -1.0f,
  -1.0f, -1.0f, -1.0f,

   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
   1.0f, -1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
};

static const GLfloat uvs[] = {
  0,0, 0,1, 1,1,  1,1, 1,0, 0,0,
  0,0, 0,1, 1,1,  1,1, 1,0, 0,0
};
/*
static const GLfloat vertices[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end

     1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end

     1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
     1.0f,-1.0f,-1.0f,

     1.0f, 1.0f,-1.0f,
     1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,

    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,

     1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,

    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
     1.0f,-1.0f, 1.0f,

     1.0f, 1.0f, 1.0f,
     1.0f,-1.0f,-1.0f,
     1.0f, 1.0f,-1.0f,

     1.0f,-1.0f,-1.0f,
     1.0f, 1.0f, 1.0f,
     1.0f,-1.0f, 1.0f,

     1.0f, 1.0f, 1.0f,
     1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,

     1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,

     1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
     1.0f,-1.0f, 1.0f
};

const float uvs[] = {
    0, 0, 0, 1, 1, 1,
    1, 1, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 1,
    1, 1, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 1,
    1, 1, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 1,
    1, 1, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 1,
    1, 1, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 1,
    1, 1, 1, 0, 0, 0,
};
*/
void RenderScene::render(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, uvs);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 12);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}
