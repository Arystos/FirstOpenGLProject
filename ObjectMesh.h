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

    glm::vec3 currentPosition;

    void CreateMesh(char type, GLfloat* vertices, GLsizeiptr verticesSize, GLuint* indices, GLsizeiptr indicesSize);
    void MoveObject(glm::vec3 direction, float speed);
    void OrbitObject(glm::vec3 center, float radius, float speed);
    void RenderMesh(Camera camera, GLsizei size);
    void SetLightPosition(glm::vec3 vec);
    void SetLightColor(glm::vec4 vec);
    void SpawnObject(char type, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation);

private:
    Shader shaderProgram;
    Texture texture;
    VAO* vao;
};

#endif
