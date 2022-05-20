#include "MapLoader.h"

bool MapLoader::Load(std::string path, SceneNode* root, std::shared_ptr<cmp::Shader> shader[2], CollidersManager* collisionManager)
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
            gameObject->AddComponent(std::make_shared<cmp::Name>());
            std::shared_ptr<cmp::Name> nameCmp = gameObject->GetComponent<cmp::Name>();
            {
                std::string name;
                file >> name;
                line_id++;

                nameCmp->Set(name);
            }
        }
        else if(line == "Model:")
        {
            gameObject->AddComponent(shader[0]);

            std::string modelName;

            gameObject->AddComponent(std::make_shared<cmp::Model>());
            std::shared_ptr<cmp::Model> modelCmp = gameObject->GetComponent<cmp::Model>();
            {
                file >> modelName;
                line_id++;

                modelCmp->Create(
                    resMan->GetMesh("Resources/models/" + modelName + ".obj"),
                    resMan->GetMaterial("Resources/models/" + modelName + ".mtl")
                );
            }

            bool useFC;
            file >> std::dec >> useFC;
            line_id++;
            if(useFC)
            {
                gameObject->AddComponent(std::make_shared<cmp::FrustumCulling>());
                gameObject->GetComponent<cmp::FrustumCulling>()->Create(
                    resMan->GetMesh("Resources/models/" + modelName + ".obj")
                );
            }
        }
        else if(line == "Inst:")
        {
            gameObject->AddComponent(shader[1]);

            std::string modelName;
            int amount = 0;

            gameObject->AddComponent(std::make_shared<cmp::ModelInst>());
            std::shared_ptr<cmp::ModelInst> modelCmp = gameObject->GetComponent<cmp::ModelInst>();
            {
                file >> modelName;
                line_id++;

                file >> std::dec >> amount;
                line_id++;

                modelCmp->Create(
                    amount,
                    resMan->GetMesh("Resources/models/" + modelName + ".obj"),
                    resMan->GetMaterial("Resources/models/" + modelName + ".mtl")
                );
            }

            std::string command;
            glm::vec3 position;
            glm::vec3 rotation;
            for(int i=0; i<amount; i++)
            {
                file >> command;

                file >> std::dec >> position.x;
                file >> std::dec >> position.y;
                file >> std::dec >> position.z;

                file >> std::dec >> rotation.x;
                file >> std::dec >> rotation.y;
                file >> std::dec >> rotation.z;

                line_id += 7;

                modelCmp->SetTransformation(i, 
                    cmp::Transform::Transform(position, rotation, 1.f)
                );
            }
            modelCmp->UpdateTransformations();
        }
        else if(line == "Transformations:")
        {
            gameObject->AddComponent(std::make_shared<cmp::Transform>());
            std::shared_ptr<cmp::Transform> transformCmp = gameObject->GetComponent<cmp::Transform>();
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
                gameObject->AddComponent(std::make_shared<BoxCollider>(false, true));
                boxCollider = gameObject->GetComponent<BoxCollider>();
                {
                    glm::vec3 size;
                    file >> std::dec >> size.x;
                    file >> std::dec >> size.y;
                    file >> std::dec >> size.z;
                    line_id += 3;

                    boxCollider->AddToCollidersManager(collisionManager);
                    boxCollider->setLengths(size);
                }
            break;

            case 1: /// SPHERE
                gameObject->AddComponent(std::make_shared<SphereCollider>(false, true));
                sphereCollider = gameObject->GetComponent<SphereCollider>();
                {
                    float size;
                    file >> std::dec >> size;
                    line_id++;

                    sphereCollider->AddToCollidersManager(collisionManager);
                    sphereCollider->SetRadius(size);
                }
            break;
            
            default:
                std::cerr << "Undefined collider type:" << type << " supported [0,1]" << std::endl;
            }
        }
        else if(line == "END")
        {
            root->AddChild(gameObject);
        }
        else
        {
            std::cerr << "Unknown operation !\nline: " << line_id-1 << " | cmd: " << line << std::endl;
        }
    }
    file.close();

    return true;
}