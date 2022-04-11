#include "MapLoader.h"

bool MapLoader::Load(std::string path, SceneNode* root)
{
    ResourceManager* resMan = GameApplication::GetResourceManager();

    std::shared_ptr<GameObject> gameObject;
    std::shared_ptr<cmp::Name> nameCmp;
    std::shared_ptr<cmp::Model> modelCmp;
    std::shared_ptr<cmp::Transform> transformCmp;
    std::shared_ptr<cmp::Shader> shaderCmp = std::make_shared<cmp::Shader>();
    shaderCmp->Create("Resources/shaders/default.vert", "Resources/shaders/default.frag");

    std::string line;
    uint32_t line_id = 0;

    std::fstream file;
    file.open(path.c_str(), std::ios::in);

    if(!file.good())
    {
        file.close();
        std::cerr << "Can not open file" << std::endl;
        return false;
    }

    while(!file.eof())
    {
        file >> line;
        line_id++;

        if(line == "NEW")
        {
            gameObject = std::make_shared<GameObject>();
            nameCmp = std::make_shared<cmp::Name>();
            modelCmp = std::make_shared<cmp::Model>();
            transformCmp = std::make_shared<cmp::Transform>();
        }
        else if(line == "Name:")
        {
            std::string name;

            file >> name;
            line_id++;

            nameCmp->Set(name);
        }
        else if(line == "Path:")
        {
            std::string modelName;

            file >> modelName;
            line_id++;

            modelCmp->Create(
                resMan->GetMesh("Resources/models/" + modelName + ".obj"),
                resMan->GetMaterial("Resources/models/" + modelName + ".mtl")
            );
        }
        else if(line == "Position:")
        {
            glm::vec3 position;

            file >> std::dec >> position.x;
            file >> std::dec >> position.y;
            file >> std::dec >> position.z;
            line_id += 3;

            transformCmp->SetPosition(position);
        }
        else if(line == "Rotation:")
        {
            glm::vec3 rotation;

            file >> std::dec >> rotation.x;
            file >> std::dec >> rotation.y;
            file >> std::dec >> rotation.z;
            line_id += 3;

            transformCmp->SetRotation(rotation);
        }
        else if(line == "END")
        {
            gameObject->AddComponent(nameCmp);
            gameObject->AddComponent(modelCmp);
            gameObject->AddComponent(shaderCmp);
            gameObject->AddComponent(transformCmp);
            root->AddChild(gameObject);
        }
        else
        {
            std::cerr << "Unknown operation !\nline: " << line_id << " | cmd: " << line << std::endl;
        }
    }
    file.close();

    return true;
}