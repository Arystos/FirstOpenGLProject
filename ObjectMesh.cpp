#include "ObjectMesh.h"
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"

constexpr char PYRAMID = 0;
constexpr char LIGHT = 1;

ObjectMesh::ObjectMesh(Shader shaderProgram, Texture texture)
    : shaderProgram(shaderProgram), texture(texture), currentPosition(glm::vec3(0.0f, 0.0f, 0.0f))
{
    this->shaderProgram = shaderProgram;
    this->texture = texture;
    vao = new VAO();
}

ObjectMesh::~ObjectMesh()
{
    // Destructor
    vao->Delete();
    texture.Delete();
    shaderProgram.Delete();
}

void ObjectMesh::CreateMesh(char type, GLfloat* vertices, GLsizeiptr verticesSize, GLuint* indices, GLsizeiptr indicesSize)
{
    // Bind the VAO
    vao->Bind();
    VBO vbo = VBO(vertices, verticesSize);
    EBO ebo = EBO(indices, indicesSize);

    // if case for pyramid and light
    if (type == PYRAMID) // Pyramid
    {
        // Links vbo attributes to vao
        vao->LinkAttrib(vbo, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0); // Position of the cube
        vao->LinkAttrib(vbo, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float))); // Color of the cube
        vao->LinkAttrib(vbo, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float))); // Texture coordinates of the cube
        vao->LinkAttrib(vbo, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float))); // Normals of the cube

        // Set the texture wrapping/filtering options (on the currently bound texture object)
        texture.texUnit(shaderProgram, "tex0", 0);
    }
    else if (type == LIGHT) // Light
    {
        // Links vbo to vao
        vao->LinkAttrib(vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    }
    else if (type == 2) // Cube
    {
        vao->LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
        vao->LinkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        vao->LinkAttrib(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    }

    // Unbind all to prevent accidentally modifying them
    vao->Unbind();
    vbo.Unbind();
    ebo.Unbind();
}

void ObjectMesh::RenderMesh(Camera camera, GLsizei size)
{
    // Render the object mesh
    // Activate the shader program
    shaderProgram.Activate();
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    // Bind the texture
    texture.Bind();
    // Bind the object VAO
    vao->Bind();
    // camera matrix
    camera.Matrix(shaderProgram, "camMatrix");
    // Draw the object
    glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
}

// Function to spawn the object at a certain position
void ObjectMesh::SpawnObject(char type, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
{
    // Create the model matrix
    glm::mat4 model = glm::mat4(1.0f); // Set to identity
    
    glm::vec4 lightColor = glm::vec4(1.0, 1.0, 1.0, 1.0);
    glm::vec3 lightPos;
    model = translate(model, position); // Change the position of the object
    model = glm::scale(model, scale); // Change the scale of the object
    model = rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate the object on the x axis
    model = rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate the object on the y axis
    model = rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate the object on the z axis

    // Set the model matrix in the shader
    shaderProgram.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform4fv(glGetUniformLocation(shaderProgram.ID, "lightColor"), 1, glm::value_ptr(lightColor));

    if (type != LIGHT)
    {
        glUniform3fv(glGetUniformLocation(shaderProgram.ID, "lightPos"), 1, glm::value_ptr(lightPos));
    }

    // Set the current position to the spawn position
    currentPosition = position;
}

// Function to move the object
void ObjectMesh::MoveObject(glm::vec3 direction, float speed)
{
    // Calculate the displacement
    glm::vec3 displacement = direction * speed * 0.01f;

    // Calculate the new position
    glm::vec3 newPosition = currentPosition + displacement;

    // Create the model matrix
    glm::mat4 model = glm::mat4(1.0f); // Set to identity
    model = glm::translate(model, newPosition); // Change the position of the object

    // Set the model matrix in the shader
    shaderProgram.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    // Update the current position
    currentPosition = newPosition;
}

void ObjectMesh::SetLightPosition(glm::vec3 vec)
{
    // Set the light position in the shader
    shaderProgram.Activate();
    glUniform3fv(glGetUniformLocation(shaderProgram.ID, "lightPos"), 1, glm::value_ptr(vec));
}