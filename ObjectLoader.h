#ifndef OBJECT_LOADER_CLASS_H
#define OBJECT_LOADER_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

class ObjectLoader
{
public:
    ObjectLoader();
    ~ObjectLoader();

    bool loadOBJ(const std::string& path);
    void renderOBJ();

private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
};

#endif