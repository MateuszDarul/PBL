#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <iostream>
#include <fstream>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image.h>

class Material
{
public:
    unsigned int diffuse_texture_id;
    unsigned int specular_texture_id;
    unsigned int normal_texture_id;

private:
    void LoadTexture(unsigned int &id, const std::string& path);

public:
    Material();
    ~Material();

    bool Load(const std::string& mtlPath);
    bool Load(const std::string& diffusePath, const std::string& specularPath, const std::string& normalPath);
};

#endif // __MATERIAL_H__