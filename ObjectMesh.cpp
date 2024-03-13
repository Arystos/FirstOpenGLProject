#include "ObjectMesh.h"
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"

ObjectMesh::ObjectMesh(Shader shaderProgram, Texture texture): shaderProgram(shaderProgram), texture(texture)
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

void ObjectMesh::CreateMesh(GLfloat* vertices, char type, GLsizeiptr verticesSize, GLuint* indices, GLsizeiptr indicesSize)
{
    // Bind the VAO
    vao->Bind();
    VBO vbo = VBO(vertices, verticesSize);
    EBO ebo = EBO(indices, indicesSize);

    // if case for pyramid and light
    if (type == 0) // Pyramid
    {
        // Links vbo attributes to vao
        // Position of the cube
        vao->LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
        // Color of the cube
        vao->LinkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        // Texture coordinates of the cube
        vao->LinkAttrib(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

        // Set the texture wrapping/filtering options (on the currently bound texture object)
        texture.texUnit(shaderProgram, "tex0", 0);
    }
    else if (type == 1) // Light
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
    try
    {
        shaderProgram.Activate();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    // Bind the texture
    try
    {
        texture.Bind();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    // Bind the object VAO
    vao->Bind();
    // camera matrix
    try
    {
        camera.Matrix(shaderProgram, "camMatrix");
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    // Draw the object
    try
    {
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

// Function to spawn the object at a certain position
void ObjectMesh::SpawnObject(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
{
    // Create the model matrix
    glm::mat4 model = glm::mat4(1.0f); // Set to identity
    model = glm::translate(model, position); // Change the position of the object
    model = glm::scale(model, scale); // Change the scale of the object
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate the object on the x axis
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate the object on the y axis
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate the object on the z axis

    // Set the model matrix in the shader
    shaderProgram.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

// Function to translate the object (this function would be applied in the while loop to move the object)
void ObjectMesh::Translate(glm::vec3 position)
{
    // Create the model matrix
    glm::mat4 model = glm::mat4(1.0f); // Set to identity
    model = glm::translate(model, position); // Change the position of the object

    // Set the model matrix in the shader
    shaderProgram.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
}