#include "Scene.h"

#include "Scripts/TestScript.h"
#include "Scripts/StatsScript.h"

Scene::Scene()
{
    collidersManager = new CollidersManager();

    glfwSetInputMode(GameApplication::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    ResourceManager* resMan = GameApplication::GetResourceManager();

    std::shared_ptr<cmp::Model> mc;
    std::shared_ptr<cmp::ModelInst> mic;
    std::shared_ptr<GameObject> go;
    std::shared_ptr<cmp::Transform> tc;

    world = new SceneNode(std::make_shared<GameObject>());
    world->GetGameObject()->AddComponent(std::make_shared<cmp::Name>("ROOT"));
    world->GetGameObject()->AddComponent(std::make_shared<cmp::Transform>());
    MapLoader::Load("Resources/maps/world.map", world);

    ///***

    goCamera = std::make_shared<GameObject>();
    goCamera->AddComponent(std::make_shared<CameraComponent>());
    goCamera->GetComponent<cmp::Camera>()->Create(glm::vec3(0,3,10));
    goCamera->GetComponent<cmp::Camera>()->SetSpeed(5);

    ///***

    std::shared_ptr<ShaderComponent> shader_d = std::make_shared<ShaderComponent>();
    shader_d->Create("Resources/shaders/default.vert", "Resources/shaders/default.frag");
    std::shared_ptr<ShaderComponent> shader_i = std::make_shared<ShaderComponent>();
    shader_i->Create("Resources/shaders/inst.vert", "Resources/shaders/inst.frag");

    ///***

    go = std::make_shared<GameObject>();
    tc = std::make_shared<TransformComponent>();
    mc = std::make_shared<ModelComponent>();
    mc->Create(
        resMan->GetMesh("Resources/models/Crate/Crate.obj"),
        resMan->GetMaterial("Resources/models/Crate/Crate.mtl")
    );
    go->AddComponent(shader_d);
    go->AddComponent(mc);
    go->AddComponent(tc);
    go->AddComponent(std::make_shared<cmp::Name>("GO"));

    world->AddChild(go);

    ///***

    go = std::make_shared<GameObject>();
    tc = std::make_shared<TransformComponent>();
    mc = std::make_shared<ModelComponent>();
    mc->Create(
        resMan->GetMesh("Resources/models/Crate/Crate.obj"),
        resMan->GetMaterial("Resources/models/Crate/Crate.mtl")
    );
    go->AddComponent(shader_d);
    go->AddComponent(mc);
    go->AddComponent(tc);
    go->GetComponent<cmp::Transform>()->SetPosition(5, 0, 0);
    go->AddComponent(std::make_shared<cmp::Name>("GO1"));

    world->FindNode("GO")->AddChild(go);

    ///***

    go = std::make_shared<GameObject>();
    tc = std::make_shared<TransformComponent>();
    mic = std::make_shared<ModelInstancesComponent>();
    mic->Create(9,
        resMan->GetMesh("Resources/models/Crate/Crate.obj"),
        resMan->GetMaterial("Resources/models/Crate/Crate.mtl")
    );
    for(int x=-4, y=-4, i=0; i<9; i++)
    {
        mic->SetTransformation(i, TransformComponent::Transform(glm::vec3(x, -2, y), glm::vec3(0, 0, 0), 1));
        x += 4;
        if((x + 1) % 3 == 0)
        {
            x = -4;
            y += 4;
        }
    }
    mic->UpdateTransformations();
    go->AddComponent(shader_i);
    go->AddComponent(mic);
    go->AddComponent(std::make_shared<cmp::Name>("GO2"));

    world->AddChild(go);

    ///***

    go = std::make_shared<GameObject>();
    tc = std::make_shared<TransformComponent>();
    mc = std::make_shared<ModelComponent>();
    mc->Create(
        resMan->GetMesh("Resources/models/Crate/Crate.obj"),
        resMan->GetMaterial("Resources/models/Crate/Crate.mtl")
    );
    go->AddComponent(shader_d);
    go->AddComponent(mc);
    go->AddComponent(tc);
    go->GetComponent<TransformComponent>()->SetPosition(0.0f, 1.0f, -20.0f);
    go->AddComponent(std::make_shared<cmp::Name>("Box1"));
    go->AddComponent(std::make_shared<cmp::BoxCol>(false, true));
    go->GetComponent<BoxCollider>()->AddToCollidersManager(collidersManager);
    go->GetComponent<BoxCollider>()->setLengths({ 2.0f, 2.0f, 2.0f });

    world->AddChild(go);

    // ///***

    go = std::make_shared<GameObject>();
    tc = std::make_shared<TransformComponent>();
    mc = std::make_shared<ModelComponent>();
    mc->Create(
        resMan->GetMesh("Resources/models/Crate/Crate.obj"),
        resMan->GetMaterial("Resources/models/Crate/Crate.mtl")
    );
    go->AddComponent(shader_d);
    go->AddComponent(mc);
    go->AddComponent(tc);
    go->GetComponent<TransformComponent>()->SetPosition(0.0f, 3.0f, -22.0f);
    go->AddComponent(std::make_shared<cmp::Name>("Box2"));
    go->AddComponent(std::make_shared<cmp::BoxCol>(false, true));
    go->GetComponent<BoxCollider>()->AddToCollidersManager(collidersManager);
    go->GetComponent<BoxCollider>()->setLengths({ 2.0f, 2.0f, 2.0f });

    world->AddChild(go);

    // ///***

    go = std::make_shared<GameObject>();
    tc = std::make_shared<TransformComponent>();
    mc = std::make_shared<ModelComponent>();
    mc->Create(
        resMan->GetMesh("Resources/models/Crate/Crate.obj"),
        resMan->GetMaterial("Resources/models/Crate/Crate.mtl")
    );
    go->AddComponent(shader_d);
    go->AddComponent(mc);
    go->AddComponent(tc);
    go->GetComponent<TransformComponent>()->SetPosition(0.0f, 5.0f, -24.0f);
    go->AddComponent(std::make_shared<cmp::Name>("Box3"));
    go->AddComponent(std::make_shared<cmp::BoxCol>(false, true));
    go->GetComponent<BoxCollider>()->AddToCollidersManager(collidersManager);
    go->GetComponent<BoxCollider>()->setLengths({ 2.0f, 2.0f, 2.0f });

    world->AddChild(go);
    ///***

    go = std::make_shared<GameObject>();
    tc = std::make_shared<TransformComponent>();
    tc->SetScale(0.01f);
    mc = std::make_shared<ModelComponent>();
    mc->Create(
        resMan->GetMesh("Resources/models/sphere.obj"),
        resMan->GetMaterial("Resources/models/sphere.mtl")
    );
    go->AddComponent(shader_d);
    go->AddComponent(mc);
    go->AddComponent(tc);
    go->GetComponent<TransformComponent>()->SetPosition(0.0f, 4.1f, 20.0f);
    go->AddComponent(std::make_shared<cmp::Name>("Ball1"));
    go->AddComponent(std::make_shared<cmp::SphereCol>(false, false));
    go->GetComponent<SphereCollider>()->AddToCollidersManager(collidersManager);
    go->GetComponent<SphereCollider>()->SetRadius(4.1f);

    world->AddChild(go);

    ///***

    world->FindNode("GO")->GetLocalTransformations()->SetPosition(0, 2, 0);

    ///***

    world->LoadScripts();
}

Scene::~Scene()
{
    delete world;
    world = nullptr;

    goCamera = nullptr;
}

void Scene::Update(float dt)
{
    goCamera->GetComponent<cmp::Camera>()->Update(GameApplication::GetInputManager(), dt);

    transform = GameApplication::GetProjection() * goCamera->GetComponent<cmp::Camera>()->GetView();

    // scene->FindNode("GO")->GetLocalTransformations()->Rotate(0, 180*dt, 0);

    world->FindNode("Ball1")->GetLocalTransformations()->Move(0.0f, 0.0f, dt * -5.0f);
    collidersManager->CheckCollisions();

    world->Update(dt);
}

void Scene::Render()
{
    world->Render(transform);
}