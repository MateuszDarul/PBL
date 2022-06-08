#include "MapLoader.h"

#include "ShadowsManager.h"

bool MapLoader::Load(std::string path, SceneNode* root, std::shared_ptr<cmp::Shader> shader, CollidersManager* collisionManager, ShadowsManager* shadowsManager)
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
        else if(line == "Light:")
        {
            std::string type;
            file >> type;
            line_id++;

            std::string cmd;

            if(type == "Point")
            {
                gameObject->AddComponent(std::make_shared<cmp::PointLight>());
                std::shared_ptr<cmp::PointLight> lightCmp = gameObject->GetComponent<cmp::PointLight>();
                {
                    lightCmp->Create();
                    lightCmp->AddShader(shader);

                    for(int i=0; i<4; i++)
                    {
                        file >> cmd;
                        line_id++;
                        if(cmd == "Position:")
                        {
                            glm::vec3 position;
                            file >> position.x;
                            file >> position.y;
                            file >> position.z;
                            line_id+=3;
                            lightCmp->SetPosition(position);
                        }
                        else if(cmd == "Color:")
                        {
                            glm::vec3 color;
                            file >> color.x;
                            file >> color.y;
                            file >> color.z;
                            line_id+=3;
                            lightCmp->SetLightColor(color);
                        }
                        else if(cmd == "SpecularColor:")
                        {
                            glm::vec3 color;
                            file >> color.x;
                            file >> color.y;
                            file >> color.z;
                            line_id+=3;
                            lightCmp->SetSpecularColor(color);
                        }
                        else if(cmd == "Damping:")
                        {
                            float damping;
                            file >> damping;
                            line_id++;
                            lightCmp->SetDamping(damping);
                        }
                        else
                        {
                            std::cerr << "Light error | Point | ";
                            std::cerr << "Command: " << cmd << " | ";
                            std::cerr << "Line: " << line_id-1 << std::endl;
                        }
                    }
                }
                shadowsManager->AddLight(gameObject.get());
            }
            else if(type == "Spot")
            {
                gameObject->AddComponent(std::make_shared<cmp::SpotLight>());
                std::shared_ptr<cmp::SpotLight> lightCmp = gameObject->GetComponent<cmp::SpotLight>();
                {
                    lightCmp->Create();
                    lightCmp->AddShader(shader);

                    for(int i=0; i<6; i++)
                    {
                        file >> cmd;
                        line_id++;
                        if(cmd == "Position:")
                        {
                            glm::vec3 position;
                            file >> position.x;
                            file >> position.y;
                            file >> position.z;
                            line_id+=3;
                            lightCmp->SetPosition(position);
                        }
                        else if(cmd == "Direction:")
                        {
                            glm::vec3 direction;
                            file >> direction.x;
                            file >> direction.y;
                            file >> direction.z;
                            line_id+=3;
                            lightCmp->SetDirection(direction);
                        }
                        else if(cmd == "Rotation:")
                        {
                            glm::vec3 rotation;
                            file >> rotation.x;
                            file >> rotation.y;
                            file >> rotation.z;
                            line_id+=3;
                            lightCmp->Rotate(rotation);
                        }
                        else if(cmd == "Color:")
                        {
                            glm::vec3 color;
                            file >> color.x;
                            file >> color.y;
                            file >> color.z;
                            line_id+=3;
                            lightCmp->SetLightColor(color);
                        }
                        else if(cmd == "SpecularColor:")
                        {
                            glm::vec3 color;
                            file >> color.x;
                            file >> color.y;
                            file >> color.z;
                            line_id+=3;
                            lightCmp->SetSpecularColor(color);
                        }
                        else if(cmd == "CutOff:")
                        {
                            glm::vec2 cutOff;
                            file >> cutOff.x;
                            file >> cutOff.y;
                            line_id+=2;
                            lightCmp->SetCutOff(cutOff);
                        }
                        else if(cmd == "Damping:")
                        {
                            float damping;
                            file >> damping;
                            line_id++;
                            lightCmp->SetDamping(damping);
                        }
                        else
                        {
                            std::cerr << "Light error | Spot | ";
                            std::cerr << "Command: " << cmd << " | ";
                            std::cerr << "Line: " << line_id-1 << std::endl;
                        }
                    }
                }
            }
        }
        else if(line == "Model:")
        {
            gameObject->AddComponent(shader);

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
            gameObject->AddComponent(shader);

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
        else if(line == "Shade:")
        {
            gameObject->AddComponent(std::make_shared<cmp::Shade>());
            std::shared_ptr<cmp::Shade> shadeCmp = gameObject->GetComponent<cmp::Shade>();
            {
                int type = 0;
                file >> std::dec >> type;
                line_id++;

                shadeCmp->Create(type);
            }
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
                    boxCollider->SetLengths(size);
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