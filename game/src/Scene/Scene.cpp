#include "Scene.h"

#include "ShadowsManager.h"

#include "Scripts/RaycastTest.h"
#include "Scripts/DoorActivator.h"
#include "Scripts/PlayerPlaceTurret.h"
#include "Scripts/GameManager.h"
#include "Scripts/Resource.h"
#include "Scripts/Blueprint.h"
#include "Scripts/PlayerInteract.h"
#include "Scripts/PlayerGroundCheck.h"
#include "Scripts/Health.h"
#include "EnemyScript.h"

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

    auto displShader = std::make_shared<ShaderComponent>();
    displShader->Create("Resources/shaders/displ.vert", "Resources/shaders/displ.frag");

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
    go->GetComponent<cmp::Camera>()->Create(glm::vec3(0,3.5,10));
    go->GetComponent<cmp::Camera>()->SetSpeed(5);
    collidersManager = new CollidersManager(go); //mened�er kolider�w
    collidersManager->SetDistanceFromPlayer(10.0f);
    go->AddComponent(std::make_shared<BoxCollider>(false, false));
    go->GetComponent<cmp::BoxCol>()->setLengths(glm::vec3(1.5,3,1.5));
    go->GetComponent<cmp::BoxCol>()->SetOffset(glm::vec3(0,-1,0));
    go->GetComponent<cmp::BoxCol>()->layer = CollisionLayer::Player;
    go->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collidersManager);
    // go->AddComponent(std::make_shared<SphereCollider>(false, false));
    // go->GetComponent<cmp::SphereCol>()->SetRadius(0.5);
    // go->GetComponent<cmp::SphereCol>()->SetOffset(glm::vec3(0,-2.5,0));
    // go->GetComponent<cmp::SphereCol>()->layer = CollisionLayer::Player;
    // go->GetComponent<cmp::SphereCol>()->AddToCollidersManager(collidersManager);
    world->AddChild(go);

    auto playerGO = go;

    //ground check
    {
        auto groundCheckGO = std::make_shared<GameObject>();
        groundCheckGO->AddComponent(std::make_shared<cmp::Transform>());
        groundCheckGO->GetComponent<cmp::Transform>()->SetPosition(0.0, -2.5, 0.0);
        // groundCheckGO->GetComponent<cmp::Transform>()->SetPosition(13.5, 5.5, -8);

        groundCheckGO->AddComponent(std::make_shared<SphereCollider>(true, false));
        groundCheckGO->GetComponent<cmp::SphereCol>()->SetRadius(0.3f);
        groundCheckGO->GetComponent<cmp::SphereCol>()->AddToCollidersManager(collidersManager);

        groundCheckGO->AddComponent(std::make_shared<cmp::Scriptable>());

        auto groundCheckScript = new PlayerGroundCheck();
        groundCheckGO->GetComponent<cmp::Scriptable>()->Add(groundCheckScript);
        groundCheckScript->player = playerGO->GetComponent<cmp::Camera>();

        world->FindNode("CAMERA")->AddChild(groundCheckGO);
    }

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
    playerPlace->lineShader = lineShader;

    //to fix rendering order
    auto turretsHolderGO = std::make_shared<GameObject>();
    turretsHolderGO->AddComponent(std::make_shared<cmp::Transform>());
    auto turretsHolderNode = world->AddChild(turretsHolderGO);

    playerPlace->turretsHolder = turretsHolderNode.get();
    
    go->GetComponent<ScriptComponent>()->Add(playerPlace);


    auto playerInteract = new PlayerInteract();
    
    playerInteract->colMan = collidersManager;
    playerInteract->camera = playerGO->GetComponent<cmp::Camera>();

    auto crosshairTextTEMP = std::make_shared<TextComponent>();
    playerInteract->textTEMP = crosshairTextTEMP;

    go->GetComponent<ScriptComponent>()->Add(playerInteract);
    

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

    go = std::make_shared<GameObject>();
    {
        mc = std::make_shared<ModelComponent>();
        mc->Create(
            resMan->GetMesh("Resources/models/displacement test/capsule.obj"),
            resMan->GetMaterial("Resources/models/displacement test/capsule.mtl")
        );
        go->AddComponent(displShader);
        go->AddComponent(mc);

        go->AddComponent(std::make_shared<FrustumCullingComponent>());
        go->GetComponent<cmp::FrustumCulling>()->Create(
                resMan->GetMesh("Resources/models/displacement test/capsule.obj")
            );

        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(0, 2, 5);

        go->AddComponent(std::make_shared<cmp::Name>("Enemy"));

        go->AddComponent(std::make_shared<BoxCollider>(true, false, CollisionLayer::ENEMY));
        go->GetComponent<cmp::BoxCol>()->setLengths(glm::vec3(2, 2, 2));
        go->GetComponent<cmp::BoxCol>()->isOptimized = false;
        go->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collidersManager);


        go->AddComponent(std::make_shared<cmp::Scriptable>());
        Health* health = new Health();
        health->SetMaxHealth(50.0f);
        health->scene = this;
        go->GetComponent<cmp::Scriptable>()->Add(health);

        auto enemyScript = new EnemyScript(std::shared_ptr<SceneNode>(world), playerGO);
        go->GetComponent<cmp::Scriptable>()->Add(enemyScript);
    }
    world->AddChild(go);

    ///***
    {
        go = std::make_shared<GameObject>();
        go->AddComponent(std::make_shared<cmp::Name>("Crate1"));
        std::shared_ptr<cmp::Refract> refr = std::make_shared<cmp::Refract>();
        refr->Create(resMan->GetMesh("Resources/models/Crate/Crate.obj"), 1.52f, skybox->GetTexture());
        go->AddComponent(refr);
        go->AddComponent(shader_r);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(-15.1, 3, 0);
        go->GetComponent<cmp::Transform>()->SetScale(glm::vec3(0.2, 2, 4));
        world->AddChild(go);
    }
    
    ///***

    MapLoader::Load("Resources/maps/world.map", world, shader_l, collidersManager, shadowsManager);

        
    //=== pickupable resource
    for (int i = 0; i < 3; i++)
    {
        go = std::make_shared<GameObject>();
        go->AddComponent(std::make_shared<cmp::Name>("Resource " + std::to_string(i)));
        
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(14.5, 1.5, i * 2);
        go->GetComponent<cmp::Transform>()->SetScale(0.5);

        go->AddComponent(std::make_shared<BoxCollider>(true, true));
        go->GetComponent<cmp::BoxCol>()->setLengths({1.0, 1.0, 1.0});
        go->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collidersManager);

        auto model = std::make_shared<cmp::Model>();
        model->Create(
            resMan->GetMesh("Resources/models/Crate/Crate.obj"),
            resMan->GetMaterial("Resources/models/Crate/Crate.mtl")
        );
        go->AddComponent(model);
        go->AddComponent(shader_d);

        go->AddComponent(std::make_shared<cmp::FrustumCulling>());
        go->GetComponent<cmp::FrustumCulling>()->Create(resMan->GetMesh("Resources/models/Crate/Crate.obj"));

        auto resourceScript = new Resource();
        go->AddComponent(std::make_shared<cmp::Scriptable>());
        go->GetComponent<cmp::Scriptable>()->Add(resourceScript);

    
        world->AddChild(go);
    }

    //blueprints
    for (int i = 0; i < 3; i++)
    {
        go = std::make_shared<GameObject>();
        go->AddComponent(std::make_shared<cmp::Name>("Blueprint " + std::to_string(i)));
        
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(14.5, 1.5, i * 2 - 8);
        go->GetComponent<cmp::Transform>()->SetScale(0.5);

        go->AddComponent(std::make_shared<BoxCollider>(true, true));
        go->GetComponent<cmp::BoxCol>()->setLengths({1.0, 1.0, 1.0});
        go->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collidersManager);

        auto model = std::make_shared<cmp::Model>();
        model->Create(
            resMan->GetMesh("Resources/models/Crate/Crate.obj"),
            resMan->GetMaterial("Resources/models/wall/wall.mtl")
        );
        go->AddComponent(model);
        go->AddComponent(shader_d);

        go->AddComponent(std::make_shared<cmp::FrustumCulling>());
        go->GetComponent<cmp::FrustumCulling>()->Create(resMan->GetMesh("Resources/models/Crate/Crate.obj"));

        auto resourceScript = new Blueprint();
        resourceScript->type = i;
        go->AddComponent(std::make_shared<cmp::Scriptable>());
        go->GetComponent<cmp::Scriptable>()->Add(resourceScript);

    
        world->AddChild(go);
    }

    //mirrors
    {
        go = std::make_shared<GameObject>();
        go->AddComponent(std::make_shared<cmp::Name>("Mirror"));

        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(2.5, 2.5, -28.3);
        go->GetComponent<cmp::Transform>()->SetScale(1.0);

        go->AddComponent(std::make_shared<BoxCollider>(false, false));
        go->GetComponent<cmp::BoxCol>()->setLengths({2.0, 2.0, 2.0});
        go->GetComponent<cmp::BoxCol>()->SetMass(999999999.9f);
        go->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collidersManager);

        auto model = std::make_shared<cmp::Model>();
        model->Create(
            resMan->GetMesh("Resources/models/Crate/Crate.obj"),
            resMan->GetMaterial("Resources/models/floor/floor.mtl")
        );
        go->AddComponent(model);
        go->AddComponent(shader_d);

        go->AddComponent(std::make_shared<cmp::FrustumCulling>());
        go->GetComponent<cmp::FrustumCulling>()->Create(resMan->GetMesh("Resources/models/Crate/Crate.obj"));


        go->AddComponent(std::make_shared<cmp::Scriptable>());

        auto mirrorScript = new MirrorRotate();
        mirrorScript->SetEnabled(false);
        go->GetComponent<cmp::Scriptable>()->Add(mirrorScript);

        world->AddChild(go);
    }

    {
        //rotator
        auto mirrorHolderGO = std::make_shared<GameObject>();

        mirrorHolderGO->AddComponent(std::make_shared<cmp::Transform>());
        mirrorHolderGO->GetComponent<cmp::Transform>()->SetPosition(8.5, 0.5, -20.3);

        mirrorHolderGO->AddComponent(std::make_shared<cmp::Scriptable>());

        auto mirrorScript = new MirrorRotate();
        mirrorScript->SetEnabled(false);
        mirrorHolderGO->GetComponent<cmp::Scriptable>()->Add(mirrorScript);


        auto mirrorHolder = world->AddChild(mirrorHolderGO);


        //mirror itself
        go = std::make_shared<GameObject>();
        go->AddComponent(std::make_shared<cmp::Name>("Mirror"));

        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(0.0, 1.0, 0.0);
        go->GetComponent<cmp::Transform>()->SetScale(1.0);

        go->AddComponent(std::make_shared<BoxCollider>(false, false));
        go->GetComponent<cmp::BoxCol>()->setLengths({2.0, 2.0, 2.0});
        go->GetComponent<cmp::BoxCol>()->SetMass(999999999.9f);
        go->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collidersManager);

        auto model = std::make_shared<cmp::Model>();
        model->Create(
            resMan->GetMesh("Resources/models/Crate/Crate.obj"),
            resMan->GetMaterial("Resources/models/floor/floor.mtl")
        );
        go->AddComponent(model);
        go->AddComponent(shader_d);

        go->AddComponent(std::make_shared<cmp::FrustumCulling>());
        go->GetComponent<cmp::FrustumCulling>()->Create(resMan->GetMesh("Resources/models/Crate/Crate.obj"));


        mirrorHolder->AddChild(go);
    }
    
    //doors and activators
    struct DoorAndActivatorPair
    {
        glm::vec3 doorPosition;
        float doorRotationY;
        glm::vec3 activatorPosition;
    };

    std::vector<DoorAndActivatorPair> doorsAndButtons = {
        { {  0.0, 3.0, -10.5 }, 90.0f,   {  6.0, 2.5,  -9.5 } },
        { { 10.0, 3.0, -36.5 }, 90.0f,   {  3.5, 2.5, -36.0 } },
        { { 35.5, 3.0, -47.5 },  0.0f,   { 35.0, 2.5, -54.0 } },
    };

    for (auto& [doorPosition, doorRotation, activatorPosition] : doorsAndButtons)
    {
        //create door
        go = std::make_shared<GameObject>();

        auto doorTransform = std::make_shared<cmp::Transform>();
        doorTransform->SetPosition(doorPosition);
        doorTransform->SetRotation(0, doorRotation, 0);
        go->AddComponent(doorTransform);

        go->AddComponent(std::make_shared<BoxCollider>(false, true));
        if (doorRotation < 0.001f) go->GetComponent<cmp::BoxCol>()->setLengths({1.0, 5.0, 5.0});
        else go->GetComponent<cmp::BoxCol>()->setLengths({5.0, 5.0, 1.0});
        go->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collidersManager);

        auto model = std::make_shared<cmp::Model>();
        model->Create(
            resMan->GetMesh("Resources/models/wall/wall.obj"),
            resMan->GetMaterial("Resources/models/wall/wall.mtl")
        );
        model->SetTintColor(1.0, 0.5, 0.0);
        go->AddComponent(model);
        go->AddComponent(shader_d);

        world->AddChild(go);

        //create activator
        go = std::make_shared<GameObject>();
        go->AddComponent(std::make_shared<cmp::Name>("Door activator"));

        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(activatorPosition);
        go->GetComponent<cmp::Transform>()->SetScale(0.5);

        go->AddComponent(std::make_shared<BoxCollider>(false, true));
        go->GetComponent<cmp::BoxCol>()->setLengths({1.0, 1.0, 1.0});
        go->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collidersManager);

        model = std::make_shared<cmp::Model>();
        model->Create(
            resMan->GetMesh("Resources/models/Crate/Crate.obj"),
            resMan->GetMaterial("Resources/models/wall/wall.mtl")
        );
        go->AddComponent(model);
        go->AddComponent(shader_d);

        go->AddComponent(std::make_shared<cmp::FrustumCulling>());
        go->GetComponent<cmp::FrustumCulling>()->Create(resMan->GetMesh("Resources/models/Crate/Crate.obj"));


        go->AddComponent(std::make_shared<cmp::Scriptable>());

        auto activator = new DoorActivator();
        activator->doorTransform = doorTransform;
        go->GetComponent<cmp::Scriptable>()->Add(activator);

        world->AddChild(go);
    }


    //=== text

    //renderowany jako ostatni bo inaczej sa te dziwne artefakty

    {
        Font* font = resMan->GetFont("Resources/fonts/arial.ttf");

        go = std::make_shared<GameObject>();
        auto tc = std::make_shared<TransformComponent>();
        auto energyTextComponentTEMP = std::make_shared<TextComponent>();
        energyTextComponentTEMP->Create("Resources: ", font);
        energyTextComponentTEMP->alwaysSeen = true;
        energyTextComponentTEMP->isGuiElement = true;
        energyTextComponentTEMP->color = {1.0f, 0.6f, 0.0f};

        auto textShader = std::make_shared<ShaderComponent>();
        textShader->Create("Resources/shaders/text.vert", "Resources/shaders/text.frag");

        
        go->AddComponent(textShader);
        go->AddComponent(tc);
        go->AddComponent(energyTextComponentTEMP);

        go->GetComponent<TransformComponent>()->SetPosition(0.1f, 0.1f, 0.1f);
        go->GetComponent<TransformComponent>()->SetScale(0.1f);
        go->AddComponent(std::make_shared<cmp::Name>("EnergyText"));
        
        world->AddChild(go);
    }


    //crosshair
    {
        Font* font = resMan->GetFont("Resources/fonts/arial.ttf");

        go = std::make_shared<GameObject>();
        auto tc = std::make_shared<TransformComponent>();
        // auto crossText = std::make_shared<TextComponent>();
        crosshairTextTEMP->Create("+", font);
        crosshairTextTEMP->alwaysSeen = true;
        crosshairTextTEMP->isGuiElement = true;
        crosshairTextTEMP->color = {1.0f, 0.0f, 0.0f};
        crosshairTextTEMP->isCentered = true;

        auto textShader = std::make_shared<ShaderComponent>();
        textShader->Create("Resources/shaders/text.vert", "Resources/shaders/text.frag");

        
        go->AddComponent(textShader);
        go->AddComponent(tc);
        go->AddComponent(crosshairTextTEMP);

        go->GetComponent<TransformComponent>()->SetPosition(0.1f, 0.1f, 0.1f);
        go->GetComponent<TransformComponent>()->SetScale(0.1f);
        go->AddComponent(std::make_shared<cmp::Name>("CROSSHAIR"));
        
        world->AddChild(go);
    }


    //===
   
    world->LoadScripts();


    // PREVENT CAMERA AND MIRRORS FROM GOING TO INFINITY
    std::shared_ptr<GameObject> goCamera = world->FindNode("CAMERA")->GetGameObject();
    std::shared_ptr<TransformComponent> transformCamera = goCamera->GetComponent<cmp::Transform>();

    goCamera->GetComponent<CameraComponent>()->Update(GameApplication::GetInputManager(), 0.0f);
    transformCamera->SetPosition(goCamera->GetComponent<CameraComponent>()->GetPosition());
    goCamera->GetNode()->ResetGlobalTransformations();

    goCamera->GetComponent<CameraComponent>()->SetPosition(transformCamera->GetPosition());
    transform = GameApplication::GetProjection() * goCamera->GetComponent<CameraComponent>()->GetView();

    world->Update(0.0f); 
}

Scene::~Scene()
{
    world = nullptr;

    delete shadowsManager;
    shadowsManager = nullptr;

    delete collidersManager;
    collidersManager = nullptr;
}

void Scene::Update(float dt)
{
    world->FindNode("CROSSHAIR")->GetGameObject()->GetComponent<cmp::Transform>()->SetPosition(GameApplication::GetAspectRatio() * 0.5f, 0.5f, 0.1f);

    //Update camera
    std::shared_ptr<GameObject> goCamera = world->FindNode("CAMERA")->GetGameObject();
    std::shared_ptr<TransformComponent> transformCamera = goCamera->GetComponent<cmp::Transform>();

    goCamera->GetComponent<CameraComponent>()->Update(GameApplication::GetInputManager(), dt);
    transformCamera->SetPosition(goCamera->GetComponent<CameraComponent>()->GetPosition());


    //Detect collision
    goCamera->GetNode()->ResetGlobalTransformations();
    collidersManager->CheckEverything();


    //Prevent camera jiggle and set correct position
    goCamera->GetComponent<CameraComponent>()->SetPosition(transformCamera->GetPosition());
    transform = GameApplication::GetProjection() * goCamera->GetComponent<CameraComponent>()->GetView();
    // auto pos = transformCamera->GetPosition();
    // printf("camera %f %f %f\n", pos.x, pos.y, pos.z);

    
    //Position multitool

    auto mtTransform = world->FindNode("MultiTool")->GetGameObject()->GetComponent<cmp::Transform>();
    auto m = glm::inverse(goCamera->GetComponent<cmp::Camera>()->GetView());

    glm::vec4 mtNewPosition = m * glm::vec4(mtTransform->GetPosition(), 1.0f);

    m[3][0] = mtNewPosition.x;
    m[3][1] = mtNewPosition.y;
    m[3][2] = mtNewPosition.z;
    mtTransform->SetModelMatrix(m);


    //Update scene

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

CollidersManager* Scene::GetCollidersManager()
{
    return collidersManager;
}
