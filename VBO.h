#pragma once

#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Color;
	glm::vec2 TexCoords;
	glm::vec3 Normals;
};

class VBO
{
	public:
		GLuint ID;
		VBO(std::vector<Vertex>& vertices);

		void Bind();
		void Unbind();
		void Delete();
		void Create(GLfloat* vertices, GLsizeiptr size);
};


#endif