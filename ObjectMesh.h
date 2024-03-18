#ifndef PYRAMID_MESH_CLASS_H
#define PYRAMID_MESH_CLASS_H

#include "Camera.h"
#include "shaderClass.h"
#include "Texture.h"
#include "Camera.h"
#include <vector>

#include"VAO.h"

class ObjectMesh
{
public:
    ObjectMesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, Shader& shaderProgram, Texture& texture);
    ~ObjectMesh();

    Shader shaderProgram;
    Texture texture;

    glm::vec3 currentPosition;
    std::vector <Vertex> vertices;
    std::vector <GLuint> indices;

    void CreateMesh();
    void MoveObject(glm::vec3 direction, float speed);
    void OrbitObject(glm::vec3 center, float radius, float speed);
    void RenderMesh(Camera& camera, GLsizei size);
    void RotateObject(glm::vec3 rotation);
    void SetLightPosition(glm::vec3 vec);
    void SetLightColor(glm::vec4 vec);
    void SpawnObject(char type, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation);

private:
    VAO* vao;
};

#endif
