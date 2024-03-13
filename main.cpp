#include<iostream>
#include <thread>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Texture.h"
#include"shaderClass.h"
#include"Camera.h"
#include "EBO.h"
#include"ObjectMesh.h"

// Constants
const unsigned int width = 800;
const unsigned int height = 800;

const char PYRAMID = 0;
const char LIGHT = 1;

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
{ //     COORDINATES
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

int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "YoutubeOpenGL", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Create the pyramid
	// Initialize the shader program
	Shader pyramidShader("default.vert", "default.frag");
	// Initialize the texture
	Texture pyramidTexture("Textures/dirt.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	// Create the pyramid object
	ObjectMesh* pyramidObject = new ObjectMesh(pyramidShader, pyramidTexture);
	// Create the pyramid mesh
	pyramidObject->CreateMesh(vertices, PYRAMID, sizeof(vertices), indices, sizeof(indices));

	// TODO: Initialize the light
	// Initialize the shader program
	Shader lightShader("light.vert", "light.frag");
	// Create the light object
	ObjectMesh* lightObject = new ObjectMesh(lightShader, pyramidTexture);
	// Create the light mesh
	lightObject->CreateMesh(lightVertices, LIGHT, sizeof(lightVertices), lightIndices, sizeof(lightIndices));

	/*
	// Create the model matrix
	glm::vec3 lightPos(0.5f, 0.5f, 0.5f); // Position of the light source
	glm::mat4 lightModel = glm::mat4(1.0f); // Set to identity
	lightModel = glm::translate(lightModel, lightPos); // Change the position of the light source

	glm::vec3 pyramidPos(0.0f, 0.0f, 0.0f); // Position of the pyramid
	glm::mat4 pyramidModel = glm::mat4(1.0f); // Set to identity
	pyramidModel = glm::translate(pyramidModel, pyramidPos); // Change the position of the pyramid

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel)); // Set the model matrix in the shader
	pyramidShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(pyramidShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	*/

	// Spawn the pyramid
	pyramidObject->SpawnObject(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0.0f));
	// Spawn the light
	lightObject->SpawnObject(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f), glm::vec3(0.0f));
	

	// Create camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));


	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Update the camera view
		camera.Inputs(window);
		// Update camera
		camera.updateMatrix(45.0f, 0.1f, 100.0f);
		
		// Use the shader program
		pyramidShader.Activate();
		// Update camera matrix
		camera.Matrix(pyramidShader, "camMatrix");
		// Binds texture so that is appears in rendering
		pyramidTexture.Bind();
		// Draw the pyramid
		pyramidObject->RenderMesh(camera, sizeof(indices)/sizeof(int));

		// TODO: Create the rendering for the light
		// Tells OpenGL which Shader Program we want to use
		lightShader.Activate();
		// Export the camMatrix to the Vertex Shader of the light cube
		camera.Matrix(lightShader, "camMatrix");
		// Bind the VAO so OpenGL knows to use it
		lightObject->RenderMesh(camera, sizeof(lightIndices) / sizeof(int));

		// get light position
		glm::vec3 lightPos(0.5f, 0.5f, 0.5f);

		// Update the light's position
		double time = glfwGetTime();
		lightPos.x = sin(time) * 2.0f;
		lightPos.z = cos(time) * 2.0f;

		// Spawn the light at the new position
		lightObject->SpawnObject(lightPos, glm::vec3(1.0f), glm::vec3(0.0f));
		
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	//pyramidTexture.Delete();
	//pyramidShader.Delete();
	
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}
