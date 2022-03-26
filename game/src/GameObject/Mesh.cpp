#include "Mesh.h"

Mesh::Mesh()
{
    ;
}

Mesh::~Mesh()
{
    ;
}

bool Mesh::Load(const std::string& objPath)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(objPath, 
                                            aiProcess_Triangulate | aiProcess_GenSmoothNormals | 
                                            aiProcess_FlipUVs | aiProcess_CalcTangentSpace
                                            );

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << "\n";
        return false;
    }

    if(scene->mNumMeshes > 1)
    {
        std::cerr << "ERROR::ASSIMP:: Model consist of more than 1 mesh \nLoad only first mesh" << "\n";
    }

    aiMesh* mesh = scene->mMeshes[0];

    Vertex vertex;
    glm::vec3 vector;
    glm::vec2 vec;

    vertices.reserve(mesh->mNumVertices);
    
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        vector = glm::vec3(0,0,0);
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;
            
        if(mesh->HasNormals())
        {
            vector = glm::vec3(0,0,0);
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }
            
        if(mesh->mTextureCoords[0])
        {
            vec = glm::vec3(0,0,0);
            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        }
        else
        {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }
    
    return true;
}