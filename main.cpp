#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Texture.h"
#include"shaderClass.h"
#include"Camera.h"
#include"ObjectMesh.h"

// Constants
constexpr unsigned int width = 800;
constexpr unsigned int height = 800;
constexpr char PYRAMID = 0;
constexpr char LIGHT = 1;

// Vertex ddata
Vertex verticesV [] =
{
	// Positions, Colors, Texture Coords, Normals
	Vertex{glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)},
	Vertex{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(0.0f, 5.0f), glm::vec3(0.0f, -1.0f, 0.0f)},
	Vertex{glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(5.0f, 5.0f), glm::vec3(0.0f, -1.0f, 0.0f)},
	Vertex{glm::vec3(0.5f, 0.0f,  0.5f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(5.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)},

	Vertex{glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(0.0f, 0.0f), glm::vec3(-0.8f, 0.5f,  0.0f)},
	Vertex{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(5.0f, 0.0f), glm::vec3(-0.8f, 0.5f,  0.0f)},
	Vertex{glm::vec3(0.0f, 0.8f,  0.0f), glm::vec3(0.92f, 0.86f, 0.76f), glm::vec2(2.5f, 5.0f), glm::vec3(-0.8f, 0.5f,  0.0f)},

	Vertex{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(5.0f, 0.0f), glm::vec3(0.0f, 0.5f, -0.8f)},
	Vertex{glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.5f, -0.8f)},
	Vertex{glm::vec3(0.0f, 0.8f,  0.0f), glm::vec3(0.92f, 0.86f, 0.76f), glm::vec2(2.5f, 5.0f), glm::vec3(0.0f, 0.5f, -0.8f)},

	Vertex{glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(0.0f, 0.0f), glm::vec3(0.8f, 0.5f,  0.0f)},
	Vertex{glm::vec3(0.5f, 0.0f,  0.5f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(5.0f, 0.0f), glm::vec3(0.8f, 0.5f,  0.0f)},
	Vertex{glm::vec3(0.0f, 0.8f,  0.0f), glm::vec3(0.92f, 0.86f, 0.76f), glm::vec2(2.5f, 5.0f), glm::vec3(0.8f, 0.5f,  0.0f)},

	Vertex{glm::vec3(0.5f, 0.0f,  0.5f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(5.0f, 0.0f), glm::vec3(0.0f, 0.5f,  0.8f)},
	Vertex{glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.5f,  0.8f)},
	Vertex{glm::vec3(0.0f, 0.8f,  0.0f), glm::vec3(0.92f, 0.86f, 0.76f), glm::vec2(2.5f, 5.0f), glm::vec3(0.0f, 0.5f,  0.8f)}
};

// Indices for vertices order
GLuint indices[18] =
{
	0, 1, 2, // Bottom side
	0, 2, 3, // Bottom side
	4, 6, 5, // Left sideki
	7, 9, 8, // Non-facing side
	10, 12, 11, // Right side
	13, 15, 14 // Facing side
};

// Light data
Vertex lightVertiesV[]
{
	// Positions, Colors, Texture Coords, Normals
	Vertex{glm::vec3(-0.1f, -0.1f,  0.1f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-0.1f, -0.1f, -0.1f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 5.0f)},
	Vertex{glm::vec3(0.1f, -0.1f, -0.1f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(5.0f, 5.0f)},
	Vertex{glm::vec3(0.1f, -0.1f,  0.1f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(5.0f, 0.0f)},
	Vertex{glm::vec3(-0.1f,  0.1f,  0.1f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-0.1f,  0.1f, -0.1f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 5.0f)},
	Vertex{glm::vec3(0.1f,  0.1f, -0.1f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(5.0f, 5.0f)},
	Vertex{glm::vec3(0.1f,  0.1f,  0.1f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(5.0f, 0.0f)}
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
		std::cout << "Failed to create GLFW window" << '\n';
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

	Texture textures[] =
	{
		Texture("Textures/dirt.png", "diffuse", 0, GL_RGB, GL_UNSIGNED_BYTE),
		Texture("Textures/dirtSpecular.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
	};

	// Create the pyramid
	// Initialize the shader program
	Shader pyramidShader("default.vert", "default.frag");
	// Initialize the texture
	
	// Create the pyramid object
	std::vector<Vertex> verticesVector(std::begin(verticesV), std::end(verticesV));
	std::vector<GLuint> indicesVector(std::begin(indices), std::end(indices));
	std::vector<Texture> pyramidTexture(std::begin(textures), std::end(textures));
	ObjectMesh* pyramidObject = new ObjectMesh(verticesVector, indicesVector, pyramidTexture, pyramidShader);

	// Create the light
	// Initialize the shader program
	Shader lightShader("light.vert", "light.frag");
	// Create the light object
	std::vector<Vertex> lightVerticesVector(std::begin(lightVertiesV), std::end(lightVertiesV));
	std::vector<GLuint> lightIndicesVector(std::begin(lightIndices), std::end(lightIndices));
	ObjectMesh* lightObject = new ObjectMesh(lightVerticesVector, lightIndicesVector, pyramidTexture, lightShader);
	// Create the light mesh
	lightObject->CreateMesh();

	glm::vec3 lightPos = glm::vec3(0.0f, 1.0f, 0.0f);

	// Spawn the light
	lightObject->SpawnObject(LIGHT, lightPos, glm::vec3(1.0f), glm::vec3(90.0f));
	// Spawn the pyramid
	pyramidObject->SpawnObject(PYRAMID, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0.0f));
	
	// Pass the light position to the fragment shader
	pyramidObject->SetLightPosition(lightPos);

	
	// Create camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.5f, 3.0f));
	
	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Update the camera view
		camera.Inputs(window);
		// Update camera (projection, near plane, far plane)
		camera.updateMatrix(45.0f, 0.1f, 100.0f);
		
		// Bind the VAO so OpenGL knows to use it
		lightObject->RenderMesh(camera, sizeof(lightIndices) / sizeof(int));
		// Orbit the light object on the y axis
		lightObject->OrbitObject(glm::vec3(0.0f, 0.5f, 0.0f), 1.0f, 0.7f);
		
		// Draw the pyramid
		pyramidObject->RenderMesh(camera, sizeof(indices)/sizeof(int));
		// Update the light position
		pyramidObject->SetLightPosition(lightObject->currentPosition);
		
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete the objects
	pyramidObject->~ObjectMesh();
	lightObject->~ObjectMesh();
	
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}