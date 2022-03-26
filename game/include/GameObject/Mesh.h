#ifndef __MESH_H__
#define __MESH_H__

#include <iostream>

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh
{
public:
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
    };

public:
    std::vector<Vertex> vertices;

public:
    Mesh();
    ~Mesh();

    bool Load(const std::string& objPath);
}; 

#endif // __MESH_H__