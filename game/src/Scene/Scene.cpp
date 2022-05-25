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

    skybox = new Skybox();
    std::string skyboxFaces[] = {
    "Resources/textures/skybox/right.png",
    "Resources/textures/skybox/left.png",
    "Resources/textures/skybox/top.png",
    "Resources/textures/skybox/bottom.png",
    "Resources/textures/skybox/front.png",
    "Resources/textures/skybox/back.png" };
    
    std::shared_ptr<ShaderComponent> shader_skybox = std::make_shared<ShaderComponent>();
    shader_skybox->Create("Resources/shaders/skybox.vert", "Resources/shaders/skybox.frag");
    skybox->Create(skyboxFaces, shader_skybox);

    /// ***

    std::shared_ptr<ShaderComponent> shader_d = std::make_shared<ShaderComponent>();
    std::shared_ptr<ShaderComponent> shader_l = std::make_shared<ShaderComponent>();
    shader_d->Create("Resources/shaders/default.vert", "Resources/shaders/default.frag");
    shader_l->Create("Resources/shaders/light.vert", "Resources/shaders/light.frag");
    std::shared_ptr<ShaderComponent> shader_r = std::make_shared<ShaderComponent>();
    shader_r->Create("Resources/shaders/refraction.vert", "Resources/shaders/refraction.frag");

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
        go->AddComponent(std::make_shared<cmp::Name>("wieza"));
        go->GetComponent<cmp::Transform>()->SetPosition(0, 0.5, -5);
        go->AddComponent(std::make_shared<cmp::FrustumCulling>());
        go->GetComponent<cmp::FrustumCulling>()->Create(
            resMan->GetMesh("Resources/models/wieze/w1/w1.obj"));
        go->AddComponent(std::make_shared<cmp::Particles>());
        std::shared_ptr<cmp::Particles> particles = go->GetComponent<cmp::Particles>();
        particles->Create(world->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Camera>(), true, 200);
        particles->SetTexture("Resources/textures/particle.png");
        particles->SetParticlesPerSecond(100);
        particles->SetOffset(glm::vec3(0, 2.0f, 0));
        particles->SetDirectionVar(45);
        particles->SetParticleLifetime(1.0f);
        particles->SetScale(0.1f);
        particles->SetSpeed(20.0f);
    }
    world->AddChild(go);

    ///***
    go = std::make_shared<GameObject>();
    {
        go->AddComponent(std::make_shared<cmp::Name>("Crate1"));
        go = std::make_shared<GameObject>();
        std::shared_ptr<cmp::Refract> refr = std::make_shared<cmp::Refract>();
        refr->Create(resMan->GetMesh("Resources/models/Crate/Crate.obj"), 1.52f, skybox->GetTexture());
        go->AddComponent(refr);
        go->AddComponent(shader_r);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(-15.1, 3, 0);
        go->GetComponent<cmp::Transform>()->SetScale(glm::vec3(0.2, 2, 4));
    }
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
    if (Input()->Keyboard()->OnPressed(KeyboardKey::B))
    {
        world->FindNode("wieza")->GetGameObject()->GetComponent<cmp::Particles>()->Burst();
    }
    goCamera->GetComponent<CameraComponent>()->SetPosition(transformCamera->GetPosition());
    world->Update(dt);

    shadowsManager->Update();
}

void Scene::Render()
{
    glViewport(0, 0, GameApplication::GetWindowSize().x, GameApplication::GetWindowSize().y);
    std::shared_ptr<GameObject> goCamera = world->FindNode("CAMERA")->GetGameObject();
    glm::mat4 skyboxTransform = GameApplication::GetProjection() * glm::mat4(glm::mat3(goCamera->GetComponent<CameraComponent>()->GetView()));
    skybox->Render(skyboxTransform);
    world->Render(transform);
}