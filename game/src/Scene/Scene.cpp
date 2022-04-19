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

    world = new SceneNode(std::make_shared<GameObject>());
    world->GetGameObject()->AddComponent(std::make_shared<cmp::Name>("ROOT"));
    world->GetGameObject()->AddComponent(std::make_shared<cmp::Transform>());
    MapLoader::Load("Resources/maps/world.map", world);

    ///***

    goCamera = new GameObject();
    goCamera->AddComponent(std::make_shared<CameraComponent>());
    goCamera->GetComponent<cmp::Camera>()->Create(glm::vec3(0,3,10));
    goCamera->GetComponent<cmp::Camera>()->SetSpeed(5);

    ///***

    std::shared_ptr<ShaderComponent> shader_d = std::make_shared<ShaderComponent>();
    shader_d->Create("Resources/shaders/default.vert", "Resources/shaders/default.frag");
    std::shared_ptr<ShaderComponent> shader_i = std::make_shared<ShaderComponent>();
    shader_i->Create("Resources/shaders/inst.vert", "Resources/shaders/inst.frag");
}

Scene::~Scene()
{
    delete world;
    world = nullptr;

    delete goCamera;
    goCamera = nullptr;
}

void Scene::Update(float dt)
{
    goCamera->GetComponent<cmp::Camera>()->Update(GameApplication::GetInputManager(), dt);
    transform = GameApplication::GetProjection() * goCamera->GetComponent<cmp::Camera>()->GetView();
    
    world->Update();
}

void Scene::Render()
{
    world->Render(transform);
}