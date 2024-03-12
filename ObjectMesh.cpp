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

void ObjectMesh::CreateMesh(GLfloat* vertices, GLsizeiptr verticesSize, GLuint* indices, GLsizeiptr indicesSize)
{
    // Bind the VAO
    vao->Bind();
    VBO vbo = VBO(vertices, verticesSize);
    EBO ebo = EBO(indices, indicesSize);

    // Links vbo attributes to vao
    // Position of the cube
    vao->LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    // Color of the cube
    vao->LinkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // Texture coordinates of the cube
    vao->LinkAttrib(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    // Set the texture wrapping/filtering options (on the currently bound texture object)
    texture.texUnit(shaderProgram, "tex0", 0);

    // Unbind all to prevent accidentally modifying them
    vao->Unbind();
    vbo.Unbind();
    ebo.Unbind();
}

void ObjectMesh::RenderMesh(GLsizei size)
{
    // Render the object mesh
    
    // Activate the shader program
    try
    {
        shaderProgram.Activate();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    // Bind the texture
    try
    {
        texture.Bind();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    // Bind the object VAO
    vao->Bind();
    // Draw the object
    try
    {
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}
