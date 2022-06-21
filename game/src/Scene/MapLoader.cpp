#include "MapLoader.h"
#include "Scripts/PlayerPlaceTurret.h"
#include "Scripts/Blueprint.h"
#include "Scripts/Resource.h"
#include "Scripts/MirrorRotate.h"

#include "ShadowsManager.h"
#include "EnemySpawnerScript.h"

bool MapLoader::Load(
    std::string path, 
    SceneNode* root, 
    std::shared_ptr<cmp::Shader> shader, 
    std::shared_ptr<cmp::Shader> shader_d,
    std::shared_ptr<cmp::Shader> shader_line,
    std::shared_ptr<cmp::Shader> shader_dis,
    std::shared_ptr<cmp::Shader> shader_part_light,
    CollidersManager* collisionManager, 
    ShadowsManager* shadowsManager, 
    Scene* scn,
    MultiToolController* multiToolScript,
    std::shared_ptr<GameObject> p)
{
    int resourceBoxCounter = 0;
    int predefinedLasersCounter = 0;
    int generatorCounter = 0;
    int spawnerCounter = 0;
    int mirrorCounter = 0;
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
                    lightCmp->AddShader(shader_part_light);

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
                std::cout << line_id << " " << modelName << "\n";
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

                // shadeCmp->Create(gameObject->GetComponent<cmp::ModelInst>() == nullptr);
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

            case 2: /// SLOPE
                gameObject->AddComponent(std::make_shared<SlopeCollider>(false, true));
                {
                    auto slopeCollider = gameObject->GetComponent<SlopeCollider>();

                    glm::vec3 size;
                    file >> std::dec >> size.x;
                    file >> std::dec >> size.y;
                    file >> std::dec >> size.z;
                    line_id += 3;
                    
                    int dir = 0;
                    file >> std::dec >> dir;
                    line_id++;

                    SlopeCollider::Direction direction;
                    switch (dir)
                    {
                    case 0:
                        direction = SlopeCollider::Direction::X;
                        break;
                    case 1:
                        direction = SlopeCollider::Direction::X_NEG;
                        break;
                    case 2:
                        direction = SlopeCollider::Direction::Z;
                        break;
                    case 3:
                        direction = SlopeCollider::Direction::Z_NEG;
                        break;
                    default:
                        direction = SlopeCollider::Direction::X;
                    }

                    slopeCollider->SetDimensions(size);
                    slopeCollider->SetDirection(direction);
                    slopeCollider->AddToCollidersManager(collisionManager);
                }
            break;
            
            default:
                std::cerr << "Undefined collider type:" << type << " supported [0,1,2]" << std::endl;
            }
        }
        else if (line == "Resource:")
        {
        gameObject->AddComponent(std::make_shared<cmp::Name>("Resource " + std::to_string(resourceBoxCounter)));
        resourceBoxCounter++;

        gameObject->AddComponent(std::make_shared<BoxCollider>(true, true));
        float y = gameObject->GetComponent<cmp::Transform>()->GetRotation().y;
        glm::vec3 hitbox = glm::vec3(2.5, 2.5, 4.5);
        if ((abs(y) < 135) && (abs(y) > 45)) hitbox = glm::vec3(4.5, 2.5, 2.5);
        gameObject->GetComponent<cmp::BoxCol>()->SetLengths(hitbox);
        gameObject->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collisionManager);

        auto model = std::make_shared<cmp::Model>();
        model->Create(
            resMan->GetMesh("Resources/models/ny/skrzynia2/skrzynia2/skrzynia2.obj"),
            resMan->GetMaterial("Resources/models/ny/skrzynia2/skrzynia2/skrzynia2.mtl")
        );
        gameObject->AddComponent(model);
        gameObject->AddComponent(shader);

        gameObject->AddComponent(std::make_shared<cmp::FrustumCulling>());
        gameObject->GetComponent<cmp::FrustumCulling>()->Create(resMan->GetMesh("Resources/models/Crate/Crate.obj"));

        auto resourceScript = new Resource();
        gameObject->AddComponent(std::make_shared<cmp::Scriptable>());
        gameObject->GetComponent<cmp::Scriptable>()->Add(resourceScript);

        std::string endOrAmount;
        file >> endOrAmount;
        line_id++;
        if (endOrAmount.compare("END") == 0)
        {
            root->AddChild(gameObject);
        }
        else
        {
            resourceScript->energy = std::stoi(endOrAmount);
        }

        auto emissiveGO = std::make_shared<GameObject>();
        emissiveGO->AddComponent(std::make_shared<cmp::Transform>());
        auto emissiveModel = std::make_shared<cmp::Model>();
        emissiveModel->Create(
            resMan->GetMesh("Resources/models/mirror2/mirror2emission.obj"),
            resMan->GetMaterial("Resources/models/multitool/icon.mtl")
        );
        emissiveModel->SetTintColor(0.9, 0.9, 1.0);
        emissiveGO->AddComponent(emissiveModel);
        emissiveGO->AddComponent(shader_d);

        emissiveGO->AddComponent(std::make_shared<cmp::FrustumCulling>());
        emissiveGO->GetComponent<cmp::FrustumCulling>()->Create(resMan->GetMesh("Resources/models/mirror2/mirror2emission.obj"));

        root->AddChild(gameObject)->AddChild(emissiveGO);

        }
        else if (line == "Blueprints:")
        {
            //blueprints
            struct BlueprintPosition {
                glm::vec3 position;
                PlayerPlaceTurret::TurretType type;
            };
            std::vector<BlueprintPosition> blueprints = {
                {{ -73.0f, 1.25f, 89.5f },  PlayerPlaceTurret::TurretType::Blockade  },    //baricade
                {{ -83.0f, 1.25f, 68.8f },  PlayerPlaceTurret::TurretType::Shooting },    //Shooting
                {{ -26.8f, 1.25f,  5.0f },  PlayerPlaceTurret::TurretType::Laser    }     //Laser
            };
            for (int i = 0; i < 3; i++)
            {
                gameObject = std::make_shared<GameObject>();
                gameObject->AddComponent(std::make_shared<cmp::Name>("Blueprint " + std::to_string(i)));

                gameObject->AddComponent(std::make_shared<cmp::Transform>());
                gameObject->GetComponent<cmp::Transform>()->SetPosition(blueprints[i].position);

                if (i == 2)
                {
                    gameObject->GetComponent<cmp::Transform>()->SetRotation(0.0f, 90.0f, 0.0f);
                }

                gameObject->AddComponent(std::make_shared<BoxCollider>(true, true));
                gameObject->GetComponent<cmp::BoxCol>()->SetLengths({ 1.1, 1.1, 1.1 });
                gameObject->GetComponent<cmp::BoxCol>()->SetOffset({0.0, 1.0, 0.0});
                gameObject->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collisionManager);

                auto model = std::make_shared<cmp::Model>();

                //barricade
                if (i == 0)
                {
                    model->Create(
                        resMan->GetMesh("Resources/models/Board/Board.obj"),
                        resMan->GetMaterial("Resources/models/Board/blueprintDefensive.mtl")
                    );
                }

                //shooting
                if (i == 1)
                {
                    gameObject->GetComponent<cmp::BoxCol>()->SetLengths({ 4.0, 2.5, 0.3 });
                    model->Create(
                        resMan->GetMesh("Resources/models/Board/Board.obj"),
                        resMan->GetMaterial("Resources/models/Board/blueprintOffensive.mtl")
                    );
                }

                //laser
                if (i == 2)
                {
                    gameObject->GetComponent<cmp::BoxCol>()->SetLengths({ 0.3, 2.5, 4.0 });
                    model->Create(
                        resMan->GetMesh("Resources/models/Board/Board.obj"),
                        resMan->GetMaterial("Resources/models/Board/blueprintLaser.mtl")
                    );
                }
                gameObject->AddComponent(model);
                gameObject->AddComponent(shader_d);

                gameObject->AddComponent(std::make_shared<cmp::FrustumCulling>());
                gameObject->GetComponent<cmp::FrustumCulling>()->Create(resMan->GetMesh("Resources/models/Board/Board.obj"));

                auto resourceScript = new Blueprint();
                resourceScript->type = blueprints[i].type;
                gameObject->AddComponent(std::make_shared<cmp::Scriptable>());
                gameObject->GetComponent<cmp::Scriptable>()->Add(resourceScript);

                if (i != 2)
                {
                    root->AddChild(gameObject);
                }
            }
        }
        else if (line == "LaserTurret:")
        {
            gameObject->AddComponent(std::make_shared<cmp::Name>("predefLaserTurret" + std::to_string(predefinedLasersCounter)));
            
            auto scriptHolder = std::make_shared<cmp::Scriptable>();
            gameObject->AddComponent(scriptHolder);

            auto turretScript = new TurretLaser();
            turretScript->colMan = collisionManager;

            auto line = std::make_shared<cmp::Line>();
            line->Create();
            line->thickness = 2.0f;
            line->color1 = { 1.0f, 1.0f, 0.0f };
            line->color2 = { 1.0f, 0.7f, 0.0f };

            gameObject->AddComponent(line);
            gameObject->AddComponent(shader_line);
            turretScript->line = line.get();

            scriptHolder->Add(turretScript);

            gameObject->AddComponent(std::make_shared<cmp::BoxCol>(true, true, CollisionLayer::GUI));
            std::shared_ptr<cmp::BoxCol> col = gameObject->GetComponent<cmp::BoxCol>();
            col->SetLengths({ 2.0, 2.5, 2.0 });
           col->SetOffset({ 0.0, 1.75, 0.0 });
            col->AddToCollidersManager(collisionManager);

            auto gfxGO = std::make_shared<GameObject>();
            gfxGO->AddComponent(std::make_shared<cmp::Transform>());
            gfxGO->AddComponent(std::make_shared<cmp::Name>("gfxLaser" + std::to_string(predefinedLasersCounter)));

            auto mc = std::make_shared<cmp::Model>();
            mc->Create(
                resMan->GetMesh("Resources/models/Wieze/laser.obj"),
                resMan->GetMaterial("Resources/models/Wieze/laser.mtl")
            );
            gfxGO->AddComponent(mc);

            gfxGO->AddComponent(std::make_shared<cmp::FrustumCulling>());
            gfxGO->GetComponent<cmp::FrustumCulling>()->Create(
                resMan->GetMesh("Resources/models/Wieze/laser.obj")
            );
            gfxGO->AddComponent(shader);

            root->AddChild(gameObject)->AddChild(gfxGO);

            predefinedLasersCounter++;
        }
        else if (line == "Generator:")
        {
            glm::vec3 position;
            bool isEnabled;
            file >> std::dec >> position.x;
            file >> std::dec >> position.y;
            file >> std::dec >> position.z;
            file >> std::dec >> isEnabled;

            line_id += 4;

            float lightbulbOffset = 2.5f;
            float lightRange = 11.0f;

            auto bulbPos = glm::vec3(position.x, position.y + lightbulbOffset, position.z);

            gameObject->AddComponent(std::make_shared<cmp::Transform>());
            gameObject->GetComponent<cmp::Transform>()->SetPosition(bulbPos);
            gameObject->AddComponent(std::make_shared<cmp::Name>("bulb" + std::to_string(generatorCounter)));

            auto bulbModel = std::make_shared<cmp::Model>();
            bulbModel->Create(
                resMan->GetMesh("Resources/models/Sphere/Sphere.obj"),
                resMan->GetMaterial("Resources/models/wall/wall.mtl")
            );
            gameObject->AddComponent(bulbModel);
            gameObject->AddComponent(shader_d);

            gameObject->AddComponent(std::make_shared<SphereCollider>(false, true, CollisionLayer::GUI));
            gameObject->GetComponent<SphereCollider>()->SetRadius(0.6f);
            gameObject->GetComponent<SphereCollider>()->AddToCollidersManager(collisionManager);


            auto light = std::make_shared<cmp::PointLight>();
            auto lightGO = std::make_shared<GameObject>();
            lightGO->AddComponent(light);
            light->Create();
            light->AddShader(shader);
            //light->AddShader(shader_part_light);
            light->SetPosition(bulbPos);
            light->SetDamping(lightRange);
            light->SetLightColor({ 0.8f, 0.8f, 1.0f });
            shadowsManager->AddLight(lightGO.get());

            if (!isEnabled)
            {
                bulbModel->SetTintColor(0.4f, 0.4f, 0.8f);
                light->SetPosition({ 999, 999, 999 });
            }

            gameObject->AddComponent(std::make_shared<cmp::Scriptable>());

            LightActivator* activator = new LightActivator();
            activator->bulbModel = bulbModel.get();
            activator->lightComponent = light.get();
            activator->isAlwaysLit = isEnabled;
            gameObject->GetComponent<cmp::Scriptable>()->Add(activator);


            auto rangeGO = std::make_shared<GameObject>();
            rangeGO->AddComponent(std::make_shared<cmp::Transform>());
            rangeGO->AddComponent(std::make_shared<cmp::Name>("range" + std::to_string(generatorCounter)));

            rangeGO->AddComponent(std::make_shared<SphereCollider>(true, false, CollisionLayer::Ignore));
            rangeGO->GetComponent<SphereCollider>()->SetRadius(lightRange);
            rangeGO->GetComponent<SphereCollider>()->AddToCollidersManager(collisionManager);

            rangeGO->AddComponent(std::make_shared<cmp::Scriptable>());
            LanternRange* range = new LanternRange();
            range->colMan = collisionManager;
            range->isAlwaysLit = isEnabled;
            range->ChangeLightPower(isEnabled);
            rangeGO->GetComponent<cmp::Scriptable>()->Add(range);

            activator->range = range;

            auto gfxGO = std::make_shared<GameObject>();
            gfxGO->AddComponent(std::make_shared<cmp::Transform>());
            gfxGO->GetComponent<cmp::Transform>()->SetPosition(0.0f, -lightbulbOffset, 0.0f);
            gfxGO->AddComponent(std::make_shared<cmp::Name>("gfxGen" + std::to_string(generatorCounter)));

            auto mc = std::make_shared<cmp::Model>();
            mc->Create(
                resMan->GetMesh("Resources/models/Wieze/Latarnia.obj"),
                resMan->GetMaterial("Resources/models/Wieze/Latarnia.mtl")
            );
            gfxGO->AddComponent(mc);

            gfxGO->AddComponent(std::make_shared<cmp::FrustumCulling>());
            gfxGO->GetComponent<cmp::FrustumCulling>()->Create(
                resMan->GetMesh("Resources/models/Wieze/Latarnia.obj")
            );
            gfxGO->AddComponent(shader);

            generatorCounter++;

            auto node = root->AddChild(gameObject);
            node->AddChild(gfxGO);
            node->AddChild(rangeGO);
            root->AddChild(lightGO);
        }
        else if (line == "Spawner:")
        {
            gameObject->AddComponent(std::make_shared<cmp::Name>("spawner" + std::to_string(spawnerCounter)));

            auto scriptHolder = std::make_shared<cmp::Scriptable>();
            gameObject->AddComponent(scriptHolder);

            auto spawnerScript = new EnemySpawnerScript(root, shader_dis, collisionManager, scn, multiToolScript, p);

            std::string ln;
            file >> std::dec >> ln;
            line_id++;

            glm::vec3 position;

            while (ln == "Node:")
            {
                file >> std::dec >> position.x;
                file >> std::dec >> position.y;
                file >> std::dec >> position.z;
                line_id += 3;

                spawnerScript->AddWayPoint(position);

                file >> std::dec >> ln;
                line_id++;
            }

            std::vector<glm::vec3> waypoints = spawnerScript->GetWayPoints();
            for (int i = 0; waypoints.size() > 1 && i < waypoints.size() - 1; i++)
            {
                float wDistance = glm::distance(waypoints[i], waypoints[i + 1]);
                auto sParticlesGO = std::make_shared<GameObject>();
                sParticlesGO->AddComponent(std::make_shared<cmp::Transform>());
                sParticlesGO->GetComponent<cmp::Transform>()->SetPosition(waypoints[i]);
                sParticlesGO->AddComponent(std::make_shared<cmp::Particles>());
                auto sParticles = sParticlesGO->GetComponent<cmp::Particles>();
                sParticles->Create(p->GetComponent<cmp::Camera>(), false, 30, shader_part_light);
                sParticles->SetTexture("Resources/textures/shadowParticle.png");
                sParticles->SetSpeed(3.0f);
                float lifeTime = wDistance / sParticles->GetSpeed();
                sParticles->SetParticlesPerSecond(30 / lifeTime);
                sParticles->SetOffset(glm::vec3(0.0f, 0.0f, 0));
                sParticles->SetDirection(glm::normalize(waypoints[i+1] - waypoints[i]));
                sParticles->SetDirectionVar(0);
                sParticles->SetSpeed(3.0f);
                sParticles->SetParticleLifetime(lifeTime);
                sParticles->SetScale(0.25f, 0.25f);
                sParticles->SetColor({ 1.0f, 1.0f, 1.0f,   1.0f }, { 1.0f, 1.0f, 1.0f,   1.0f });
                sParticles->SetForce({ 0.0f, -0.01f, 0.0f });
                root->AddChild(sParticlesGO);
            }
            scriptHolder->Add(spawnerScript);

            spawnerCounter++;
        }
        else if (line == "Mirror:")
        {
        /*NEW
        Transformations:
        x
        y
        z
        rx      - obrot gora/dol
        ry      - obrot lewo/prawo
        0.0     - zawsze 0
        Mirror:
        Offset:
        x       - przesuniecie punktu obrotu w lewo/prawo
        y       - przesuniecie punktu obrotu w dol/gore
        z       - przesuniecie punktu obrotu do przodu/tylu
        Facing:
        x       - obrot gora/dol w pozycji zerowej
        y       - obrot lewo/prawo w pozycji zerowej
        MaxRot:
        x       - maksymalne wychylenie w gore/dol
        y       - maksymalne wychylenie w lewo/prawo
        END*/

        glm::vec3 offset, initialRot, maxRot;

        std::string ln;
        file >> ln;
        line_id++;
        while (ln != "END" && ln != "")
        {
            if (ln == "Offset:")
            {
                file >> std::dec >> offset.x;
                file >> std::dec >> offset.y;
                file >> std::dec >> offset.z;

                file >> ln;
                line_id += 4;
            }
            if (ln == "Facing:")
            {
                file >> std::dec >> initialRot.x;
                file >> std::dec >> initialRot.y;

                file >> ln;
                line_id += 3;
            }
            if (ln == "MaxRot:")
            {
                file >> std::dec >> maxRot.x;
                file >> std::dec >> maxRot.y;

                file >> ln;
                line_id += 3;
            }
        }

        gameObject->AddComponent(std::make_shared<cmp::Name>("MirrorHolder" + std::to_string(mirrorCounter)));

        auto mirrorGO = std::make_shared<GameObject>();
        mirrorGO->AddComponent(std::make_shared<cmp::Name>("MirrorCol" + std::to_string(mirrorCounter)));

        mirrorGO->AddComponent(std::make_shared<BoxCollider>(false, true, CollisionLayer::Mirror));
        mirrorGO->GetComponent<cmp::BoxCol>()->SetLengths({ 1.7, 2.4, 0.1515 });
        mirrorGO->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collisionManager);

        mirrorGO->AddComponent(std::make_shared<cmp::Transform>());
        mirrorGO->GetComponent<cmp::Transform>()->SetPosition(offset);


        auto model = std::make_shared<cmp::Model>();
        model->Create(
            resMan->GetMesh("Resources/models/mirror2/mirror2alt.obj"),
            resMan->GetMaterial("Resources/models/mirror2/mirror2.mtl")
        );
        model->SetTintColor(0.85, 0.68, 0.30);
        mirrorGO->AddComponent(model);
        mirrorGO->AddComponent(shader);

        mirrorGO->AddComponent(std::make_shared<cmp::FrustumCulling>());
        mirrorGO->GetComponent<cmp::FrustumCulling>()->Create(resMan->GetMesh("Resources/models/mirror2/mirror2alt.obj"));


        gameObject->AddComponent(std::make_shared<cmp::Scriptable>());

        auto mirrorScript = new MirrorRotate();
        mirrorScript->SetEnabled(false);
        mirrorScript->initialRotationOffsetX = initialRot.x;
        mirrorScript->initialRotationOffsetY = initialRot.y;
        mirrorScript->maxRotationX = maxRot.x;
        mirrorScript->maxRotationY = maxRot.y;
        gameObject->GetComponent<cmp::Scriptable>()->Add(mirrorScript);


        auto emissiveGO = std::make_shared<GameObject>();
        emissiveGO->AddComponent(std::make_shared<cmp::Transform>());
        auto emissiveModel = std::make_shared<cmp::Model>();
        emissiveModel->Create(
            resMan->GetMesh("Resources/models/mirror2/mirror2emission.obj"),
            resMan->GetMaterial("Resources/models/multitool/icon.mtl")
        );
        emissiveModel->SetTintColor(0.9, 0.9, 1.0);
        emissiveGO->AddComponent(emissiveModel);
        emissiveGO->AddComponent(shader_d);

        emissiveGO->AddComponent(std::make_shared<cmp::FrustumCulling>());
        emissiveGO->GetComponent<cmp::FrustumCulling>()->Create(resMan->GetMesh("Resources/models/mirror2/mirror2emission.obj"));


        root->AddChild(gameObject)->AddChild(mirrorGO)->AddChild(emissiveGO);
        mirrorCounter++;
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