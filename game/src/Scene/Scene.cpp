#include "Scene.h"

#include "Scripts/RaycastTest.h"
#include "Scripts/DoorActivator.h"
#include "Scripts/PlayerPlaceTurret.h"
#include "Scripts/GameManager.h"
#include "Scripts/Resource.h"

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

    auto lineShader = std::make_shared<cmp::Shader>();
    lineShader->Create("Resources/shaders/line.vert", "Resources/shaders/line.frag");

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

    go->AddComponent(std::make_shared<BoxCollider>(false, false, CollisionLayer::Player));
    go->GetComponent<cmp::BoxCol>()->setLengths(glm::vec3(1,3,1));
    go->GetComponent<cmp::BoxCol>()->SetOffset(glm::vec3(0,-1.5,0));
    go->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collidersManager);
    world->AddChild(go);


    //skrypty gracza
    go->AddComponent(std::make_shared<cmp::Scriptable>());
    GameManager* gm = new GameManager();
    go->GetComponent<cmp::Scriptable>()->Add(gm);
    

    auto debugLineGO = std::make_shared<GameObject>();
    auto debugLineCmp = std::make_shared<cmp::Line>();
    debugLineCmp->Create();
    debugLineGO->AddComponent(debugLineCmp);
    debugLineGO->AddComponent(std::make_shared<cmp::Transform>());
    world->FindNode("CAMERA")->AddChild(debugLineGO);
    
    auto playerPlace = new PlayerPlaceTurret();
    
    playerPlace->gameManager = gm;
    playerPlace->line = debugLineCmp;
    playerPlace->colMan = collidersManager;
    playerPlace->resMan = resMan;
    playerPlace->turretShader = shader_l;
    playerPlace->scene = this;
    
    go->GetComponent<ScriptComponent>()->Add(playerPlace);



    ///***

    std::shared_ptr<cmp::Shader> shader[2];
    shader[0] = shader_l;
    shader[1] = shader_i;
    MapLoader::Load("Resources/maps/world.map", world, shader, collidersManager);

    ///***

    go = std::make_shared<GameObject>();
    {
        go->AddComponent(std::make_shared<cmp::PointLight>());
        go->GetComponent<cmp::PointLight>()->Create();
        go->GetComponent<cmp::PointLight>()->SetPosition(glm::vec3(-5, 4, 0));
        go->GetComponent<cmp::PointLight>()->AddShader(shader_l);
        go->GetComponent<cmp::PointLight>()->AddShader(shader_i);
        go->AddComponent(std::make_shared<cmp::Name>("light1"));
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
        go->GetComponent<cmp::PointLight>()->SetPosition(glm::vec3(13, 5, 0));
        go->GetComponent<cmp::PointLight>()->AddShader(shader_l);
        go->GetComponent<cmp::PointLight>()->AddShader(shader_i);
        go->AddComponent(std::make_shared<cmp::Name>("light2"));
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
        go->GetComponent<cmp::SpotLight>()->SetPosition(glm::vec3(-5, 5, 13));
        go->GetComponent<cmp::SpotLight>()->SetDirection(glm::vec3(-1, -1, 0));
        go->GetComponent<cmp::SpotLight>()->AddShader(shader_l);
        go->GetComponent<cmp::SpotLight>()->AddShader(shader_i);
        go->AddComponent(std::make_shared<cmp::Name>("light3"));
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

    ///***

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
            resMan->GetMesh("Resources/models/wieze/w1/w1.obj"));
        go->AddComponent(std::make_shared<cmp::Particles>());
        std::shared_ptr<cmp::Particles> particles = go->GetComponent<cmp::Particles>();
        particles->Create(world->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Camera>());
        particles->SetTexture("Resources/textures/particle.png");
        particles->SetParticlesPerSecond(100);
        particles->SetParticleMaxAmount(500);
        particles->SetOffset(glm::vec3(0, 2.0f, 0));
        particles->SetDirectionVar(45);
        particles->SetParticleLifetime(5.0f);
        particles->SetScale(0.1f);
        particles->SetSpeed(5.0f);
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

    go = std::make_shared<GameObject>();
    {
        mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/Crate/Crate.obj"),
            resMan->GetMaterial("Resources/models/Crate/Crate.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_d);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(2, 0.5, 4);
        go->AddComponent(std::make_shared<cmp::FrustumCulling>());
        go->GetComponent<cmp::FrustumCulling>()->Create(
            resMan->GetMesh("Resources/models/Crate/Crate.obj")
        );

        go->AddComponent(std::make_shared<BoxCollider>(true, false));
        go->GetComponent<cmp::BoxCol>()->setLengths(glm::vec3(2, 2, 2));
        go->GetComponent<cmp::BoxCol>()->isOptimized = true;
        go->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collidersManager);

        go->AddComponent(std::make_shared<cmp::Scriptable>());
        Resource* resource = new Resource();
        resource->energy = 150;
        go->GetComponent<cmp::Scriptable>()->Add(resource);
        resource->gameObject = go;
    }
    world->AddChild(go);

    
    //=== ray test ===

    //- mirror 1
    go = std::make_shared<GameObject>();
    tc = std::make_shared<TransformComponent>();
    tc->SetPosition(8.0f, 3.0f, 0.5f);
    tc->SetRotation(0.0f, 15.0f, 0.0f);
    go->AddComponent(tc);
    mc = std::make_shared<ModelComponent>();
    mc->Create(
        resMan->GetMesh("Resources/models/Crate/Crate.obj"),
        resMan->GetMaterial("Resources/models/floor/floor.mtl")
    );
    go->AddComponent(shader_d);
    go->AddComponent(mc);
    go->AddComponent(std::make_shared<cmp::Name>("Mirror"));
    go->AddComponent(std::make_shared<cmp::BoxCol>(false,false));
    go->GetComponent<BoxCollider>()->AddToCollidersManager(collidersManager);
    go->GetComponent<BoxCollider>()->setLengths({ 2.0f, 2.0f, 2.0f });
    go->AddComponent(std::make_shared<FrustumCullingComponent>());
    go->GetComponent<cmp::FrustumCulling>()->Create(
            resMan->GetMesh("Resources/models/Crate/Crate.obj")
        );

    world->AddChild(go);

    //- mirror 2
    go = std::make_shared<GameObject>();
    tc = std::make_shared<TransformComponent>();
    tc->SetPosition(1.0f, 3.0f, 9.0f);
    tc->SetRotation(0.0f, 180.0f, 0.0f);
    go->AddComponent(tc);
    mc = std::make_shared<ModelComponent>();
    mc->Create(
        resMan->GetMesh("Resources/models/Crate/Crate.obj"),
        resMan->GetMaterial("Resources/models/floor/floor.mtl")
    );
    go->AddComponent(shader_d);
    go->AddComponent(mc);
    go->AddComponent(std::make_shared<cmp::Name>("Mirror"));
    go->AddComponent(std::make_shared<cmp::BoxCol>(false, false));
    go->GetComponent<BoxCollider>()->AddToCollidersManager(collidersManager);
    go->GetComponent<BoxCollider>()->setLengths({ 2.0f, 2.0f, 2.0f });
    go->AddComponent(std::make_shared<FrustumCullingComponent>());
    go->GetComponent<cmp::FrustumCulling>()->Create(
            resMan->GetMesh("Resources/models/Crate/Crate.obj")
        );

    world->AddChild(go);

    //- button
    {      
        go = std::make_shared<GameObject>();
        tc = std::make_shared<TransformComponent>();
        tc->SetPosition(12.0f, 2.5f, -5.0f);
        
        go->AddComponent(tc);
        mc = std::make_shared<ModelComponent>();
        mc->Create(
            resMan->GetMesh("Resources/models/Crate/Crate.obj"),
            resMan->GetMaterial("Resources/models/wall/wall.mtl")
        );
        go->AddComponent(shader_d);
        go->AddComponent(mc);
        go->AddComponent(std::make_shared<cmp::Name>("Button"));
        go->AddComponent(std::make_shared<cmp::BoxCol>(false, true));
        go->GetComponent<BoxCollider>()->AddToCollidersManager(collidersManager);
        go->GetComponent<BoxCollider>()->setLengths({ 2.0f, 2.0f, 2.0f });
        go->AddComponent(std::make_shared<FrustumCullingComponent>());
        go->GetComponent<cmp::FrustumCulling>()->Create(
                resMan->GetMesh("Resources/models/Crate/Crate.obj")
            );

        auto scriptHolder = std::make_shared<cmp::Scriptable>();
        go->AddComponent(scriptHolder);

        auto doorScript = new DoorActivator();
        scriptHolder->Add(doorScript);


        world->AddChild(go);
    }


    //- raycaster object

    go = std::make_shared<GameObject>();

    tc = std::make_shared<TransformComponent>();
    //tc->SetPosition(3.0f, 7.0f, 6.5f);
    tc->SetPosition(0.0f, 1.0f, 0.0f);
    go->AddComponent(tc);

    go->AddComponent(std::make_shared<cmp::Name>("Raycaster"));

    go->AddComponent(lineShader);

    auto line = std::make_shared<cmp::Line>();
    line->Create();
    
    line->Set(0, {0.0f, 3.0f, 0.0f});
    line->Set(1, {0.0f, 4.0f, 0.0f});
    go->AddComponent(line);

    auto scriptHolder = std::make_shared<cmp::Scriptable>();
    go->AddComponent(scriptHolder);

    auto raycastScript = new RaycastTest();
    scriptHolder->Add(raycastScript);
    raycastScript->line = line.get();
    raycastScript->colMan = collidersManager;

    world->AddChild(go);

    //=== text

    //renderowany jako ostatni bo inaczej sa te dziwne artefakty

    Font* font = resMan->GetFont("Resources/fonts/arial.ttf");

    go = std::make_shared<GameObject>();
    tc = std::make_shared<TransformComponent>();
    auto textComponent = std::make_shared<TextComponent>();
    textComponent->Create("Energy: ", font);
    textComponent->alwaysSeen = true;
    textComponent->isGuiElement = true;
    textComponent->color = {1.0f, 0.0f, 0.0f};

    auto textShader = std::make_shared<ShaderComponent>();
    textShader->Create("Resources/shaders/text.vert", "Resources/shaders/text.frag");

    
    go->AddComponent(textShader);
    go->AddComponent(mc);
    go->AddComponent(tc);
    go->AddComponent(textComponent);

    go->GetComponent<TransformComponent>()->SetPosition(0.1f, 0.1f, 0.1f);
    go->GetComponent<TransformComponent>()->SetScale(0.1f);
    go->AddComponent(std::make_shared<cmp::Name>("EnergyText"));
    

    world->AddChild(go);

    //===
   
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
    world->FindNode("Raycaster")->GetGameObject()->GetComponent<cmp::Transform>()->Rotate(0.0f, 12.74f*dt, 0.0f);
    

    
    std::shared_ptr<GameObject> goCamera = world->FindNode("CAMERA")->GetGameObject();
    std::shared_ptr<TransformComponent> transformCamera = goCamera->GetComponent<cmp::Transform>();

    goCamera->GetComponent<CameraComponent>()->Update(GameApplication::GetInputManager(), dt);
    transformCamera->SetPosition(goCamera->GetComponent<CameraComponent>()->GetPosition());

    
    //world->FindNode("In world text")->GetGameObject()->GetComponent<cmp::Text>()->FaceCamera(goCamera->GetComponent<cmp::Camera>());
    

    transform = GameApplication::GetProjection() * goCamera->GetComponent<CameraComponent>()->GetView();

    world->FindNode("Crate1")->GetGameObject()->GetComponent<cmp::Transform>()->Move(5 * dt,0,0);

    collidersManager->CheckEverything();
    goCamera->GetComponent<CameraComponent>()->SetPosition(transformCamera->GetPosition());
    world->Update(dt);
}

void Scene::Render()
{
    world->Render(transform);
}


void Scene::AddGameObject(std::shared_ptr<GameObject> child, std::shared_ptr<GameObject> parent)
{
    SceneNode* node = world->GetRoot();
    
    if (parent)
    {
        auto name = parent->GetComponent<cmp::Name>();
        if (name)
        {
            node = world->FindNode(name->Get());
        }
    }
    
    node->AddChild(child);
}

SceneNode* Scene::GetWorldNode()
{
    return world;
}
