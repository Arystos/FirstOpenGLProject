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

GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
    -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
    -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 1.0f,
     0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	1.0f, 1.0f,
     0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	1.0f, 0.0f,
     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	0.5f, 0.5f
};

// Indices for the vertices
GLuint indices[] =
{
    0, 1, 2,
    0, 2, 3,
    0, 1, 4,
    1, 2, 4,
    2, 3, 4,
    3, 0, 4
};

GLfloat lightVertices[] =
{ //     COORDINATES     //
    -0.1f, -0.1f,  0.1f,
    -0.1f, -0.1f, -0.1f,
     0.1f, -0.1f, -0.1f,
     0.1f, -0.1f,  0.1f,
    -0.1f,  0.1f,  0.1f,
    -0.1f,  0.1f, -0.1f,
     0.1f,  0.1f, -0.1f,
     0.1f,  0.1f,  0.1f
};

GLuint lightIndices[] =
{
    0, 1, 2,
    0, 2, 3,
    0, 4, 7,
    0, 7, 3,
    3, 7, 6,
    3, 6, 2,
    2, 6, 5,
    2, 5, 1,
    1, 5, 4,
    1, 4, 0,
    4, 5, 6,
    4, 6, 7
};

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

void ObjectMesh::CreateMesh()
{
    // Bind the VAO
    vao->Bind();

    VBO vbo = VBO(vertices, sizeof(vertices));
    EBO ebo = EBO(indices, sizeof(indices));
    
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

void ObjectMesh::RenderMesh()
{
    // Render the object mesh
    
    // Activate the shader program
    shaderProgram.Activate();
    // Bind the texture
    texture.Bind();
    // Bind the object VAO
    vao->Bind();
    // Draw the object
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
