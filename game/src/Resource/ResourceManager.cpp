#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
    if (FT_Init_FreeType(&ftLibrary))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        //return -3;
    }
    else
    {
        std::cout << "Successfully initialized FreeType" << std::endl;
    }

}

ResourceManager::~ResourceManager()
{
    FT_Done_FreeType(ftLibrary);

    for(uint32_t i=0; i<loadedMesh.size(); i++)
    {
        delete loadedMesh[i].second;
    }
    loadedMesh.clear();

    for(uint32_t i=0; i<loadedMaterial.size(); i++)
    {
        delete loadedMaterial[i].second;
    }
    loadedMaterial.clear();

    for (uint32_t i = 0; i < loadedFont.size(); i++)
    {
        delete loadedFont[i].second;
    }
    loadedFont.clear();
}

Mesh* ResourceManager::GetMesh(const std::string& path)
{
    size_t searched = std::hash<std::string> {}(path);
    for(uint32_t i=0; i<loadedMesh.size(); i++)
    {
        if(loadedMesh[i].first == searched)
        {
            return loadedMesh[i].second;
        }
    }

    std::pair<size_t, Mesh*> newObject;
    newObject.first = searched;
    newObject.second = new Mesh();

    if(newObject.second->Load(path))
    {
        loadedMesh.push_back(newObject);
        return newObject.second;
    }
    else
    {
        return nullptr;
    }
}

bool ResourceManager::RemoveMesh(const std::string& path)
{
    size_t searched = std::hash<std::string> {}(path);
    for(uint32_t i=0; i<loadedMesh.size(); i++)
    {
        if(loadedMesh[i].first == searched)
        {
            loadedMesh[i].first = 0;
            delete loadedMesh[i].second;
            loadedMesh.erase(loadedMesh.begin() + i);
            
            return true;
        }
    }
    return false;
}

Material* ResourceManager::GetMaterial(const std::string& path)
{
    size_t searched = std::hash<std::string> {}(path);
    for(uint32_t i=0; i<loadedMaterial.size(); i++)
    {
        if(loadedMaterial[i].first == searched)
        {
            return loadedMaterial[i].second;
        }
    }
    
    std::pair<size_t, Material*> newObject;
    newObject.first = searched;
    newObject.second = new Material();

    if(newObject.second->Load(path))
    {
        loadedMaterial.push_back(newObject);
        return newObject.second;
    }
    else
    {
        return nullptr;
    }
}

bool ResourceManager::RemoveMaterial(const std::string& path)
{
    size_t searched = std::hash<std::string> {}(path);
    for(uint32_t i=0; i<loadedMaterial.size(); i++)
    {
        if(loadedMaterial[i].first == searched)
        {
            loadedMaterial[i].first = 0;
            delete loadedMaterial[i].second;
            loadedMaterial.erase(loadedMaterial.begin() + i);
            
            return true;
        }
    }
    return false;
}

Font* ResourceManager::GetFont(const std::string& path)
{
    size_t searched = std::hash<std::string>{}(path);
    for (uint32_t i = 0; i < loadedFont.size(); i++)
    {
        if (loadedFont[i].first == searched)
        {
            return loadedFont[i].second;
        }
    }
    
    std::pair<size_t, Font*> newObject;
    newObject.first = searched;
    newObject.second = new Font();

    if (newObject.second->Load(path, ftLibrary))
    {
        loadedFont.push_back(newObject);
        return newObject.second;
    }
    else
    {
        return nullptr;
    }
}

bool ResourceManager::RemoveFont(const std::string& path)
{
    size_t searched = std::hash<std::string>{}(path);
    for (uint32_t i = 0; i < loadedFont.size(); i++)
    {
        if (loadedFont[i].first == searched)
        {
            loadedFont[i].first = 0;
            delete loadedFont[i].second;
            loadedFont.erase(loadedFont.begin() + i);

            return true;
        }
    }
    return false;
}