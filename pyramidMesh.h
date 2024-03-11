#ifndef PYRAMID_MESH_CLASS_H
#define PYRAMID_MESH_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

class PyramidMesh
{
public:
    PyramidMesh();
    ~PyramidMesh();

    void CreateMesh();
    void RenderMesh();
    void ClearMesh();
    
    int GetIndices();

    VAO* vao;
 

    
    // Create the pyramid mesh
    // Vertices for the pyramid

};

#endif
