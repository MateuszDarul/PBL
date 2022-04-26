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
    go->AddComponent(std::make_shared<cmp::Transform>());
    go->AddComponent(std::make_shared<NameComponent>("CAMERA"));
    go->AddComponent(std::make_shared<CameraComponent>());
    go->GetComponent<cmp::Camera>()->Create(glm::vec3(0,3,10));
    go->GetComponent<cmp::Camera>()->SetSpeed(5);
    go->AddComponent(std::make_shared<BoxCollider>(false, false));
    go->GetComponent<cmp::BoxCol>()->setLengths(glm::vec3(2,3,2));
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
    go->GetComponent<cmp::Transform>()->SetPosition(0,0.5,0);
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
    go->GetComponent<cmp::Transform>()->SetPosition(0,0.5,0);
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

void Scene::Update(float dt)
{
    std::shared_ptr<GameObject> goCamera = world->FindNode("CAMERA")->GetGameObject();
    std::shared_ptr<TransformComponent> transformCamera = goCamera->GetComponent<cmp::Transform>();

    goCamera->GetComponent<CameraComponent>()->Update(GameApplication::GetInputManager(), dt);
    transformCamera->SetPosition(goCamera->GetComponent<CameraComponent>()->GetPosition());

    transform = GameApplication::GetProjection() * goCamera->GetComponent<CameraComponent>()->GetView();



    collidersManager->CheckCollisions();
    goCamera->GetComponent<CameraComponent>()->SetPosition(transformCamera->GetPosition());
    world->Update(dt);
}

void Scene::Render()
{
    world->Render(transform);
}