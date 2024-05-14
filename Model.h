#pragma once
#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include<json/json.h>
#include<ObjectMesh.h>

using json = nlohmann::json;

class Model
{
public:
    Model(const char* file);

    void Draw(Shader& shader, Camera& camera);

private:
    const char* file;
    std::vector<unsigned char> data;
    json JSON;

    // Get the data from the buffer
    std::vector<unsigned char> getData();
    // Getters from the accessor: the accessor is a json object that contains the information of the bufferView
    std::vector<float> getFloats(json accessor);
    std::vector<GLuint> getIndices(json accessor);

    // Combine the vertices, normals and texture coordinates into a vector of vertices
    std::vector<Vertex> assembleVertices
    (
        std::vector<glm::vec3> positions,
        std::vector<glm::vec3> normals,
        std::vector<glm::vec2> textUVs
    );

    // Group the floats into vec3, vec2, vec4
    std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
    std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
    std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);
};

#endif  