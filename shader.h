#ifndef SHADER_H
#define SHADER_H

#include <QOpenGLFunctions_4_1_Core>

//must inherit from QOpenGLFunctions_4_1_Core, since we use that instead of glfw/glew/glad
class Shader : protected QOpenGLFunctions_4_1_Core
{
public:
    Shader( const GLchar *vertexPath, const GLchar *fragmentPath );
    void use( );

    GLuint getProgram() const;

private:
    GLuint program;
};

#endif
