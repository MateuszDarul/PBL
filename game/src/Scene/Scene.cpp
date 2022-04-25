#include "Scene.h"

#include "Scripts/TestScript.h"
#include "Scripts/StatsScript.h"

Scene::Scene()
{
    glfwSetInputMode(GameApplication::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    ResourceManager* resMan = GameApplication::GetResourceManager();

    std::shared_ptr<cmp::Model> mc;
    std::shared_ptr<cmp::ModelInst> mic;
    std::shared_ptr<GameObject> go;
    std::shared_ptr<cmp::Transform> tc;

    std::shared_ptr<ShaderComponent> shader_d = std::make_shared<ShaderComponent>();
    shader_d->Create("Resources/shaders/default.vert", "Resources/shaders/default.frag");
    std::shared_ptr<ShaderComponent> shader_i = std::make_shared<ShaderComponent>();
    shader_i->Create("Resources/shaders/inst.vert", "Resources/shaders/inst.frag");

    collidersManager = new CollidersManager();

    ///***

    world = new SceneNode(std::make_shared<GameObject>());
    world->GetGameObject()->AddComponent(std::make_shared<cmp::Name>("ROOT"));
    world->GetGameObject()->AddComponent(std::make_shared<cmp::Transform>());

    ///***

    go = std::make_shared<GameObject>();
    go->AddComponent(std::make_shared<NameComponent>("CAMERA"));
    go->AddComponent(std::make_shared<CameraComponent>());
    go->GetComponent<cmp::Camera>()->Create(glm::vec3(0,3,10));
    go->GetComponent<cmp::Camera>()->SetSpeed(5);
    world->AddChild(go);

    ///***

    MapLoader::Load("Resources/maps/world.map", world, shader_d, collidersManager);

    ///***

    world->LoadScripts();
}

Scene::~Scene()
{
    delete world;
    world = nullptr;

    delete collidersManager;
    collidersManager = nullptr;
}

void Scene::Update(float dt)
{
    std::shared_ptr<GameObject> goCamera = world->FindNode("CAMERA")->GetGameObject();
    goCamera->GetComponent<cmp::Camera>()->Update(GameApplication::GetInputManager(), dt);

    transform = GameApplication::GetProjection() * goCamera->GetComponent<cmp::Camera>()->GetView();



    collidersManager->CheckCollisions();
    world->Update(dt);
}

void Scene::Render()
{
    world->Render(transform);
}