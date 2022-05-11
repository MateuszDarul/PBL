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

    ///***

    std::shared_ptr<ShaderComponent> shader_d = std::make_shared<ShaderComponent>();
    shader_d->Create("Resources/shaders/default.vert", "Resources/shaders/default.frag");
    std::shared_ptr<ShaderComponent> shader_i = std::make_shared<ShaderComponent>();
    shader_i->Create("Resources/shaders/inst.vert", "Resources/shaders/inst.frag");
    std::shared_ptr<ShaderComponent> shader_l = std::make_shared<ShaderComponent>();
    shader_l->Create("Resources/shaders/light.vert", "Resources/shaders/light.frag");

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

    collidersManager = new CollidersManager(go); //mened�er kolider�w
    collidersManager->SetDistanceFromPlayer(10.0f);

    go->AddComponent(std::make_shared<BoxCollider>(false, false));
    go->GetComponent<cmp::BoxCol>()->setLengths(glm::vec3(1,3,1));
    go->GetComponent<cmp::BoxCol>()->SetOffset(glm::vec3(0,-1.5,0));
    go->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collidersManager);
    world->AddChild(go);


    ///***

    MapLoader::Load("Resources/maps/world.map", world, shader_l, collidersManager);

    ///***

    go = std::make_shared<GameObject>();
    {
        go->AddComponent(std::make_shared<cmp::PointLight>());
        go->GetComponent<cmp::PointLight>()->Create();
        go->GetComponent<cmp::PointLight>()->SetPosition(glm::vec3(-5, 4, 0));
        go->AddComponent(std::make_shared<cmp::Name>("light1"));
        go->AddComponent(shader_l);
        world->AddChild(go);

        go = std::make_shared<GameObject>();
        mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/Crate/Crate.obj"),
            resMan->GetMaterial("Resources/models/Crate/Crate.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_d);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetScale(0.1);
        world->FindNode("light1")->AddChild(go);
    }
    go = nullptr;

    go = std::make_shared<GameObject>();
    {
        go->AddComponent(std::make_shared<cmp::PointLight>());
        go->GetComponent<cmp::PointLight>()->Create();
        go->GetComponent<cmp::PointLight>()->SetPosition(glm::vec3(18, 5, 0));
        go->AddComponent(std::make_shared<cmp::Name>("light2"));
        go->AddComponent(shader_l);
        world->AddChild(go);

        go = std::make_shared<GameObject>();
        mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/Crate/Crate.obj"),
            resMan->GetMaterial("Resources/models/Crate/Crate.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_d);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetScale(0.1);
        world->FindNode("light2")->AddChild(go);
    }
    go = nullptr;

    go = std::make_shared<GameObject>();
    {
        go->AddComponent(std::make_shared<cmp::SpotLight>());
        go->GetComponent<cmp::SpotLight>()->Create();
        go->GetComponent<cmp::SpotLight>()->SetPosition(glm::vec3(-5, 5, 17));
        go->GetComponent<cmp::SpotLight>()->SetDirection(glm::vec3(-1, -1, 0));
        go->AddComponent(std::make_shared<cmp::Name>("light3"));
        go->AddComponent(shader_l);
        world->AddChild(go);

        go = std::make_shared<GameObject>();
        mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/Crate/Crate.obj"),
            resMan->GetMaterial("Resources/models/Crate/Crate.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_d);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetScale(0.1);
        world->FindNode("light3")->AddChild(go);
    }
    go = nullptr;

    go = std::make_shared<GameObject>();
    {    
        mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/wieze/w1/w1.obj"),
            resMan->GetMaterial("Resources/models/wieze/w1/w1.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_l);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(0,0.5,-5);
        go->AddComponent(std::make_shared<cmp::FrustumCulling>());
        go->GetComponent<cmp::FrustumCulling>()->Create(
            resMan->GetMesh("Resources/models/wieze/w1/w1.obj")
        );
    }
    world->AddChild(go);

    go = std::make_shared<GameObject>();
    {
        mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/wieze/w2/w2.obj"),
            resMan->GetMaterial("Resources/models/wieze/w2/w2.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_l);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(0,0.5,0);
        go->AddComponent(std::make_shared<cmp::FrustumCulling>());
        go->GetComponent<cmp::FrustumCulling>()->Create(
            resMan->GetMesh("Resources/models/wieze/w2/w2.obj")
        );
    }
    world->AddChild(go);
    
    go = std::make_shared<GameObject>();
    {
        mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/wieze/w3/w3.obj"),
            resMan->GetMaterial("Resources/models/wieze/w3/w3.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_l);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(0,0.5,5);
        go->AddComponent(std::make_shared<cmp::FrustumCulling>());
        go->GetComponent<cmp::FrustumCulling>()->Create(
            resMan->GetMesh("Resources/models/wieze/w3/w3.obj")
        );
    }
    world->AddChild(go);

    go = std::make_shared<GameObject>();
    {
        mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/Crate/Crate.obj"),
            resMan->GetMaterial("Resources/models/Crate/Crate.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_l);

        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(0, 20, 5);

        go->AddComponent(std::make_shared<cmp::Name>("Crate1"));

        go->AddComponent(std::make_shared<BoxCollider>(false, false));
        go->GetComponent<cmp::BoxCol>()->setLengths(glm::vec3(2, 2, 2));
        go->GetComponent<cmp::BoxCol>()->isOptimized = true;
        go->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collidersManager);

        go->AddComponent(std::make_shared<FrustumCullingComponent>());
        go->GetComponent<cmp::FrustumCulling>()->Create(
            resMan->GetMesh("Resources/models/Crate/Crate.obj")
        );
    }
    world->AddChild(go);

    go = std::make_shared<GameObject>();
    {
        mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/Crate/Crate.obj"),
            resMan->GetMaterial("Resources/models/Crate/Crate.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_l);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(20, 20, 5);

        go->AddComponent(std::make_shared<cmp::Name>("Crate2"));

        go->AddComponent(std::make_shared<BoxCollider>(false, false));
        go->GetComponent<cmp::BoxCol>()->setLengths(glm::vec3(2, 2, 2));
        go->GetComponent<cmp::BoxCol>()->isOptimized = true;
        go->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collidersManager);

        go->AddComponent(std::make_shared<FrustumCullingComponent>());
        go->GetComponent<cmp::FrustumCulling>()->Create(
            resMan->GetMesh("Resources/models/Crate/Crate.obj")
        );
    }
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

    world->FindNode("Crate1")->GetGameObject()->GetComponent<cmp::Transform>()->Move(5 * dt,0,0);

    collidersManager->CheckCollisions();
    goCamera->GetComponent<CameraComponent>()->SetPosition(transformCamera->GetPosition());
    world->Update(dt);
}

void Scene::Render()
{
    world->Render(transform);
}