#include "Scene.h"

#include "Scripts/TestScript.h"
#include "Scripts/StatsScript.h"

Scene::Scene()
{
    glfwSetInputMode(GameApplication::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    ResourceManager* resMan = GameApplication::GetResourceManager();

    std::shared_ptr<cmp::Model> mc = nullptr;
    std::shared_ptr<cmp::ModelInst> mic = nullptr;
    std::shared_ptr<GameObject> go = nullptr;

    std::shared_ptr<ShaderComponent> shader_d = std::make_shared<ShaderComponent>();
    shader_d->Create("Resources/shaders/default.vert", "Resources/shaders/default.frag");
    std::shared_ptr<ShaderComponent> shader_i = std::make_shared<ShaderComponent>();
    shader_i->Create("Resources/shaders/inst.vert", "Resources/shaders/inst.frag");
    std::shared_ptr<ShaderComponent> shader_l = std::make_shared<ShaderComponent>();
    shader_l->Create("Resources/shaders/light.vert", "Resources/shaders/light.frag");

    collidersManager = new CollidersManager();

    ///***

    world = new SceneNode(std::make_shared<GameObject>());
    world->GetGameObject()->AddComponent(std::make_shared<cmp::Name>("ROOT"));
    world->GetGameObject()->AddComponent(std::make_shared<cmp::Transform>());

    ///***
  
    go = std::make_shared<GameObject>();
    go->AddComponent(std::make_shared<CameraComponent>());
    go->GetComponent<cmp::Camera>()->Create(glm::vec3(0,3,10));
    go->GetComponent<cmp::Camera>()->SetSpeed(5);
    go->AddComponent(std::make_shared<cmp::Name>("Camera"));
    world->AddChild(go);
    go = nullptr;
  
    ///***
  
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

    MapLoader::Load("Resources/maps/world.map", world, shader_l);

    ///***
  
    go->AddComponent(std::make_shared<cmp::Transform>());
    go->AddComponent(std::make_shared<NameComponent>("CAMERA"));
    go->AddComponent(std::make_shared<CameraComponent>());
    go->GetComponent<cmp::Camera>()->Create(glm::vec3(0,3,10));
    go->GetComponent<cmp::Camera>()->SetSpeed(5);
    go->AddComponent(std::make_shared<BoxCollider>(false, false));
    go->GetComponent<cmp::BoxCol>()->setLengths(glm::vec3(1,3,1));
    go->GetComponent<cmp::BoxCol>()->SetOffset(glm::vec3(0,-1.5,0));
    go->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collidersManager);
    world->AddChild(go);

    ///***

    MapLoader::Load("Resources/maps/world.map", world, shader_d, collidersManager);

    ///***

    go = std::make_shared<GameObject>();
    mc = std::make_shared<cmp::Model>();
    mc->Create(
        resMan->GetMesh("Resources/models/wieze/w1/w1.obj"),
        resMan->GetMaterial("Resources/models/wieze/w1/w1.mtl")
    );
    go->AddComponent(mc);
    go->AddComponent(shader_d);
    go->AddComponent(std::make_shared<cmp::Transform>());
    go->GetComponent<cmp::Transform>()->SetPosition(0,0.5,-5);
    world->AddChild(go);

    go = std::make_shared<GameObject>();
    mc = std::make_shared<cmp::Model>();
    mc->Create(
        resMan->GetMesh("Resources/models/wieze/w2/w2.obj"),
        resMan->GetMaterial("Resources/models/wieze/w2/w2.mtl")
    );
    go->AddComponent(mc);
    go->AddComponent(shader_d);
    go->AddComponent(std::make_shared<cmp::Transform>());
    go->GetComponent<cmp::Transform>()->SetPosition(0,0.5,0);
    world->AddChild(go);
    
    go = std::make_shared<GameObject>();
    mc = std::make_shared<cmp::Model>();
    mc->Create(
        resMan->GetMesh("Resources/models/wieze/w3/w3.obj"),
        resMan->GetMaterial("Resources/models/wieze/w3/w3.mtl")
    );
    go->AddComponent(mc);
    go->AddComponent(shader_d);
    go->AddComponent(std::make_shared<cmp::Transform>());
    go->GetComponent<cmp::Transform>()->SetPosition(0,0.5,5);
    world->AddChild(go);

    world->LoadScripts();
}

Scene::~Scene()
{
    delete world;
    world = nullptr;

    delete collidersManager;
    collidersManager = nullptr;
}


float h_y = 1;
void Scene::Update(float dt)
{
    std::shared_ptr<GameObject> goCamera = world->FindNode("CAMERA")->GetGameObject();
    std::shared_ptr<TransformComponent> transformCamera = goCamera->GetComponent<cmp::Transform>();

    goCamera->GetComponent<CameraComponent>()->Update(GameApplication::GetInputManager(), dt);
    transformCamera->SetPosition(goCamera->GetComponent<CameraComponent>()->GetPosition());
    
    transform = GameApplication::GetProjection() * goCamera->GetComponent<CameraComponent>()->GetView();
  
  
    if(GameApplication::GetInputManager()->Keyboard()->IsPressed(KeyboardKey::Z))
    {
        h_y += 1*dt;
        world->FindNode("light1")->GetGameObject()->GetComponent<cmp::PointLight>()->SetPosition(glm::vec3(0, h_y, 0));
        world->FindNode("mark1")->GetLocalTransformations()->SetPosition(glm::vec3(0,h_y,0));
        //std::cout << h_y << "\n";
    }
  
  
    collidersManager->CheckCollisions();
    goCamera->GetComponent<CameraComponent>()->SetPosition(transformCamera->GetPosition());
    world->Update(dt);
}

void Scene::Render()
{
    world->Render(transform);
}