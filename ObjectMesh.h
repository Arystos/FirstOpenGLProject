#ifndef PYRAMID_MESH_CLASS_H
#define PYRAMID_MESH_CLASS_H

#include "Camera.h"
#include "shaderClass.h"
#include "Texture.h"
#include "Camera.h"

#include"VAO.h"

class ObjectMesh
{
public:
    ObjectMesh(Shader shaderProgram, Texture texture );
    ~ObjectMesh();

    void CreateMesh(GLfloat* vertices, char type, GLsizeiptr verticesSize, GLuint* indices, GLsizeiptr indicesSize);
    void RenderMesh(Camera camera, GLsizei size);
    void SpawnObject(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation);
    void Translate(glm::vec3 position);
    

private:
    Shader shaderProgram;
    Texture texture;
    VAO* vao;
    
    // Create the pyramid mesh
    // Vertices for the pyramid

};

#endif
