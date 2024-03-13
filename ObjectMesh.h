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
    ObjectMesh(Shader shaderProgram, Texture texture);
    ~ObjectMesh();

    void CreateMesh(char type, GLfloat* vertices, GLsizeiptr verticesSize, GLuint* indices, GLsizeiptr indicesSize);
    void RenderMesh(Camera camera, GLsizei size);
    void SpawnObject(char type, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation);
    void MoveObject(glm::vec3 direction, float speed);

    void SetLightPosition(glm::vec3 vec);

private:
    Shader shaderProgram;
    Texture texture;
    VAO* vao;
    glm::vec3 currentPosition;

};

#endif
