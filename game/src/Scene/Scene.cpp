#include "Scene.h"

#include "ShadowsManager.h"

#include "Scripts/TestScript.h"
#include "Scripts/StatsScript.h"
#include "Scripts/RaycastTest.h"


Scene::Scene()
{
    glfwSetInputMode(GameApplication::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    ResourceManager* resMan = GameApplication::GetResourceManager();

    ///***

    std::shared_ptr<GameObject> go;
    std::shared_ptr<cmp::Model> mc;
    std::shared_ptr<cmp::ModelInst> mic;

    ///***

    std::shared_ptr<ShaderComponent> shader_d = std::make_shared<ShaderComponent>();
    std::shared_ptr<ShaderComponent> shader_l = std::make_shared<ShaderComponent>();
    shader_d->Create("Resources/shaders/default.vert", "Resources/shaders/default.frag");
    shader_l->Create("Resources/shaders/light.vert", "Resources/shaders/light.frag");

    ///***

    world = new SceneNode(std::make_shared<GameObject>());
    world->GetGameObject()->AddComponent(std::make_shared<cmp::Name>("ROOT"));
    world->GetGameObject()->AddComponent(std::make_shared<cmp::Transform>());
    
    ///***

    shadowsManager = new ShadowsManager(world, shader_l);

    ///***

    go = std::make_shared<GameObject>();
    go->AddComponent(std::make_shared<cmp::Transform>());
    go->AddComponent(std::make_shared<NameComponent>("CAMERA"));
    go->AddComponent(std::make_shared<CameraComponent>());
    go->GetComponent<cmp::Camera>()->Create(glm::vec3(0,3,10));
    go->GetComponent<cmp::Camera>()->SetSpeed(5);
    collidersManager = new CollidersManager(go); //mened�er kolider�w
    collidersManager->SetDistanceFromPlayer(10.0f);
    go->AddComponent(std::make_shared<BoxCollider>(false, false));
    go->GetComponent<cmp::BoxCol>()->setLengths(glm::vec3(1.5,3,1.5));
    go->GetComponent<cmp::BoxCol>()->SetOffset(glm::vec3(0,-1,0));
    go->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collidersManager);
    go->AddComponent(std::make_shared<SphereCollider>(false, false));
    go->GetComponent<cmp::SphereCol>()->SetRadius(0.5);
    go->GetComponent<cmp::SphereCol>()->SetOffset(glm::vec3(0,-2.5,0));
    go->GetComponent<cmp::SphereCol>()->AddToCollidersManager(collidersManager);
    world->AddChild(go);

    ///***

    MapLoader::Load("Resources/maps/world.map", world, shader_l, collidersManager, shadowsManager);

    ///***
   
    world->LoadScripts();
}

Scene::~Scene()
{
    delete world;
    world = nullptr;

    delete shadowsManager;
    shadowsManager = nullptr;

    delete collidersManager;
    collidersManager = nullptr;
}

void Scene::Update(float dt)
{
    std::shared_ptr<GameObject> goCamera = world->FindNode("CAMERA")->GetGameObject();
    std::shared_ptr<TransformComponent> transformCamera = goCamera->GetComponent<cmp::Transform>();

    goCamera->GetComponent<CameraComponent>()->Update(GameApplication::GetInputManager(), dt);
    transformCamera->SetPosition(goCamera->GetComponent<CameraComponent>()->GetPosition());

    transform = GameApplication::GetProjection() * goCamera->GetComponent<CameraComponent>()->GetView();

    collidersManager->CheckEverything();
    goCamera->GetComponent<CameraComponent>()->SetPosition(transformCamera->GetPosition());
    world->Update(dt);

    shadowsManager->Update();
}

void Scene::Render()
{
    glViewport(0, 0, GameApplication::GetWindowSize().x, GameApplication::GetWindowSize().y);
    world->Render(transform);
}