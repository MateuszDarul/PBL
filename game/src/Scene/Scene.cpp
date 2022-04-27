#include "Scene.h"

#include "Scripts/TestScript.h"
#include "Scripts/StatsScript.h"

Scene::Scene()
{
    glfwSetInputMode(GameApplication::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    ResourceManager* resMan = GameApplication::GetResourceManager();

    std::shared_ptr<ShaderComponent> shader_d = std::make_shared<ShaderComponent>();
    shader_d->Create("Resources/shaders/default.vert", "Resources/shaders/default.frag");
    std::shared_ptr<ShaderComponent> shader_i = std::make_shared<ShaderComponent>();
    shader_i->Create("Resources/shaders/inst.vert", "Resources/shaders/inst.frag");
    std::shared_ptr<ShaderComponent> shader_l = std::make_shared<ShaderComponent>();
    shader_l->Create("Resources/shaders/light.vert", "Resources/shaders/light.frag");

    std::shared_ptr<cmp::Model> mc = nullptr;
    std::shared_ptr<cmp::ModelInst> mic = nullptr;
    std::shared_ptr<GameObject> go = nullptr;

    world = new SceneNode(std::make_shared<GameObject>());
    world->GetGameObject()->AddComponent(std::make_shared<cmp::Name>("ROOT"));
    world->GetGameObject()->AddComponent(std::make_shared<cmp::Transform>());

    go = std::make_shared<GameObject>();
    go->AddComponent(std::make_shared<cmp::PointLight>());
    go->GetComponent<cmp::PointLight>()->Create();
    go->GetComponent<cmp::PointLight>()->position = glm::vec3(0, 5, 0);
    go->AddComponent(std::make_shared<cmp::Name>("light1"));
    go->AddComponent(shader_l);
    world->AddChild(go);
    go = nullptr;

    go = std::make_shared<GameObject>();
    go->AddComponent(std::make_shared<cmp::PointLight>());
    go->GetComponent<cmp::PointLight>()->Create();
    go->GetComponent<cmp::PointLight>()->position = glm::vec3(10, 3, 0);
    go->AddComponent(std::make_shared<cmp::Name>("light2"));
    go->AddComponent(shader_l);
    world->AddChild(go);
    go = nullptr;

    go = std::make_shared<GameObject>();
    go->AddComponent(std::make_shared<CameraComponent>());
    go->GetComponent<cmp::Camera>()->Create(glm::vec3(0,3,10));
    go->GetComponent<cmp::Camera>()->SetSpeed(5);
    go->AddComponent(std::make_shared<cmp::Name>("Camera"));
    world->AddChild(go);
    go = nullptr;

    MapLoader::Load("Resources/maps/world.map", world, shader_l);

    ///***

    mc = std::make_shared<cmp::Model>();
    mc->Create(resMan->GetMesh("Resources/models/Crate/Crate.obj"),
                resMan->GetMaterial("Resources/models/Crate/Crate.mtl")
                );
    go = std::make_shared<GameObject>();
    go->AddComponent(mc);
    mc = nullptr;
    go->AddComponent(std::make_shared<cmp::Transform>());
    go->AddComponent(std::make_shared<cmp::Name>("Cos"));
    go->AddComponent(shader_l);
    world->AddChild(go);
    go = nullptr;


    mc = std::make_shared<cmp::Model>();
    mc->Create(resMan->GetMesh("Resources/models/NWM/NWM.obj"),
                resMan->GetMaterial("Resources/models/NWM/NWM.mtl")
                );
    go = std::make_shared<GameObject>();
    go->AddComponent(mc);
    mc = nullptr;
    go->AddComponent(std::make_shared<cmp::Transform>());
    go->AddComponent(std::make_shared<cmp::Name>("mark1"));
    go->AddComponent(shader_d);
    world->FindNode("light1")->AddChild(go);
    go = nullptr;

    mc = std::make_shared<cmp::Model>();
    mc->Create(resMan->GetMesh("Resources/models/NWM/NWM.obj"),
                resMan->GetMaterial("Resources/models/NWM/NWM.mtl")
                );
    go = std::make_shared<GameObject>();
    go->AddComponent(mc);
    mc = nullptr;
    go->AddComponent(std::make_shared<cmp::Transform>());
    go->AddComponent(std::make_shared<cmp::Name>("mark2"));
    go->AddComponent(shader_d);
    world->FindNode("light2")->AddChild(go);
    go = nullptr;


    world->FindNode("Cos")->GetLocalTransformations()->SetPosition(0,3,0);
    world->FindNode("mark1")->GetLocalTransformations()->SetPosition(0,5,0);
    world->FindNode("mark1")->GetLocalTransformations()->SetScale(0.1);
    world->FindNode("mark2")->GetLocalTransformations()->SetPosition(10,3,0);
    world->FindNode("mark2")->GetLocalTransformations()->SetScale(0.1);
}

Scene::~Scene()
{
    delete world;
    world = nullptr;
}


float h_y = 1;
void Scene::Update(float dt)
{
    world->FindNode("Camera")->GetGameObject()->GetComponent<cmp::Camera>()->Update(GameApplication::GetInputManager(), dt);
    
    transform = GameApplication::GetProjection();
    transform *= world->FindNode("Camera")->GetGameObject()->GetComponent<cmp::Camera>()->GetView();
    
    world->Update(dt);

    if(GameApplication::GetInputManager()->Keyboard()->IsPressed(KeyboardKey::Z))
    {
        h_y += 1*dt;
        world->FindNode("light1")->GetGameObject()->GetComponent<cmp::PointLight>()->SetPosition(glm::vec3(0, h_y, 0));
        world->FindNode("mark1")->GetLocalTransformations()->SetPosition(glm::vec3(0,h_y,0));
        //std::cout << h_y << "\n";
    }
}

void Scene::Render()
{
    world->Render(transform);
}