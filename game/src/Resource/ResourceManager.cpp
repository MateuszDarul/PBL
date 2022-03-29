#include "ResourceManager.h"

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{
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