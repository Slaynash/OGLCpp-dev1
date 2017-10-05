#include "RenderScene.h"
#define PI 3.14159f

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

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << std::endl;
        SDL_Quit();

        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
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


    Shader shadertmp("res/shaders/2dshader.vs","res/shaders/2dshader.fs");
    shader = &shadertmp;

    glm::mat4 projectionMatrix = glm::perspective(70.0f, 1.0f, 0.1f, 100.0f);

    shader->bindProjectionMatrix(projectionMatrix);

    GLuint texture = SOIL_load_OGL_texture(
        "res/textures/tile_orange.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glUseProgram(shader->getProgramID());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    float pitch=0, yaw=0;

    bool close = false;
    int lookup=0, lookdown=0, lookleft=0, lookright=0;
    int forward=0, backward=0, left=0, right=0;
    int space=0, lshift=0;

    posX = 0;
    posY = 0;
    posZ = -4;

    float angMul;

    while(!close){

        while (SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) close = true;
            else if(event.type == SDL_KEYDOWN){
                if(     event.key.keysym.sym == SDLK_UP)        lookup = 1;
                else if(event.key.keysym.sym == SDLK_DOWN)      lookdown = 1;
                else if(event.key.keysym.sym == SDLK_LEFT)      lookleft = 1;
                else if(event.key.keysym.sym == SDLK_RIGHT)     lookright = 1;
                else if(event.key.keysym.sym == SDLK_z)         forward = 1;
                else if(event.key.keysym.sym == SDLK_s)         backward = 1;
                else if(event.key.keysym.sym == SDLK_q)         left = 1;
                else if(event.key.keysym.sym == SDLK_d)         right = 1;
                else if(event.key.keysym.sym == SDLK_SPACE)     space = 1;
                else if(event.key.keysym.sym == SDLK_LSHIFT)    lshift = 1;
            }
            else if(event.type == SDL_KEYUP){
                if(     event.key.keysym.sym == SDLK_UP)        lookup = 0;
                else if(event.key.keysym.sym == SDLK_DOWN)      lookdown = 0;
                else if(event.key.keysym.sym == SDLK_LEFT)      lookleft = 0;
                else if(event.key.keysym.sym == SDLK_RIGHT)     lookright = 0;
                else if(event.key.keysym.sym == SDLK_z)         forward = 0;
                else if(event.key.keysym.sym == SDLK_s)         backward = 0;
                else if(event.key.keysym.sym == SDLK_q)         left = 0;
                else if(event.key.keysym.sym == SDLK_d)         right = 0;
                else if(event.key.keysym.sym == SDLK_SPACE)     space = 0;
                else if(event.key.keysym.sym == SDLK_LSHIFT)    lshift = 0;
            }
        }

        pitch += (lookup-lookdown)*0.1f;
        yaw += (lookleft-lookright)*0.1f;
        posX += sin(yaw)*(forward-backward);
        posZ += cos(yaw)*(forward-backward);

        posX += sin(yaw+PI*0.5f)*(left-right);
        posZ += cos(yaw+PI*0.5f)*(left-right);

        posY += (space-lshift);

        if (yaw > PI*2.0f) yaw -= PI*2.0f;
		else if (yaw < 0.0f) yaw += PI*2.0f;
        if (pitch > PI*0.5f-0.0001f) pitch = PI/2-0.0001f;
		else if (pitch < -PI*0.5f+0.0001f) pitch = -PI/2+0.0001f;

        angMul = cos(pitch);

        lookX = sin(yaw)*angMul + posX;
        lookY = sin(pitch) + posY;
        lookZ = cos(yaw)*angMul + posZ;

        

        glUseProgram(shader->getProgramID());
        render();
        glUseProgram(0);
        SDL_GL_SwapWindow(m_window);
    }
}

static const GLfloat vertices[] = {

  -1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f, -1.0f,
  -1.0f, -1.0f, -1.0f,

   1.0f,  1.0f,  1.0f,
   1.0f, -1.0f,  1.0f,
   1.0f, -1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
   1.0f,  1.0f, -1.0f,
   1.0f,  1.0f,  1.0f,

  -1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,

   1.0f, -1.0f,  1.0f,
  -1.0f, -1.0f,  1.0f,
  -1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
   1.0f, -1.0f,  1.0f,

  -1.0f, -1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,
   1.0f,  1.0f, -1.0f,
   1.0f,  1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f, -1.0f,

   1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,
  -1.0f, -1.0f,  1.0f,
  -1.0f, -1.0f,  1.0f,
   1.0f, -1.0f,  1.0f,
   1.0f,  1.0f,  1.0f

};

static const GLfloat uvs[] = {
  0,0, 0,1, 1,1,  1,1, 1,0, 0,0,
  0,0, 0,1, 1,1,  1,1, 1,0, 0,0,
  0,0, 0,1, 1,1,  1,1, 1,0, 0,0,
  0,0, 0,1, 1,1,  1,1, 1,0, 0,0,
  0,0, 0,1, 1,1,  1,1, 1,0, 0,0,
  0,0, 0,1, 1,1,  1,1, 1,0, 0,0
};

void RenderScene::render(){

  glm::mat4 viewMatrix = glm::lookAt(glm::vec3(posX, posY, posZ),glm::vec3(lookX,lookY,lookZ),glm::vec3(0,1,0));
  shader->bindViewMatrix(viewMatrix);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0, 0.0, 0.0, 1.0);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, uvs);
  glEnableVertexAttribArray(1);

  glDrawArrays(GL_TRIANGLES, 0, 6*6);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
}
