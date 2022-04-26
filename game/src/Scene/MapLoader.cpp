#include "MapLoader.h"

bool MapLoader::Load(std::string path, SceneNode* root)
{
    ResourceManager* resMan = GameApplication::GetResourceManager();

    std::shared_ptr<GameObject> gameObject;
    std::shared_ptr<cmp::Shader> shaderCmp = std::make_shared<cmp::Shader>();
    shaderCmp->Create("Resources/shaders/default.vert", "Resources/shaders/default.frag");

    std::string line;
    uint32_t line_id = 1;

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
        }
        else if(line == "Name:")
        {
            std::shared_ptr<cmp::Name> nameCmp = std::make_shared<cmp::Name>();
            {
                std::string name;

                file >> name;
                line_id++;

                nameCmp->Set(name);
            }
            gameObject->AddComponent(nameCmp);
        }
        else if(line == "Path:")
        {
            std::shared_ptr<cmp::Model> modelCmp = std::make_shared<cmp::Model>();
            {
                std::string modelName;

                file >> modelName;
                line_id++;

                modelCmp->Create(
                    resMan->GetMesh("Resources/models/" + modelName + ".obj"),
                    resMan->GetMaterial("Resources/models/" + modelName + ".mtl")
                );
            }
            gameObject->AddComponent(modelCmp);
        }
        else if(line == "Transformations:")
        {
            std::shared_ptr<cmp::Transform> transformCmp = std::make_shared<cmp::Transform>();
            {
                glm::vec3 position;
                file >> std::dec >> position.x;
                file >> std::dec >> position.y;
                file >> std::dec >> position.z;

                glm::vec3 rotation;
                file >> std::dec >> rotation.x;
                file >> std::dec >> rotation.y;
                file >> std::dec >> rotation.z;

                line_id += 6;

                transformCmp->SetPosition(position);
                transformCmp->SetRotation(rotation);
            }
            gameObject->AddComponent(transformCmp);
        }
        else if(line == "END")
        {
            gameObject->AddComponent(shaderCmp);
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