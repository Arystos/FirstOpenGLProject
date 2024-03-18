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

//constexpr char PYRAMID = 0;
constexpr char LIGHT = 1;

ObjectMesh::ObjectMesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices,std::vector<Texture>& textures, Shader& shaderProgram)
    : shaderProgram(shaderProgram), currentPosition(glm::vec3(0.0f)), vertices(vertices), indices(indices),
      textures(textures)
{
    // Constructor
    //this->shaderProgram = shaderProgram;
    //this->vertices = vertices;
    //this->indices = indices;
    vao = new VAO();

    CreateMesh();
}

ObjectMesh::~ObjectMesh()
{
    // Destructor
    vao->Delete();
    textures.clear();
    shaderProgram.Delete();
}

void ObjectMesh::CreateMesh()
{
    // Bind the VAO
    vao->Bind();
    VBO vbo(vertices);
    EBO ebo(indices);

    vao->LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0); // Position
    vao->LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float))); // Color
    vao->LinkAttrib(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float))); // Texture
    vao->LinkAttrib(vbo, 3, 3, GL_FLOAT, sizeof(Vertex), (void*)(8 * sizeof(float))); // Normals

    // Unbind all to prevent accidentally modifying them
    vao->Unbind();
    vbo.Unbind();
    ebo.Unbind();
}

// Render the object mesh
void ObjectMesh::RenderMesh(Camera& camera, GLsizei size)
{
    // Activate the shader program
    shaderProgram.Activate();
    // Bind the object VAO
    vao->Bind();
    
    // Load the texture
    unsigned int numDiffuse = 0;
    unsigned int numSpecular = 0;

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
        {
            number = std::to_string(numDiffuse++);
        }
        else if (name == "texture_specular")
        {
            number = std::to_string(numSpecular++);
        }
        textures[i].texUnit(shaderProgram, (name + number).c_str(), i);
        textures[i].Bind();
    }
    
    // Export the view matrix to the Vertex Shader
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    // Export the camMatrix to the Vertex Shader of the light cube
    camera.Matrix(shaderProgram, "camMatrix");
    // Draw the object
    glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
}

void ObjectMesh::RotateObject(glm::vec3 rotation)
{
    //TODO: Implement the rotation of the object
}


// Function to spawn the object at a certain position
void ObjectMesh::SpawnObject(char type, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
{
    // Create the model matrix
    glm::mat4 model = glm::mat4(1.0f); // Set to identity
    
    glm::vec4 lightColor = glm::vec4(1.0, 1.0, 1.0, 1.0);
    model = translate(model, position); // Change the position of the object
    model = glm::scale(model, scale); // Change the scale of the object
    model = rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate the object on the x axis
    model = rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate the object on the y axis
    model = rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate the object on the z axis

    // Set the model matrix in the shader
    shaderProgram.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    if ((type = LIGHT))
    {
        SetLightColor(lightColor);
    }
    else
    {
        SetLightPosition(position);
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

void ObjectMesh::OrbitObject(glm::vec3 center, float radius, float speed)
{
    // Calculate the new position
    glm::vec3 newPosition;
    newPosition.x = center.x + radius * cos(glfwGetTime() * speed);
    newPosition.y = center.y;
    newPosition.z = center.z + radius * sin(glfwGetTime() * speed);

    // Create the model matrix
    glm::mat4 model = glm::mat4(1.0f); // Set to identity
    model = translate(model, newPosition); // Change the position of the object

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

void ObjectMesh::SetLightColor(glm::vec4 vec)
{
    // Set the light color in the shader
    shaderProgram.Activate();
    glUniform4fv(glGetUniformLocation(shaderProgram.ID, "lightColor"), 1, glm::value_ptr(vec));
}