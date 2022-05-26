#include "Scene.h"

#include "ShadowsManager.h"


#include "Scripts/RaycastTest.h"
#include "Scripts/DoorActivator.h"
#include "Scripts/PlayerPlaceTurret.h"
#include "Scripts/GameManager.h"
#include "Scripts/Resource.h"

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

    auto lineShader = std::make_shared<cmp::Shader>();
    lineShader->Create("Resources/shaders/line.vert", "Resources/shaders/line.frag");

    auto radialShader = std::make_shared<cmp::Shader>();
    radialShader->Create("Resources/shaders/radial.vert", "Resources/shaders/radial.frag");

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
    go->GetComponent<cmp::BoxCol>()->layer = CollisionLayer::Player;
    go->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collidersManager);
    go->AddComponent(std::make_shared<SphereCollider>(false, false));
    go->GetComponent<cmp::SphereCol>()->SetRadius(0.5);
    go->GetComponent<cmp::SphereCol>()->SetOffset(glm::vec3(0,-2.5,0));
    go->GetComponent<cmp::SphereCol>()->layer = CollisionLayer::Player;
    go->GetComponent<cmp::SphereCol>()->AddToCollidersManager(collidersManager);
    world->AddChild(go);

    auto playerGO = go;

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

    //multi tool
    {
        auto multiTool = std::make_shared<GameObject>();
        multiTool->AddComponent(std::make_shared<cmp::Name>("MultiTool"));

        multiTool->AddComponent(std::make_shared<cmp::Transform>());
        multiTool->GetComponent<cmp::Transform>()->SetPosition(0.5f, -0.45f, -1.0f);

        auto baseMesh = std::make_shared<cmp::Model>();
        baseMesh->Create(
            resMan->GetMesh("Resources/models/multitool/multitool.obj"),
            resMan->GetMaterial("Resources/models/multitool/multitool.mtl")
        );
        multiTool->AddComponent(baseMesh);
        multiTool->AddComponent(shader_l);

        world->AddChild(multiTool);
        auto multiToolNode = world->FindNode("MultiTool");


        multiTool->AddComponent(std::make_shared<cmp::Scriptable>());

        MultiToolController* multiToolScript = new MultiToolController();
        multiTool->GetComponent<cmp::Scriptable>()->Add(multiToolScript);
        playerGO->GetComponent<cmp::Scriptable>()->Get<PlayerPlaceTurret>()->multiTool = multiToolScript;
        


        auto mutliToolDisplayHolder = std::make_shared<GameObject>();
        mutliToolDisplayHolder->AddComponent(std::make_shared<cmp::Transform>());
        mutliToolDisplayHolder->GetComponent<cmp::Transform>()->SetPosition(0, 0.2, 0.5);
        mutliToolDisplayHolder->GetComponent<cmp::Transform>()->SetRotation(-10, 0, 0);
        auto displayNode = multiToolNode->AddChild(mutliToolDisplayHolder);


        auto radialBar = std::make_shared<GameObject>();
        auto radialBarModel = std::make_shared<cmp::Model>();
        radialBarModel->Create(
            resMan->GetMesh("Resources/models/multitool/radial.obj"),
            resMan->GetMaterial("Resources/models/multitool/radial.mtl")
        );
        radialBar->AddComponent(radialBarModel);
        radialBar->AddComponent(radialShader);

        radialBar->AddComponent(std::make_shared<cmp::Transform>());
        radialBar->AddComponent(std::make_shared<cmp::Name>("radial"));
        displayNode->AddChild(radialBar);

        multiToolScript->progressBar = radialBar;


        for (int i = -1; i <= 1; i++)
        {
            auto icon = std::make_shared<GameObject>();
            auto iconModel = std::make_shared<cmp::Model>();
            iconModel->Create(
                resMan->GetMesh("Resources/models/multitool/icon.obj"),
                resMan->GetMaterial("Resources/models/multitool/icon.mtl")
            );
            icon->AddComponent(iconModel);
            icon->AddComponent(shader_d);

            iconModel->SetTintColor(0.17, 0.17, 0.17,  1.0);
            multiToolScript->iconsGO[i+1] = icon;

            icon->AddComponent(std::make_shared<cmp::Transform>());
            icon->GetComponent<cmp::Transform>()->SetPosition(i * 0.06, 0.05, 0.0);

            displayNode->AddChild(icon);
        }
    }




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


    //===displacement
    auto displShader = std::make_shared<ShaderComponent>();
    displShader->Create("Resources/shaders/displ.vert", "Resources/shaders/displ.frag");
    {      
        go = std::make_shared<GameObject>();
        auto tc = std::make_shared<TransformComponent>();
        tc->SetPosition(-5.0f, 6.5f, -5.0f);
        
        go->AddComponent(tc);
        mc = std::make_shared<ModelComponent>();
        mc->Create(
            resMan->GetMesh("Resources/models/displacement test/capsule.obj"),
            resMan->GetMaterial("Resources/models/displacement test/capsule.mtl")
        );
        go->AddComponent(displShader);
        go->AddComponent(mc);
        go->AddComponent(std::make_shared<cmp::Name>("Displaced capsule"));
        go->AddComponent(std::make_shared<FrustumCullingComponent>());
        go->GetComponent<cmp::FrustumCulling>()->Create(
                resMan->GetMesh("Resources/models/displacement test/capsule.obj")
            );

        world->AddChild(go);
    }

    
    // //=== ray test ===

    // tu byly lustra, czujnik i laser ale usunalem bo zrobie to teraz 'na czysto'


    
    //=== text

    //renderowany jako ostatni bo inaczej sa te dziwne artefakty

    // Font* font = resMan->GetFont("Resources/fonts/arial.ttf");

    // go = std::make_shared<GameObject>();
    // auto tc = std::make_shared<TransformComponent>();
    // auto textComponent = std::make_shared<TextComponent>();
    // textComponent->Create("Energy: ", font);
    // textComponent->alwaysSeen = true;
    // textComponent->isGuiElement = true;
    // textComponent->color = {1.0f, 0.0f, 0.0f};

    // auto textShader = std::make_shared<ShaderComponent>();
    // textShader->Create("Resources/shaders/text.vert", "Resources/shaders/text.frag");

    
    // go->AddComponent(textShader);
    // go->AddComponent(mc);
    // go->AddComponent(tc);
    // go->AddComponent(textComponent);

    // go->GetComponent<TransformComponent>()->SetPosition(0.1f, 0.1f, 0.1f);
    // go->GetComponent<TransformComponent>()->SetScale(0.1f);
    // go->AddComponent(std::make_shared<cmp::Name>("EnergyText"));
    
    // world->AddChild(go);


    //===
   
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


    //Position multitool

    auto mtTransform = world->FindNode("MultiTool")->GetGameObject()->GetComponent<cmp::Transform>();
    auto m = glm::inverse(goCamera->GetComponent<cmp::Camera>()->GetView());
    
    // printf("Inverse view pos: %f %f %f\n", m[3][0], m[3][1], m[3][2]);
    // printf("Inverse view right: %f %f %f\n", m[0][0], m[1][0], m[2][0]);
    // printf("Inverse view up: %f %f %f\n", m[0][1], m[1][1], m[2][1]);
    // printf("Inverse view forward: %f %f %f\n", m[0][2], m[1][2], m[2][2]);

    glm::vec4 mtNewPosition = m * glm::vec4(mtTransform->GetPosition(), 1.0f);

    m[3][0] = mtNewPosition.x;
    m[3][1] = mtNewPosition.y;
    m[3][2] = mtNewPosition.z;
    mtTransform->SetModelMatrix(m);




    transform = GameApplication::GetProjection() * goCamera->GetComponent<CameraComponent>()->GetView();

    collidersManager->CheckEverything();
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
