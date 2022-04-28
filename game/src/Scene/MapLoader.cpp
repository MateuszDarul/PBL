#include "MapLoader.h"

bool MapLoader::Load(std::string path, SceneNode* root, std::shared_ptr<cmp::Shader> shader, CollidersManager* collisionManager)
{
    ResourceManager* resMan = GameApplication::GetResourceManager();

    std::shared_ptr<GameObject> gameObject;

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
        else if(line == "Collider:")
        {
            int type = 0;
            file >> std::dec >> type;
            line_id++;

            std::shared_ptr<BoxCollider> boxCollider = nullptr;
            std::shared_ptr<SphereCollider> sphereCollider = nullptr;

            switch(type)
            {
            case 0: /// BOX
                boxCollider = std::make_shared<BoxCollider>(false, true);
                {
                    glm::vec3 size;
                    file >> std::dec >> size.x;
                    file >> std::dec >> size.y;
                    file >> std::dec >> size.z;
                    line_id += 3;

                    boxCollider->AddToCollidersManager(collisionManager);
                    boxCollider->setLengths(size);
                }
                gameObject->AddComponent(boxCollider);
            break;

            case 1: /// SPHERE
                sphereCollider = std::make_shared<SphereCollider>(false, true);
                {
                    float size;
                    file >> std::dec >> size;
                    line_id++;

                    sphereCollider->AddToCollidersManager(collisionManager);
                    sphereCollider->SetRadius(size);
                }
                gameObject->AddComponent(sphereCollider);
            break;
            
            default:
                std::cerr << "Undefined collider type:" << type << " supported [0,1]" << std::endl;
            }
        }
        else if(line == "END")
        {
            gameObject->AddComponent(shader);
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