#include "ShaderUtils.h"

bool loadShader(std::string vertexSource, std::string fragmentSource, GLuint &programID, GLuint &vertexID, GLuint &fragmentID){

	if(!compileShader(vertexID, GL_VERTEX_SHADER, vertexSource)) return false;
    if(!compileShader(fragmentID, GL_FRAGMENT_SHADER, fragmentSource)) return false;

    programID = glCreateProgram();

    glAttachShader(programID, vertexID);
    glAttachShader(programID, fragmentID);

    return true;
}

bool linkShader(GLuint programID){

    glLinkProgram(programID);


    GLint linkError(0);
    glGetProgramiv(programID, GL_LINK_STATUS, &linkError);


    if(linkError != GL_TRUE){
        GLint errorSize(0);
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &errorSize);
		
		char *error = new char[errorSize + 1];

		glGetShaderInfoLog(programID, errorSize, &errorSize, error);
        error[errorSize] = '\0';
		
		std::cout << error << std::endl;

		delete[] error;
        glDeleteProgram(programID);

        return false;
    }
    else{
        glUseProgram(programID);
    }
    return true;
}

bool compileShader(GLuint &shader, GLenum type, std::string const &sourceFile){
    
    shader = glCreateShader(type);

    if(shader == 0){
        std::cout << "Bad shader type (" << type << ")" << std::endl;
        return false;
    }

    std::ifstream fichier(sourceFile.c_str());

    if(!fichier){
        std::cout << "Could not find file " << sourceFile << std::endl;
        glDeleteShader(shader);

        return false;
    }


    std::string ligne;
    std::string codeSource;

    while(getline(fichier, ligne)) codeSource += ligne + '\n';
    
    fichier.close();


    const GLchar* chaineCodeSource = codeSource.c_str();

    glShaderSource(shader, 1, &chaineCodeSource, 0);
    glCompileShader(shader);

    GLint erreurCompilation(0);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &erreurCompilation);

    if(erreurCompilation != GL_TRUE){
        
        GLint tailleErreur(0);
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &tailleErreur);

        char *erreur = new char[tailleErreur + 1];

        glGetShaderInfoLog(shader, tailleErreur, &tailleErreur, erreur);
        erreur[tailleErreur] = '\0';


        std::cout << erreur << std::endl;


        delete[] erreur;
        glDeleteShader(shader);

        return false;
    }
    else return true;
}