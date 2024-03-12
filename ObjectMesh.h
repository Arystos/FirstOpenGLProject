#ifndef PYRAMID_MESH_CLASS_H
#define PYRAMID_MESH_CLASS_H

#include "shaderClass.h"
#include "Texture.h"

#include"VAO.h"

class ObjectMesh
{
public:
    ObjectMesh(Shader shaderProgram, Texture texture );
    ~ObjectMesh();

    void CreateMesh(GLfloat* vertices, GLsizeiptr verticesSize, GLuint* indices, GLsizeiptr indicesSize);
    void RenderMesh(GLsizei size);

private:
    Shader shaderProgram;
    Texture texture;
    VAO* vao;
    
    // Create the pyramid mesh
    // Vertices for the pyramid

};

#endif
