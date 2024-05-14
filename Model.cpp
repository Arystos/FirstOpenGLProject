#include "Model.h"

Model::Model(const char* file)
{
    std::string text = get_file_contents(file);
    JSON = json::parse(text);

    Model::file = file;
    data = getData();
}

std::vector<unsigned char> Model::getData()
{
    std::string bytesText;
    std::string uri = JSON["buffers"][0]["uri"];

    std::string fileStr = std::string(file);
    std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of("/") + 1);
    bytesText = get_file_contents((fileDirectory + uri).c_str());

    std::vector<unsigned char> data(bytesText.begin(), bytesText.end());
    return data;
}

std::vector<float> Model::getFloats(json accessor)
{
    std::vector<float> floatVec;

    // Get the information from the accessor
    unsigned int buffViewInd = accessor.value("bufferView", 1); // Index of the bufferView
    unsigned int count = accessor["count"]; // Number of attributes referenced by this accessor
    unsigned int accByteOffset = accessor.value("byteOffset", 0); // Offset relative to the start of the bufferView in bytes
    std::string type = accessor["type"]; // Type of the attributes (scalar, vector or matrix)

    json bufferView = JSON["bufferViews"][buffViewInd];
    unsigned int byteOffset = bufferView["byteOffset"];

    // Get the number of attributes per vertex
    unsigned int numPerVert;
    if (type == "SCALAR")
        numPerVert = 1;
    else if (type == "VEC2")
        numPerVert = 2;
    else if (type == "VEC3")
        numPerVert = 3;
    else if (type == "VEC4")
        numPerVert = 4;
    else
        throw std::invalid_argument("Type is invalid: not SCALAR, VEC2, VEC3, or VEC4");

    // Get the data from the buffer
    unsigned int beginningOfData = byteOffset + accByteOffset;
    unsigned int lenghtOfData = count * 4 * numPerVert;
    for (unsigned int i = beginningOfData; i < beginningOfData + lenghtOfData; i)
    {
        unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] }; // temporary array to store the bytes
        float value;
        std::memcpy(&value, bytes, sizeof(float)); // copy the bytes to the float
        floatVec.push_back(value);
    }

    return floatVec;
}

std::vector<GLuint> Model::getIndices(json accessor)
{
    std::vector<GLuint> indices;

    unsigned int buffViewInd = accessor.value("bufferView", 0);
    unsigned int count = accessor["count"];
    unsigned int accByteOffset = accessor.value("byteOffset", 0);
    unsigned int componentType = accessor["componentType"];

    json bufferView = JSON["bufferViews"][buffViewInd];
    unsigned int byteOffset = bufferView["byteOffset"];

    // Check the data type of the index and read it
    unsigned int beginningOfData = byteOffset + accByteOffset;
    if (componentType == 5125) // 5125 is the component type for unsigned int
    {
        for (unsigned int i = beginningOfData; i < beginningOfData + count * 4; i)
        {
            unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
            GLuint value;
            std::memcpy(&value, bytes, sizeof(unsigned int));
            indices.push_back(value);
        }
    }
    else if (componentType == 5123) // 5123 is the component type for unsigned short
    {
        for (unsigned int i = beginningOfData; i < beginningOfData + count * 2; i)
        {
            unsigned char bytes[] = { data[i++], data[i++] };
            unsigned short value;
            std::memcpy(&value, bytes, sizeof(unsigned short));
            indices.push_back((GLuint)value);
        }
    }
    else if (componentType == 5122) // 5122 is the component type for short
    {
        for (unsigned int i = beginningOfData; i < beginningOfData + count * 2; i)
        {
            unsigned char bytes[] = { data[i++], data[i++] };
            short value;
            std::memcpy(&value, bytes, sizeof(short));
            indices.push_back((GLuint)value);
        }
    }

    return indices;
}

std::vector<Vertex> Model::assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals,
    std::vector<glm::vec2> textUVs)
{
    std::vector<Vertex> vertices;
    for (int i = 0; i < positions.size(); i++)
    {
        vertices.push_back(Vertex{ positions[i], glm::vec3(1.0f, 1.0f, 1.0f), textUVs[i], normals[i] });
        // NOTE: The color is hardcoded to white
    }
    return vertices;
}

std::vector<glm::vec2> Model::groupFloatsVec2(std::vector<float> floatVec)
{
    std::vector<glm::vec2> vectors;
    for (int i = 0; i < floatVec.size(); i)
    {
        vectors.push_back(glm::vec2(floatVec[i++], floatVec[i++]));
    }
    return vectors;
}

std::vector<glm::vec3> Model::groupFloatsVec3(std::vector<float> floatVec)
{
    std::vector<glm::vec3> vectors;
    for (int i = 0; i < floatVec.size(); i)
    {
        vectors.push_back(glm::vec3(floatVec[i++], floatVec[i++], floatVec[i++]));
    }
    return vectors;
}

std::vector<glm::vec4> Model::groupFloatsVec4(std::vector<float> floatVec)
{
    std::vector<glm::vec4> vectors;
    for (int i = 0; i < floatVec.size(); i)
    {
        vectors.push_back(glm::vec4(floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++]));
    }
    return vectors;
}


