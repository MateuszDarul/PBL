#include "Material.h"

Material::Material()
{
    this->diffuse_texture_id = 0;
    this->specular_texture_id = 0;
    this->normal_texture_id = 0;
}

Material::~Material()
{
    glDeleteTextures(1, &this->diffuse_texture_id);
    glDeleteTextures(1, &this->specular_texture_id);
    glDeleteTextures(1, &this->normal_texture_id);

    this->diffuse_texture_id = 0;
    this->specular_texture_id = 0;
    this->normal_texture_id = 0;
}

bool Material::Load(const std::string& mtlPath)
{
    std::string texturesDir = mtlPath;

    while(texturesDir[texturesDir.size() - 1] != '/')
    {
        texturesDir.pop_back();
    }

    std::fstream mtlFile(mtlPath.c_str(), std::ios::in);

    if(!mtlFile.good())
    {
        return false;
    }

    std::string word;
    while(!mtlFile.eof())
    {
        mtlFile >> word;
        if(word == "map_Kd")
        {
            mtlFile >> word;
            LoadTexture(this->diffuse_texture_id, texturesDir + word);
        }
        else if(word == "map_Ks")
        {
            mtlFile >> word;
            LoadTexture(this->specular_texture_id, texturesDir + word);
        }
        else if(word == "map_Bump")
        {
            mtlFile >> word;
            LoadTexture(this->normal_texture_id, texturesDir + word);
        }
    }

    mtlFile.close();

    return true;
}

void Material::LoadTexture(unsigned int &id, const std::string& path)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrComponents;
    //stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if(data)
    {
        GLenum format = GL_RGBA;
        if (nrComponents == 3) format = GL_RGB;
        if (nrComponents == 4) format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Texture failed to load at path: " << path << std::endl;
    }
    stbi_image_free(data);
}