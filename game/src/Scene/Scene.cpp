#include "Scene.h"

#include "ShadowsManager.h"

#include "SoundDevice.h"
#include "SoundBuffer.h"
#include "MusicBuffer.h"
#include "AudioManager.h"

#include "Scripts/RaycastTest.h"
#include "Scripts/DoorActivator.h"
#include "Scripts/PlayerPlaceTurret.h"
#include "Scripts/GameManager.h"
#include "Scripts/Resource.h"
#include "Scripts/Blueprint.h"
#include "Scripts/PlayerInteract.h"
#include "Scripts/PlayerGroundCheck.h"
#include "Scripts/PlayerHealthScript.h"
#include "Scripts/Health.h"
#include "EnemyScript.h"
#include "Scripts/PlayerFootsteps.h"
#include "Scripts/Cutscenexd.h"
#include "Scripts/CutsceneFinal.h"
#include "Scripts/LightActivator.h"
#include "Scripts/LanternRange.h"
#include "Scripts/SwitchLevelTrigger.h"



// for quick access (avoiding string comparisions in FindNode)//
std::shared_ptr<GameObject> GO_MULTITOOL;
std::shared_ptr<GameObject> GO_FLASHLIGHT;
std::shared_ptr<GameObject> GO_CROSSHAIR;
std::shared_ptr<GameObject> GO_TOOLTIP;

std::shared_ptr<SceneNode> NODE_MAIN;
std::shared_ptr<SceneNode> NODE_GUI;

static SceneInfo SCENE_INFO;

const SceneInfo& Scene::GetSceneInfo()
{
    return SCENE_INFO;
}

Scene::Scene()
{
    glfwSetInputMode(GameApplication::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    ResourceManager* resMan = GameApplication::GetResourceManager();

    isPaused = false;

    ///***

    std::shared_ptr<GameObject> go;
    std::shared_ptr<cmp::Model> mc;
    std::shared_ptr<cmp::ModelInst> mic;

    ///*** SOUND ///***

    SoundDevice* soundDevice = SoundDevice::get();

    musicBuffer = new MusicBuffer("Resources/Music/agresive.wav");
    musicBuffer->SetVolume(0.11f);


    MusicBuffer* memoryLeak1 = new MusicBuffer("Resources/Music/action.wav");
    MusicBuffer* memoryLeak2 = new MusicBuffer("Resources/Music/dramatic.wav");
    
    memoryLeak1->SetVolume(0.13f);
    memoryLeak2->SetVolume(0.16f);
    
    AudioManager::Enqueue(memoryLeak1);
    AudioManager::Enqueue(memoryLeak2);
    AudioManager::Enqueue(musicBuffer);

    /// *** SKYBOX

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
    shader_d->Create("Resources/shaders/default.vert", "Resources/shaders/default.frag");
    shader_d->Use();
    shader_d->SetFloat("brightness", GameApplication::GetBright());
    shader_d->SetFloat("gamma", GameApplication::GetGamma());
    shader_d->SetFloat("contrast", GameApplication::GetContrast());

    std::shared_ptr<ShaderComponent> shader_l = std::make_shared<ShaderComponent>();
    shader_l->Create("Resources/shaders/light.vert", "Resources/shaders/light.frag");
    shader_l->Use();
    shader_l->SetFloat("brightness", GameApplication::GetBright());
    shader_l->SetFloat("gamma", GameApplication::GetGamma());
    shader_l->SetFloat("contrast", GameApplication::GetContrast());

    std::shared_ptr<ShaderComponent> shader_r = std::make_shared<ShaderComponent>();
    shader_r->Create("Resources/shaders/refraction.vert", "Resources/shaders/refraction.frag");
    shader_r->Use();
    shader_r->SetFloat("brightness", GameApplication::GetBright());
    shader_r->SetFloat("gamma", GameApplication::GetGamma());
    shader_r->SetFloat("contrast", GameApplication::GetContrast());

    auto lineShader = std::make_shared<cmp::Shader>();
    lineShader->Create("Resources/shaders/line.vert", "Resources/shaders/line.frag");
    lineShader->Use();
    lineShader->SetFloat("brightness", GameApplication::GetBright());
    lineShader->SetFloat("gamma", GameApplication::GetGamma());
    lineShader->SetFloat("contrast", GameApplication::GetContrast());

    auto radialShader = std::make_shared<cmp::Shader>();
    radialShader->Create("Resources/shaders/radial.vert", "Resources/shaders/radial.frag");
    radialShader->Use();
    radialShader->SetFloat("brightness", GameApplication::GetBright());
    radialShader->SetFloat("gamma", GameApplication::GetGamma());
    radialShader->SetFloat("contrast", GameApplication::GetContrast());

    auto displShader = std::make_shared<ShaderComponent>();
    displShader->Create("Resources/shaders/displ.vert", "Resources/shaders/displ.frag");
    displShader->Use();
    displShader->SetFloat("brightness", GameApplication::GetBright());
    displShader->SetFloat("gamma", GameApplication::GetGamma());
    displShader->SetFloat("contrast", GameApplication::GetContrast());

    auto particleShader = std::make_shared<ShaderComponent>();
    particleShader->Create("Resources/shaders/particles.vert", "Resources/shaders/particles.frag");
    particleShader->Use();
    particleShader->SetFloat("brightness", GameApplication::GetBright());
    particleShader->SetFloat("gamma", GameApplication::GetGamma());
    particleShader->SetFloat("contrast", GameApplication::GetContrast());

    auto shadowParticleShader = std::make_shared<ShaderComponent>();
    shadowParticleShader->Create("Resources/shaders/shadowParticles.vert", "Resources/shaders/shadowParticles.frag");
    shadowParticleShader->Use();
    shadowParticleShader->SetFloat("brightness", GameApplication::GetBright());
    shadowParticleShader->SetFloat("gamma", GameApplication::GetGamma());
    shadowParticleShader->SetFloat("contrast", GameApplication::GetContrast());

    auto textShader = std::make_shared<ShaderComponent>();
    textShader->Create("Resources/shaders/text.vert", "Resources/shaders/text.frag");
    textShader->Use();
    textShader->SetFloat("brightness", GameApplication::GetBright());
    textShader->SetFloat("gamma", GameApplication::GetGamma());
    textShader->SetFloat("contrast", GameApplication::GetContrast());

    ///***

    world = new SceneNode(std::make_shared<GameObject>());
    world->GetGameObject()->AddComponent(std::make_shared<cmp::Name>("ROOT"));
    std::shared_ptr<SceneNode> main = std::make_shared<SceneNode>(std::make_shared<GameObject>());
    main->GetGameObject()->AddComponent(std::make_shared<cmp::Name>("MAIN"));
    world->FindNode("ROOT")->AddChild(main);
    std::shared_ptr<SceneNode> gui = std::make_shared<SceneNode>(std::make_shared<GameObject>());
    gui->GetGameObject()->AddComponent(std::make_shared<cmp::Name>("GUI"));
    world->FindNode("ROOT")->AddChild(gui);
    world->GetGameObject()->AddComponent(std::make_shared<cmp::Transform>());

    NODE_MAIN = main;
    NODE_GUI = gui;
    
    ///***

    shadowsManager = new ShadowsManager(world->FindNode("MAIN"), shader_l);

    ///***

    go = std::make_shared<GameObject>();
    go->AddComponent(std::make_shared<cmp::Transform>());
    go->AddComponent(std::make_shared<NameComponent>("CAMERA"));
    go->AddComponent(std::make_shared<CameraComponent>());
    go->GetComponent<cmp::Camera>()->Create(glm::vec3(-4,4.5,10)); // spawn room 0
    // go->GetComponent<cmp::Camera>()->Create(glm::vec3(-80.0f, 4.5f, 60.5f)); // spawn shooting blueprint
    go->GetComponent<cmp::Camera>()->SetSpeed(6.19251f);
    collidersManager = new CollidersManager(go); //mened???er kolider???w
    collidersManager->SetDistanceFromPlayer(10.0f);
    go->AddComponent(std::make_shared<BoxCollider>(false, false));
    go->GetComponent<cmp::BoxCol>()->SetLengths(glm::vec3(1.5,3,1.5));
    go->GetComponent<cmp::BoxCol>()->SetOffset(glm::vec3(0,-1,0));
    go->GetComponent<cmp::BoxCol>()->layer = CollisionLayer::Player;
    go->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collidersManager);
    go->AddComponent(std::make_shared<SphereCollider>(false, false));
    go->GetComponent<cmp::SphereCol>()->SetRadius(0.75);
    go->GetComponent<cmp::SphereCol>()->SetOffset(glm::vec3(0,-2.5,0));
    go->GetComponent<cmp::SphereCol>()->layer = CollisionLayer::Player;
    go->GetComponent<cmp::SphereCol>()->AddToCollidersManager(collidersManager);
    world->FindNode("MAIN")->AddChild(go);

    //FOR ENEMY SCRIPT
    MultiToolController* multiToolScript;
    multiToolScript = new MultiToolController();
    
    go->AddComponent(std::make_shared<cmp::Scriptable>());
    GameManager* gm = new GameManager();
    go->GetComponent<cmp::Scriptable>()->Add(gm);

    ///***

    auto playerGO = go;

    //ground check
    
    auto groundCheckGO = std::make_shared<GameObject>();
    groundCheckGO->AddComponent(std::make_shared<cmp::Transform>());
    groundCheckGO->GetComponent<cmp::Transform>()->SetPosition(0.0, -3.35, 0.0);

    groundCheckGO->AddComponent(std::make_shared<SphereCollider>(true, false));
    groundCheckGO->GetComponent<cmp::SphereCol>()->SetRadius(0.45f);
    groundCheckGO->GetComponent<cmp::SphereCol>()->AddToCollidersManager(collidersManager);

    groundCheckGO->AddComponent(std::make_shared<cmp::Scriptable>());

    auto groundCheckScript = new PlayerGroundCheck();
    groundCheckGO->GetComponent<cmp::Scriptable>()->Add(groundCheckScript);
    groundCheckScript->player = playerGO->GetComponent<cmp::Camera>();

    world->FindNode("CAMERA")->AddChild(groundCheckGO);

    //skrypty gracza
    

    auto debugLineGO = std::make_shared<GameObject>();
    auto debugLineCmp = std::make_shared<cmp::Line>();
    debugLineCmp->Create();
    debugLineGO->AddComponent(debugLineCmp);
    debugLineGO->AddComponent(std::make_shared<cmp::Transform>());
    world->FindNode("MAIN")->FindNode("CAMERA")->AddChild(debugLineGO);
    
    auto playerPlace = new PlayerPlaceTurret();
 
    playerPlace->gameManager = gm;
    playerPlace->line = debugLineCmp;
    playerPlace->colMan = collidersManager;
    playerPlace->resMan = resMan;
    playerPlace->turretShader = shader_l;
    playerPlace->ghostShader = shader_d;
    playerPlace->lineShader = lineShader;
    playerPlace->shootingParticleShader = particleShader;

    //to fix rendering order - moved at the end
    // auto turretsHolderGO = std::make_shared<GameObject>();
    // turretsHolderGO->AddComponent(std::make_shared<cmp::Transform>());
    // auto turretsHolderNode = world->FindNode("MAIN")->AddChild(turretsHolderGO);

    // playerPlace->turretsHolder = turretsHolderNode.get();
    
    go->GetComponent<ScriptComponent>()->Add(playerPlace);


    auto playerInteract = new PlayerInteract();
    
    playerInteract->colMan = collidersManager;
    playerInteract->camera = playerGO->GetComponent<cmp::Camera>();
    playerInteract->placeTurretScript = playerPlace;

    auto crosshairTextTEMP = std::make_shared<TextComponent>();
    playerInteract->textTEMP = crosshairTextTEMP;

    go->GetComponent<ScriptComponent>()->Add(playerInteract);


    auto playerFootsteps = new PlayerFootsteps();
    playerFootsteps->camera = go->GetComponent<cmp::Camera>().get();
    playerFootsteps->groundCheck = groundCheckScript;
    go->GetComponent<ScriptComponent>()->Add(playerFootsteps);

    
    //MultiToolController* multiToolScript; MOVED BEFORE MAP LOADER
    //multi tool
    SceneNode* multiToolDisplayNode;
    {
        auto multiTool = std::make_shared<GameObject>();
        multiTool->AddComponent(std::make_shared<cmp::Name>("MultiTool"));

        multiTool->AddComponent(std::make_shared<cmp::Transform>()); 
        multiTool->GetComponent<cmp::Transform>()->SetPosition(0.509937, -0.257875, -0.8f);
        multiTool->GetComponent<cmp::Transform>()->SetRotation(-0.076019f, 0.0f, 0.0f);

        auto baseMesh = std::make_shared<cmp::Model>();
        baseMesh->Create(
            resMan->GetMesh("Resources/models/multitool_new/multitool.obj"),
            resMan->GetMaterial("Resources/models/multitool_new/multitool.mtl")
        );
       
        multiTool->AddComponent(baseMesh);
        multiTool->AddComponent(shader_l);

        world->FindNode("MAIN")->AddChild(multiTool);
        auto multiToolNode = world->FindNode("MAIN")->FindNode("MultiTool");


        multiTool->AddComponent(std::make_shared<cmp::Scriptable>());

        //multiToolScript = new MultiToolController(); MOVED BEFORE MAP LOADER
        multiTool->GetComponent<cmp::Scriptable>()->Add(multiToolScript);
        playerGO->GetComponent<cmp::Scriptable>()->Get<PlayerPlaceTurret>()->multiTool = multiToolScript;
        multiToolScript->gm = gm;


        auto mutliToolDisplayHolder = std::make_shared<GameObject>();
        mutliToolDisplayHolder->AddComponent(std::make_shared<cmp::Transform>());
        mutliToolDisplayHolder->GetComponent<cmp::Transform>()->SetPosition(0.0016f, -0.003329f, 0.262514f);
        mutliToolDisplayHolder->GetComponent<cmp::Transform>()->SetRotation(-10, 0, 0);
        auto displayNode = multiToolNode->AddChild(mutliToolDisplayHolder);
        multiToolDisplayNode = displayNode.get();
        mutliToolDisplayHolder->AddComponent(std::make_shared<cmp::Name>("mtDisplay"));

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
            std::string iconMaterialPath = "Resources/models/multitool/icon" + std::to_string(i+1) + ".mtl";
            iconModel->Create(
                resMan->GetMesh("Resources/models/multitool/icon.obj"),
                resMan->GetMaterial(iconMaterialPath)
            );
            icon->AddComponent(iconModel);
            icon->AddComponent(shader_d);

            iconModel->SetTintColor(0.17, 0.17, 0.17,  1.0);
            multiToolScript->iconsGO[i+1] = icon;

            icon->AddComponent(std::make_shared<cmp::Transform>());
            icon->GetComponent<cmp::Transform>()->SetPosition(i * 0.06, 0.05, 0.0);

            displayNode->AddChild(icon);
        }

        //multitool light
        auto flashLightGO = std::make_shared<GameObject>();
        flashLightGO->AddComponent(std::make_shared<cmp::SpotLight>());
        auto lightCmp = flashLightGO->GetComponent<cmp::SpotLight>();
        lightCmp->Create();
        lightCmp->AddShader(shader_l);
        lightCmp->AddShader(shadowParticleShader);
        lightCmp->SetPosition(multiTool->GetComponent<cmp::Transform>()->GetPosition());
        lightCmp->SetLightColor({0.25f, 0.02f, 0.58f});
        lightCmp->SetCutOff({38.5f, 40.0f});
        flashLightGO->AddComponent(std::make_shared<cmp::Name>("Flashlight"));
        world->FindNode("MAIN")->AddChild(flashLightGO);

        multiToolScript->flashlight = lightCmp;

        

        GO_MULTITOOL = multiTool;
        GO_FLASHLIGHT = flashLightGO;
    }

    //"Zdrowie" gracza
    auto healthGo = std::make_shared<GameObject>();
    healthGo->AddComponent(std::make_shared<cmp::Name>("PlayerHealthGO"));
    healthGo->AddComponent(std::make_shared<cmp::Transform>());
    healthGo->GetComponent<cmp::Transform>()->SetPosition(0.0, 0.0, 0.0);

    healthGo->AddComponent(std::make_shared<SphereCollider>(true, false));
    healthGo->GetComponent<cmp::SphereCol>()->SetRadius(0.45f);
    healthGo->GetComponent<cmp::SphereCol>()->AddToCollidersManager(collidersManager);

    healthGo->AddComponent(std::make_shared<cmp::Scriptable>());

    auto healthScript = new PlayerHealthScript();
    healthGo->GetComponent<cmp::Scriptable>()->Add(healthScript);
    healthScript->player = playerGO->GetComponent<cmp::Camera>();

    world->FindNode("CAMERA")->AddChild(healthGo);


    //LOAD SCENE
    auto levelsNode = std::make_shared<SceneNode>(std::make_shared<GameObject>());
    levelsNode->GetGameObject()->AddComponent(std::make_shared<cmp::Name>("LEVELS"));
    world->FindNode("MAIN")->AddChild(levelsNode);

    SCENE_INFO = {
        shader_l, shader_d, lineShader, displShader, particleShader, shadowParticleShader, resMan, collidersManager, shadowsManager, this, multiToolScript, go
    };

    currentLevelIndex = 0;
    newLevelToSwitch = -1;
    LoadLevelTutorial(SCENE_INFO);


    //=== fixing rendering order
    auto turretsHolderParentGO = std::make_shared<GameObject>();
    turretsHolderParentGO->AddComponent(std::make_shared<cmp::Transform>());
    turretsHolderParentGO->AddComponent(std::make_shared<cmp::Name>("TURRETS_HOLDER"));
    auto turretsHolderParentNode = world->FindNode("MAIN")->AddChild(turretsHolderParentGO);

    auto turretsHolderGO = std::make_shared<GameObject>();
    turretsHolderGO->AddComponent(std::make_shared<cmp::Transform>());
    auto turretsHolderNode = turretsHolderParentNode->AddChild(turretsHolderGO);

    playerPlace->turretsHolder = turretsHolderNode.get();

    //=== text

    //renderowany jako ostatni bo inaczej sa te dziwne artefakty
    {
        Font* font = resMan->GetFont("Resources/fonts/good-times-rg.otf");

        go = std::make_shared<GameObject>();
        auto tc = std::make_shared<TransformComponent>();

        auto textComponent = std::make_shared<TextComponent>();
        textComponent->Create("WASD / LMB - Rotate\nSHIFT - Slow down", font);
        textComponent->alwaysSeen = true;
        textComponent->isGuiElement = true;
        textComponent->color = { 0.8f, 0.8f, 0.8f };

        go->AddComponent(textShader);
        go->AddComponent(tc);
        go->AddComponent(textComponent);

        go->GetComponent<TransformComponent>()->SetPosition(0.05f, 0.1f, 0.0f);
        go->GetComponent<TransformComponent>()->SetScale(0.0f);
        go->AddComponent(std::make_shared<cmp::Name>("MirrorControlsText"));

        world->FindNode("GUI")->AddChild(go);
    }
    {
        Font* font = resMan->GetFont("Resources/fonts/Segment7-4Gml.otf");

        go = std::make_shared<GameObject>();
        auto tc = std::make_shared<TransformComponent>();
        auto energyTextComponentTEMP = std::make_shared<TextComponent>();
        energyTextComponentTEMP->Create("0", font);
        energyTextComponentTEMP->alwaysSeen = true;
        //energyTextComponentTEMP->isCentered = true;
        // energyTextComponentTEMP->isGuiElement = true;
        energyTextComponentTEMP->color = {1.0f, 0.6f, 0.0f};

        
        go->AddComponent(textShader);
        go->AddComponent(tc);
        go->AddComponent(energyTextComponentTEMP);

        go->GetComponent<TransformComponent>()->SetPosition(-0.02341f, -0.035f, 0.0f);
        go->GetComponent<TransformComponent>()->SetScale(0.07f);
        go->AddComponent(std::make_shared<cmp::Name>("EnergyText"));
        
        multiToolDisplayNode->AddChild(go);
    }
    {
        Font* font = resMan->GetFont("Resources/fonts/good-times-rg.otf");
        go = std::make_shared<GameObject>();
        auto tc = std::make_shared<TransformComponent>();

        auto textComponent = std::make_shared<TextComponent>();
        textComponent->Create("Press T for\nthe toollight", font);
        textComponent->alwaysSeen = true;
        textComponent->isGuiElement = true;
        textComponent->color = { 0.8f, 0.8f, 0.8f };

        go->AddComponent(textShader);
        go->AddComponent(tc);
        go->AddComponent(textComponent);

        go->GetComponent<TransformComponent>()->SetPosition(0.35f, 0.2f, 0.0f);
        go->GetComponent<TransformComponent>()->SetScale(0.0f);
        go->AddComponent(std::make_shared<cmp::Name>("TutorialFlash"));

        world->FindNode("GUI")->AddChild(go);
    }


    //== tooltip
    {
        go = std::make_shared<GameObject>();
        auto tc = std::make_shared<TransformComponent>();
        auto modelGui = std::make_shared<ModelGuiComponent>();
        modelGui->Create("Resources/textures/mouse256.png");
        modelGui->alwaysSeen = true;
        modelGui->isGuiElement = true;

        go->AddComponent(shader_d);
        go->AddComponent(tc);
        go->AddComponent(modelGui);

        go->GetComponent<TransformComponent>()->SetScale(0.0f);

        go->AddComponent(std::make_shared<cmp::Name>("Tooltip"));

        world->FindNode("GUI")->AddChild(go);

        GO_TOOLTIP = go;
    }

    //
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
        
        go->AddComponent(textShader);
        go->AddComponent(tc);
        go->AddComponent(crosshairTextTEMP);

        // go->GetComponent<TransformComponent>()->SetPosition(0.1f, 0.1f, 0.1f);
        go->GetComponent<TransformComponent>()->SetScale(0.1f);
        go->AddComponent(std::make_shared<cmp::Name>("CROSSHAIR"));
        
        world->FindNode("GUI")->AddChild(go);

        GO_CROSSHAIR = go;
    }


    //===

    world->LoadScripts();


    // PREVENT CAMERA AND MIRRORS FROM GOING TO INFINITY 
    //(i tak raz na ktores uruchomienie kamera jest w 0 1312321321312 -10;  trzeba to ogarnac)
    std::shared_ptr<GameObject> goCamera = world->FindNode("CAMERA")->GetGameObject();
    std::shared_ptr<TransformComponent> transformCamera = goCamera->GetComponent<cmp::Transform>();

    goCamera->GetComponent<CameraComponent>()->Update(GameApplication::GetInputManager(), 0.0f);
    transformCamera->SetPosition(goCamera->GetComponent<CameraComponent>()->GetPosition());
    goCamera->GetNode()->ResetGlobalTransformations();

    goCamera->GetComponent<CameraComponent>()->SetPosition(transformCamera->GetPosition());
    transform = GameApplication::GetProjection() * goCamera->GetComponent<CameraComponent>()->GetView();


    world->Update(0.0f);
}

void Scene::LoadLevelTutorial(const SceneInfo& sceneInfo)
{
    auto levelNode = std::make_shared<SceneNode>(std::make_shared<GameObject>());
    levelNode->GetGameObject()->AddComponent(std::make_shared<cmp::Name>("LEVEL_TUTORIAL"));
    world->FindNode("LEVELS")->AddChild(levelNode);
    MapLoader::Load("Resources/maps/world_newtutorial.map", levelNode.get(), 
        sceneInfo.shader_l, 
        sceneInfo.shader_d, 
        sceneInfo.lineShader, 
        sceneInfo.displShader,
        sceneInfo.particlesShader,
        sceneInfo.shadowParticlesShader,
        sceneInfo.collidersManager, 
        sceneInfo.shadowsManager,
        sceneInfo.scene, 
        sceneInfo.multiToolScript,
        sceneInfo.cameraGO);

    sceneInfo.cameraGO->GetComponent<cmp::Camera>()->RestartMovement(-4, 4.5, 10);

    //restart picked blueprints
    sceneInfo.multiToolScript->Lock(0);
    sceneInfo.multiToolScript->Lock(1);
    sceneInfo.cameraGO->GetComponent<cmp::Scriptable>()->Get<PlayerPlaceTurret>()->unlocked[0] = false;
    sceneInfo.cameraGO->GetComponent<cmp::Scriptable>()->Get<PlayerPlaceTurret>()->unlocked[1] = false;
    sceneInfo.cameraGO->GetComponent<cmp::Scriptable>()->Get<PlayerPlaceTurret>()->selectedTurretType = PlayerPlaceTurret::Laser;
    sceneInfo.multiToolScript->SetActiveIcon(0);


    //===enemy
    {

        auto go = std::make_shared<GameObject>();
        auto mc = std::make_shared<ModelComponent>();
        mc->Create(
            //sceneInfo.resourceManager->GetMesh("Resources/models/ny/przeciwnik/przeciwnik/przeciwniksmooth.obj"),
            sceneInfo.resourceManager->GetMesh("Resources/models/enemy/enemy.obj"),
            sceneInfo.resourceManager->GetMaterial("Resources/models/displacement test/capsule.mtl")
        );
        go->AddComponent(sceneInfo.displShader);
        go->AddComponent(mc);

        go->AddComponent(std::make_shared<FrustumCullingComponent>());
        go->GetComponent<cmp::FrustumCulling>()->Create(
            //sceneInfo.resourceManager->GetMesh("Resources/models/ny/przeciwnik/przeciwnik/przeciwnik.obj")
            sceneInfo.resourceManager->GetMesh("Resources/models/enemy/enemy.obj")
        );

        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(-83.0f, -10, 6);
        //go->GetComponent<cmp::Transform>()->SetRotation(0,90,0);
        go->GetComponent<cmp::Transform>()->SetScale(0.50f);


        go->AddComponent(std::make_shared<cmp::Name>("CutsceneEnemy"));

        go->AddComponent(std::make_shared<BoxCollider>(true, false, CollisionLayer::ENEMY));
        go->GetComponent<cmp::BoxCol>()->SetLengths(glm::vec3(2, 2, 2));
        go->GetComponent<cmp::BoxCol>()->isOptimized = false;

        go->AddComponent(std::make_shared<cmp::Scriptable>());
        Health* health = new Health();
        health->SetMaxHealth(50.0f);
        health->scene = this;
        go->GetComponent<cmp::Scriptable>()->Add(health);
        auto particles = std::make_shared<ParticleComponent>();

        particles->Create(sceneInfo.cameraGO->GetComponent<cmp::Camera>(), false, 40, sceneInfo.particlesShader);
        particles->SetTexture("Resources/textures/shadowParticle.png");
        particles->SetParticlesPerSecond(20.0f);
        particles->SetOffset({ 0, 0, 0 });
        particles->SetDirection({ 0.0f, -1.0f, 0.0f });
        particles->SetDirectionVar(20);
        particles->SetParticleLifetime(2.0f);
        particles->SetScale(0.2f, 0.2f);
        particles->SetSpeed(3.0f);
        particles->SetColor({ 1.0,1.0,1.0,1.0 });
        particles->SetForce({ 0.0f, -0.01f, 0.0f });

        go->AddComponent(particles);

        sceneInfo.cameraGO->GetComponent<cmp::Scriptable>()->Get<GameManager>()->immortal = true;

        auto enemyScript = new EnemyScript(sceneInfo.cameraGO);
        enemyScript->health = health;
        enemyScript->multitool = sceneInfo.multiToolScript;
        std::shared_ptr<Path> path = std::make_shared<Path>(true);
        path->Clear();
        path->AddWayPoint({ - 83.0f, 2.0, 6});
        path->AddWayPoint({ -84.72, 2.0, 28 });
        path->Set();
        enemyScript->SetPath(path);
        go->GetComponent<cmp::Scriptable>()->Add(enemyScript);
        go->GetComponent<cmp::Scriptable>()->DisableAll();

        //right arm
        auto rightArm = std::make_shared<GameObject>();
        mc = std::make_shared<ModelComponent>();
        mc->Create(
            //sceneInfo.resourceManager->GetMesh("Resources/models/ny/przeciwnik/przeciwnik/przeciwniksmooth.obj"),
            sceneInfo.resourceManager->GetMesh("Resources/models/enemy/enemyRightArm.obj"),
            sceneInfo.resourceManager->GetMaterial("Resources/models/displacement test/capsule.mtl")
        );
        rightArm->AddComponent(sceneInfo.displShader);
        rightArm->AddComponent(mc);
        rightArm->AddComponent(std::make_shared<FrustumCullingComponent>());
        rightArm->GetComponent<cmp::FrustumCulling>()->Create(
            //sceneInfo.resourceManager->GetMesh("Resources/models/ny/przeciwnik/przeciwnik/przeciwnik.obj")
            sceneInfo.resourceManager->GetMesh("Resources/models/enemy/enemyRightArm.obj")
        );

        rightArm->AddComponent(std::make_shared<cmp::Transform>());
        rightArm->GetComponent<cmp::Transform>()->SetPosition(-1, 4, 0);
        rightArm->GetComponent<cmp::Transform>()->SetRotation(0, -90, 0);
        rightArm->AddComponent(std::make_shared<cmp::Name>("EnemyRightArm"));

        //left arm
        auto leftArm = std::make_shared<GameObject>();
        mc = std::make_shared<ModelComponent>();
        mc->Create(
            //sceneInfo.resourceManager->GetMesh("Resources/models/ny/przeciwnik/przeciwnik/przeciwniksmooth.obj"),
            sceneInfo.resourceManager->GetMesh("Resources/models/enemy/enemyLeftArm.obj"),
            sceneInfo.resourceManager->GetMaterial("Resources/models/displacement test/capsule.mtl")
        );
        leftArm->AddComponent(sceneInfo.displShader);
        leftArm->AddComponent(mc);
        leftArm->AddComponent(std::make_shared<FrustumCullingComponent>());
        leftArm->GetComponent<cmp::FrustumCulling>()->Create(
            //sceneInfo.resourceManager->GetMesh("Resources/models/ny/przeciwnik/przeciwnik/przeciwnik.obj")
            sceneInfo.resourceManager->GetMesh("Resources/models/enemy/enemyLeftArm.obj")
        );

        leftArm->AddComponent(std::make_shared<cmp::Transform>());
        leftArm->GetComponent<cmp::Transform>()->SetPosition(1, 4, 0);
        leftArm->GetComponent<cmp::Transform>()->SetRotation(0, -90, 0);
        leftArm->AddComponent(std::make_shared<cmp::Name>("EnemyLeftArm"));

        levelNode->AddChild(go)->AddChild(rightArm);
        go->GetNode()->AddChild(leftArm);

        auto deathBall = std::make_shared<GameObject>();
        mc = std::make_shared<ModelComponent>();
        mc->Create(
            //sceneInfo.resourceManager->GetMesh("Resources/models/ny/przeciwnik/przeciwnik/przeciwniksmooth.obj"),
            sceneInfo.resourceManager->GetMesh("Resources/models/Sphere/Sphere.obj"),
            sceneInfo.resourceManager->GetMaterial("Resources/models/displacement test/capsule.mtl")
        );
        deathBall->AddComponent(sceneInfo.displShader);
        deathBall->AddComponent(mc);
        deathBall->AddComponent(std::make_shared<FrustumCullingComponent>());
        deathBall->GetComponent<cmp::FrustumCulling>()->Create(
            //sceneInfo.resourceManager->GetMesh("Resources/models/ny/przeciwnik/przeciwnik/przeciwnik.obj")
            sceneInfo.resourceManager->GetMesh("Resources/models/Sphere/Sphere.obj")
        );

        deathBall->AddComponent(std::make_shared<cmp::Transform>());
        deathBall->GetComponent<cmp::Transform>()->SetScale(0.01f);
        deathBall->AddComponent(std::make_shared<cmp::Name>("DeathBall"));

        go->GetNode()->AddChild(deathBall);
    }


    //doors and activators
    struct DoorAndActivatorPair
    {
        glm::vec3 doorPosition;
        float doorRotationY;
        bool openOnStart;
        glm::vec3 activatorPosition;
        float activatorRotation;
    };

    std::vector<DoorAndActivatorPair> doorsAndButtons = {  //0 rot = Z aligned; 90 rot = X aligned
        { { -15.00, 3.0,  25.0 }, 90.0f, false,  { -20.0,  2.5,  19.90 }, 180.0f },  //room 1
        { { -25.00, 3.0,  50.5 }, 90.0f, false,  { -20.0,  2.5,  46.90 }, 180.0f },  //room 2
        { { -64.75, 3.0,  60.0 },  0.0f, false,  { -60.9, -9.0,  55.50 },  90.0f },  //room 3 - cutscene close (important id)
        { { -83.00, 3.0,  -3.5 }, 90.0f, false,  { -60.9, -9.0,  64.60 },   0.0f },  //room 4 - cutscene open  (important id)
                                         

        { { -65.30, 3.0, -12.0 },  0.0f,  true,  { -68.0,  2.5, -16.65 },  -90.0f }, //room 4 - enemy side door
        { { -83.00, 3.0, -28.5 }, 90.0f, false,  { -83.00, -9.0, -27.5 },  90.0f },  //room 4 - elevator door

        { { -64.75, 9.0,  77.0 },  0.0f, false,  { -60.75, 9.0,  81.90 },  180.0f },  // vent path
    };

    int i = 0;
    DoorActivator* cutsceneDoorActivator = nullptr;
    std::shared_ptr<GameObject> openDoorAfterEnemyDies = nullptr;
    DoorActivator* cutsceneDoorActivator2 = nullptr;
    DoorActivator* openElevator = nullptr;
    for (auto& [doorPosition, doorRotation, openOnStart, activatorPosition, activatorRotation] : doorsAndButtons)
    {
        glm::vec3 doorOffset = { 0.0f, 6.1f, 0.0f };

        //create door
        auto go = std::make_shared<GameObject>();

        auto doorTransform = std::make_shared<cmp::Transform>();
        doorTransform->SetPosition(doorPosition + (openOnStart ? doorOffset : glm::vec3(0)));
        doorTransform->SetRotation(0, doorRotation, 0);
        float scl = 1.0f;
        doorTransform->SetScale(scl);
        go->AddComponent(doorTransform);

        go->AddComponent(std::make_shared<BoxCollider>(false, true));
        if (doorRotation < 0.001f) go->GetComponent<cmp::BoxCol>()->SetLengths({ 2.0 * scl, 7.0 * scl, 8.0 * scl });
        else go->GetComponent<cmp::BoxCol>()->SetLengths({ 8.0 * scl, 7.0 * scl, 2.0 * scl });
        go->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collidersManager);

        auto model = std::make_shared<cmp::Model>();
        model->Create(
            sceneInfo.resourceManager->GetMesh("Resources/models/doors/drzwi.obj"),
            sceneInfo.resourceManager->GetMaterial("Resources/models/doors/drzwi.mtl")
        );
        // model->SetTintColor(1.0, 0.5, 0.0);
        go->AddComponent(model);
        go->AddComponent(sceneInfo.shader_l);

        levelNode->AddChild(go);
        
        //prowadnica
        {
            auto go = std::make_shared<GameObject>();

            auto doorTransform = std::make_shared<cmp::Transform>();
            doorTransform->SetPosition(doorPosition);
            doorTransform->SetRotation(0, doorRotation, 0);
            float scl = 1.0f;
            doorTransform->SetScale(scl);
            go->AddComponent(doorTransform);

            auto model = std::make_shared<cmp::Model>();
            model->Create(
                sceneInfo.resourceManager->GetMesh("Resources/models/doors/prowadnica.obj"),
                sceneInfo.resourceManager->GetMaterial("Resources/models/doors/drzwi.mtl")
            );
            go->AddComponent(model);
            go->AddComponent(sceneInfo.shader_l);

            levelNode->AddChild(go);
        }

        //create activator
        go = std::make_shared<GameObject>();
        go->AddComponent(std::make_shared<cmp::Name>("DoorActivator" + std::to_string(i)));

        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(activatorPosition);
        go->GetComponent<cmp::Transform>()->SetRotation(0.0, activatorRotation, 0.0f);

        go->AddComponent(std::make_shared<SphereCollider>(false, true));
        go->GetComponent<cmp::SphereCol>()->SetRadius(0.18);
        go->GetComponent<cmp::SphereCol>()->AddToCollidersManager(collidersManager);

        model = std::make_shared<cmp::Model>();
        model->Create(
            sceneInfo.resourceManager->GetMesh("Resources/models/ny/przelacznikLaserowy/przelacznikLaserowy/przelacznikDetector.obj"),
            sceneInfo.resourceManager->GetMaterial("Resources/models/ny/przelacznikLaserowy/przelacznikLaserowy/przelacznikLaserowy.mtl")
        );
        model->SetTintColor(0.88, 0.21, 0.21);
        go->AddComponent(model);
        go->AddComponent(sceneInfo.shader_d);

        go->AddComponent(std::make_shared<cmp::FrustumCulling>());
        go->GetComponent<cmp::FrustumCulling>()->Create(sceneInfo.resourceManager->GetMesh("Resources/models/ny/przelacznikLaserowy/przelacznikLaserowy/przelacznikDetector.obj"));


        go->AddComponent(std::make_shared<cmp::Scriptable>());

        auto activator = new DoorActivator();
        if(i!=3)
        activator->doorTransform = doorTransform;
        activator->openedOffset = doorOffset * (openOnStart ? -1.0f : 1.0f);
        go->GetComponent<cmp::Scriptable>()->Add(activator);

        if (i == 2) cutsceneDoorActivator = activator;
        if (i == 3) openDoorAfterEnemyDies = doorTransform->GetOwner();
        if (i == 4)
        {
            cutsceneDoorActivator2 = activator;
        }
        if (i == 5) openElevator = activator;
        i++;

        //- frame model
        auto frameGO = std::make_shared<GameObject>();
        frameGO->AddComponent(std::make_shared<cmp::Transform>());
        model = std::make_shared<cmp::Model>();
        model->Create(
            sceneInfo.resourceManager->GetMesh("Resources/models/ny/przelacznikLaserowy/przelacznikLaserowy/przelacznikFrame.obj"),
            sceneInfo.resourceManager->GetMaterial("Resources/models/ny/przelacznikLaserowy/przelacznikLaserowy/przelacznikLaserowy.mtl")
        );
        frameGO->AddComponent(model);
        frameGO->AddComponent(sceneInfo.shader_l);
        frameGO->AddComponent(std::make_shared<cmp::FrustumCulling>());
        frameGO->GetComponent<cmp::FrustumCulling>()->Create(sceneInfo.resourceManager->GetMesh("Resources/models/ny/przelacznikLaserowy/przelacznikLaserowy/przelacznikFrame.obj"));

        levelNode->AddChild(go)->AddChild(frameGO);
    }

    {//broken aactivator activator
        auto go = std::make_shared<GameObject>();
        go->AddComponent(std::make_shared<cmp::Name>("BrokenActivator"));
        
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(-59.34, 0.1, 57.45);
        go->GetComponent<cmp::Transform>()->SetRotation(-90.0f, 30.0f, 0.0f);

        auto model = std::make_shared<cmp::Model>();
        model->Create(
            sceneInfo.resourceManager->GetMesh("Resources/models/ny/przelacznikLaserowy/przelacznikLaserowy/przelacznikDetector.obj"),
            sceneInfo.resourceManager->GetMaterial("Resources/models/ny/przelacznikLaserowy/przelacznikLaserowy/przelacznikLaserowy.mtl")
        );
        model->SetTintColor(0.11, 0.01, 0.01);
        go->AddComponent(model);
        go->AddComponent(sceneInfo.shader_d);

        go->AddComponent(std::make_shared<cmp::FrustumCulling>());
        go->GetComponent<cmp::FrustumCulling>()->Create(sceneInfo.resourceManager->GetMesh("Resources/models/ny/przelacznikLaserowy/przelacznikLaserowy/przelacznikDetector.obj"));

        auto frameGO = std::make_shared<GameObject>();
        frameGO->AddComponent(std::make_shared<cmp::Transform>());
        model = std::make_shared<cmp::Model>();
        model->Create(
            sceneInfo.resourceManager->GetMesh("Resources/models/ny/przelacznikLaserowy/przelacznikLaserowy/przelacznikFrame.obj"),
            sceneInfo.resourceManager->GetMaterial("Resources/models/ny/przelacznikLaserowy/przelacznikLaserowy/przelacznikLaserowy.mtl")
        );
        frameGO->AddComponent(model);
        frameGO->AddComponent(sceneInfo.shader_l);
        frameGO->AddComponent(std::make_shared<cmp::FrustumCulling>());
        frameGO->GetComponent<cmp::FrustumCulling>()->Create(sceneInfo.resourceManager->GetMesh("Resources/models/ny/przelacznikLaserowy/przelacznikLaserowy/przelacznikFrame.obj"));

        levelNode->AddChild(go)->AddChild(frameGO);
    }

    //cutscene
    
    auto go = std::make_shared<GameObject>();
    go->AddComponent(std::make_shared<cmp::Name>("cutscene"));

    go->AddComponent(std::make_shared<cmp::Transform>());
    go->GetComponent<cmp::Transform>()->SetPosition(-83.0f, 5.0f, 42.0f);

    go->AddComponent(std::make_shared<BoxCollider>(true, true));
    go->GetComponent<cmp::BoxCol>()->SetLengths({ 10.0, 10.0, 5.0 });
    go->GetComponent<cmp::BoxCol>()->layer = CollisionLayer::Ignore;
    go->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collidersManager);


    go->AddComponent(std::make_shared<cmp::Scriptable>());
    
    //--alarm gameobject
    glm::vec3 alarmPos = { -83.0f, 7.5f, 0.5f };

    auto cutscene = new Cutscenexd();
    cutscene->doorsToShut = cutsceneDoorActivator;
    cutscene->doorsToOpen = openDoorAfterEnemyDies;
    cutscene->blueprint = world->FindNode("Blueprint 1")->GetGameObject();
    cutscene->camera = world->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Camera>();
    cutscene->enemy = world->FindNode("CutsceneEnemy")->GetGameObject();
    cutscene->lightShader = sceneInfo.shader_l;
    cutscene->shadowManager = shadowsManager;
    cutscene->colMan = collidersManager;
    cutscene->dispShader = sceneInfo.displShader;
    cutscene->generator = world->FindNode("bulb2")->GetGameObject();
    std::shared_ptr<cmp::PointLight> genLight = world->FindNode("generatorLight2")->GetGameObject()->GetComponent<cmp::PointLight>();
    cutscene->generatorLight = genLight;
    cutscene->resMan = sceneInfo.resourceManager;
    go->GetComponent<cmp::Scriptable>()->Add(cutscene);

    levelNode->AddChild(go);
        
    for (int i = 0; i < 2; i++)
    {
        std::shared_ptr<GameObject> turret = std::make_shared<GameObject>();
        auto mc = std::make_shared<cmp::Model>();
        mc->Create(
            sceneInfo.resourceManager->GetMesh("Resources/models/Wieze/Strzelajaca.obj"),
            sceneInfo.resourceManager->GetMaterial("Resources/models/Wieze/Strzelajaca.mtl")
        );
        turret->AddComponent(mc);
        mc->SetTintColor(1.0,1.0,1.0);
        turret->AddComponent(std::make_shared<cmp::Shade>());
        turret->GetComponent<cmp::Shade>()->Create(1);
        turret->AddComponent(sceneInfo.shader_l);
        turret->AddComponent(std::make_shared<cmp::Transform>());
        if (i == 0)
        {
            turret->GetComponent<cmp::Transform>()->SetPosition({ -92.72f, 0.0f, 24.0f });
            cutscene->shooter1 = turret;
        }
        else
        {
            turret->GetComponent<cmp::Transform>()->SetPosition({ -92.72f, 0.0f, 32.0f });
            cutscene->shooter2 = turret;
        }
        turret->GetComponent<cmp::Transform>()->SetRotation({ 0.0f, 180.0f, 0.0f });
        turret->AddComponent(std::make_shared<cmp::FrustumCulling>());
        turret->GetComponent<cmp::FrustumCulling>()->Create(
            sceneInfo.resourceManager->GetMesh("Resources/models/Wieze/Strzelajaca.obj"));

        turret->AddComponent(std::make_shared<cmp::BoxCol>(true, true));
        std::shared_ptr<cmp::BoxCol> col = turret->GetComponent<cmp::BoxCol>();
        col->SetLengths({ 2.0, 2.5, 2.0 });
        col->SetOffset({ 0.0, 2.0, 0.0 });
        col->AddToCollidersManager(collidersManager);

        turret->AddComponent(std::make_shared<cmp::Scriptable>());
        TurretShoot* script = new TurretShoot();
        script->isPut = true;
        script->particleShader = sceneInfo.particlesShader;
        turret->GetComponent<cmp::Scriptable>()->Add(script);


        auto rangeGO = std::make_shared<GameObject>();
        rangeGO->AddComponent(std::make_shared<cmp::Transform>());

        rangeGO->AddComponent(std::make_shared<cmp::SphereCol>(true, true));
        std::shared_ptr<cmp::SphereCol> rangeCol = rangeGO->GetComponent<cmp::SphereCol>();
        rangeCol->SetRadius(15.0f);
        rangeCol->AddToCollidersManager(collidersManager);

        rangeGO->AddComponent(std::make_shared<cmp::Scriptable>());
        TurretRange* range = new TurretRange();
        script->turretRange = range;
        rangeGO->GetComponent<cmp::Scriptable>()->Add(range);


        levelNode->AddChild(turret)->AddChild(rangeGO);
    }
    go = std::make_shared<GameObject>();

    auto doorTransform = std::make_shared<cmp::Transform>();
    doorTransform->SetPosition(-89,9.05,28);
    doorTransform->SetRotation(0, 0, 0);
    doorTransform->SetScale({ 1,1,5 });
    go->AddComponent(doorTransform);

    auto model = std::make_shared<cmp::Model>();
    model->Create(
        sceneInfo.resourceManager->GetMesh("Resources/models/doors/drzwi.obj"),
        sceneInfo.resourceManager->GetMaterial("Resources/models/doors/drzwi.mtl")
    );
    // model->SetTintColor(1.0, 0.5, 0.0);
    go->AddComponent(model);
    go->AddComponent(sceneInfo.shader_l);
    cutscene->shootingDoor = go;
    levelNode->AddChild(go);
        
    auto alarmGO = std::make_shared<GameObject>();
    alarmGO->AddComponent(std::make_shared<cmp::Name>("CutscenexdAlarmGO"));

    alarmGO->AddComponent(std::make_shared<cmp::Transform>());
    alarmGO->GetComponent<cmp::Transform>()->SetPosition(alarmPos);
    alarmGO->GetComponent<cmp::Transform>()->SetRotation(0.0f, 0.0f, 180.0f);
    alarmGO->GetComponent<cmp::Transform>()->SetScale(3.0f);

    model = std::make_shared<cmp::Model>();
    model->Create(
        sceneInfo.resourceManager->GetMesh("Resources/models/ny/przelacznikLaserowy/przelacznikLaserowy/przelacznikDetector.obj"),
        sceneInfo.resourceManager->GetMaterial("Resources/models/ny/przelacznikLaserowy/przelacznikLaserowy/przelacznikLaserowy.mtl")
    );
    model->SetTintColor(0.01, 0.01, 0.01);
    alarmGO->AddComponent(model);
    alarmGO->AddComponent(sceneInfo.shader_d);

    alarmGO->AddComponent(std::make_shared<cmp::FrustumCulling>());
    alarmGO->GetComponent<cmp::FrustumCulling>()->Create(sceneInfo.resourceManager->GetMesh("Resources/models/ny/przelacznikLaserowy/przelacznikLaserowy/przelacznikDetector.obj"));

    auto frameGO = std::make_shared<GameObject>();
    frameGO->AddComponent(std::make_shared<cmp::Transform>());
    frameGO->GetComponent<cmp::Transform>()->SetPosition(-.02, -.025, -.2);
    frameGO->GetComponent<cmp::Transform>()->SetScale(0.2);
    model = std::make_shared<cmp::Model>();
    model->Create(
        sceneInfo.resourceManager->GetMesh("Resources/models/Crate/Crate.obj"),
        sceneInfo.resourceManager->GetMaterial("Resources/models/wall/wall.mtl")
    );
    model->SetTintColor(0.3, 0.3, 0.3);
    frameGO->AddComponent(model);
    frameGO->AddComponent(sceneInfo.shader_l);
    frameGO->AddComponent(std::make_shared<cmp::FrustumCulling>());
    frameGO->GetComponent<cmp::FrustumCulling>()->Create(sceneInfo.resourceManager->GetMesh("Resources/models/Crate/Crate.obj"));

    levelNode->AddChild(alarmGO)->AddChild(frameGO);
    //--

    


    //cutscene 2

    go = std::make_shared<GameObject>();
    go->AddComponent(std::make_shared<cmp::Name>("cutsceneFinal"));

    go->AddComponent(std::make_shared<cmp::Transform>());
    go->GetComponent<cmp::Transform>()->SetPosition(-83.0f, 5.0f, -10.0f);

    go->AddComponent(std::make_shared<BoxCollider>(true, true));
    go->GetComponent<cmp::BoxCol>()->SetLengths({ 10.0, 10.0, 5.0 });
    go->GetComponent<cmp::BoxCol>()->layer = CollisionLayer::Ignore;
    go->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collidersManager);

    go->AddComponent(std::make_shared<cmp::Scriptable>());

    auto cutscenef = new CutsceneFinal();
    cutscenef->objectiveDoor = cutsceneDoorActivator2;
    cutscenef->doorsToShut = openDoorAfterEnemyDies;
    cutscenef->doorsToOpen = openElevator;
    cutscenef->spawner = world->FindNode("spawner0")->GetGameObject()->GetComponent<cmp::Scriptable>()->Get<EnemySpawnerScript>();
    cutscenef->prevScene = cutscene;
    go->GetComponent<cmp::Scriptable>()->Add(cutscenef);


    levelNode->AddChild(go);

    world->FindNode("spawner0")->GetGameObject()->GetComponent<cmp::Scriptable>()->Get<EnemySpawnerScript>()->spawnTimer = 9999.9f;
    // load next level

    go = std::make_shared<GameObject>(); 

    go->AddComponent(std::make_shared<cmp::Transform>());
    go->GetComponent<cmp::Transform>()->SetPosition(-83.00, 3.0, -32.5);

    go->AddComponent(std::make_shared<BoxCollider>(true, true));
    go->GetComponent<cmp::BoxCol>()->SetLengths({ 10.0, 10.0, 5.0 });
    go->GetComponent<cmp::BoxCol>()->layer = CollisionLayer::Ignore;
    go->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collidersManager);

    go->AddComponent(std::make_shared<cmp::Scriptable>());

    auto switchTrigger = new SwitchLevelTrigger();
    switchTrigger->newLevelIndex = 1;
    go->GetComponent<cmp::Scriptable>()->Add(switchTrigger);

    levelNode->AddChild(go);
}

void Scene::LoadLevel1(const SceneInfo& sceneInfo)
{
    auto levelNode = std::make_shared<SceneNode>(std::make_shared<GameObject>());
    levelNode->GetGameObject()->AddComponent(std::make_shared<cmp::Name>("LEVEL_1"));
    world->FindNode("LEVELS")->AddChild(levelNode);
    MapLoader::Load("Resources/maps/world_level1.map", levelNode.get(),
        sceneInfo.shader_l,
        sceneInfo.shader_d,
        sceneInfo.lineShader,
        sceneInfo.displShader,
        sceneInfo.particlesShader,
        sceneInfo.shadowParticlesShader,
        sceneInfo.collidersManager,
        sceneInfo.shadowsManager,
        sceneInfo.scene,
        sceneInfo.multiToolScript,
        sceneInfo.cameraGO);

    sceneInfo.cameraGO->GetComponent<cmp::Camera>()->RestartMovement(-8, 4.5, 10);

    //restart picked blueprints
    sceneInfo.multiToolScript->Unlock(0);
    sceneInfo.multiToolScript->Unlock(1);
    sceneInfo.cameraGO->GetComponent<cmp::Scriptable>()->Get<PlayerPlaceTurret>()->unlocked[0] = true;
    sceneInfo.cameraGO->GetComponent<cmp::Scriptable>()->Get<PlayerPlaceTurret>()->unlocked[1] = true;

    //computer
    
    {
        auto gameObject = std::make_shared<GameObject>();
        gameObject->AddComponent(std::make_shared<cmp::Name>("Computer0"));

        gameObject->AddComponent(std::make_shared<cmp::Transform>());
        gameObject->GetComponent<cmp::Transform>()->SetPosition(26.0, 8.0, 45.0);

        gameObject->AddComponent(std::make_shared<BoxCollider>(true, true));
        gameObject->GetComponent<cmp::BoxCol>()->SetLengths({ 3.1, 3.1, 3.1 });
        gameObject->GetComponent<cmp::BoxCol>()->SetOffset({ 0.0, 1.0, 0.0 });
        gameObject->GetComponent<cmp::BoxCol>()->AddToCollidersManager(sceneInfo.collidersManager);

        auto resourceScript = new Blueprint();
        resourceScript->type = PlayerPlaceTurret::TurretType::None;
        gameObject->AddComponent(std::make_shared<cmp::Scriptable>());
        gameObject->GetComponent<cmp::Scriptable>()->Add(resourceScript);

        levelNode->AddChild(gameObject);

        auto kabel = std::make_shared<GameObject>();
        kabel->AddComponent(std::make_shared<cmp::Transform>());
        kabel->GetComponent<cmp::Transform>()->SetPosition(0, 0.0, 2);
        kabel->GetComponent<cmp::Transform>()->SetRotation(0, 180,0);
        auto model = std::make_shared<cmp::Model>();
        model->Create(
            sceneInfo.resourceManager->GetMesh("Resources/models/kabel.obj"),
            sceneInfo.resourceManager->GetMaterial("Resources/models/multitool/icon.mtl")
        );
        model->SetTintColor(0.2, 0.2, 0.2);
        kabel->AddComponent(model);
        kabel->AddComponent(sceneInfo.shader_l);
        levelNode->AddChild(kabel);
    }


    //doors and activators
    struct DoorAndActivatorPair
    {
        glm::vec3 doorPosition;
        float doorRotationY;
        bool openOnStart;
        glm::vec3 activatorPosition;
        float activatorRotation;
    };

    std::vector<DoorAndActivatorPair> doorsAndButtons = {  //0 rot = Z aligned; 90 rot = X aligned
        { { -16.0,  3.00,  24.5 }, 90.0f, false,  { -20.5, 2.50, 20.9 }, 180.0f }, ///X start 
        { { -56.5,  3.00,  11.0 }, 0.00f,  true,  { -52.9, 2.50, 6.50 }, 90.00f }, /// Prawy dol
        { { 35.50,  3.00,  4.00 }, 0.00f,  true,  { 31.00, 2.50, 0.50 }, -90.0f }, /// Lewy dol
        { { -69.0,  9.00,  72.5 }, 90.0f,  true,  { -73.5, 10.0, 68.9 }, 180.0f }, ///X Prawa gora
        { { 57.50,  9.00,  33.0 }, 0.00f,  true,  { 53.90, 10.0, 37.5 }, -90.0f }, /// Lewa gora
        { { -16.0,  3.00,  71.5 }, 90.0f, false,  { -20.5, 3.00, 68.5 }, 180.0f },
    };

    int i = 0;
    
    for (auto& [doorPosition, doorRotation, openOnStart, activatorPosition, activatorRotation] : doorsAndButtons)
    {
        glm::vec3 doorOffset = { 0.0f, 6.1f, 0.0f };

        //create door
        auto go = std::make_shared<GameObject>();

        auto doorTransform = std::make_shared<cmp::Transform>();
        doorTransform->SetPosition(doorPosition + (openOnStart ? doorOffset : glm::vec3(0)));
        doorTransform->SetRotation(0, doorRotation, 0);
        float scl = 1.0f;
        doorTransform->SetScale(scl);
        go->AddComponent(doorTransform);

        go->AddComponent(std::make_shared<BoxCollider>(false, true));
        if (doorRotation < 0.001f) go->GetComponent<cmp::BoxCol>()->SetLengths({ 2.0 * scl, 7.0 * scl, 8.0 * scl });
        else go->GetComponent<cmp::BoxCol>()->SetLengths({ 8.0 * scl, 7.0 * scl, 2.0 * scl });
        go->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collidersManager);

        auto model = std::make_shared<cmp::Model>();
        model->Create(
            sceneInfo.resourceManager->GetMesh("Resources/models/doors/drzwi.obj"),
            sceneInfo.resourceManager->GetMaterial("Resources/models/doors/drzwi.mtl")
        );
        // model->SetTintColor(1.0, 0.5, 0.0);
        go->AddComponent(model);
        go->AddComponent(sceneInfo.shader_l);

        levelNode->AddChild(go);
        
        //prowadnica
        {
            auto go = std::make_shared<GameObject>();

            auto doorTransform = std::make_shared<cmp::Transform>();
            doorTransform->SetPosition(doorPosition);
            doorTransform->SetRotation(0, doorRotation, 0);
            float scl = 1.0f;
            doorTransform->SetScale(scl);
            go->AddComponent(doorTransform);

            auto model = std::make_shared<cmp::Model>();
            model->Create(
                sceneInfo.resourceManager->GetMesh("Resources/models/doors/prowadnica.obj"),
                sceneInfo.resourceManager->GetMaterial("Resources/models/doors/drzwi.mtl")
            );
            go->AddComponent(model);
            go->AddComponent(sceneInfo.shader_l);

            levelNode->AddChild(go);
        }

        //create activator
        go = std::make_shared<GameObject>();
        go->AddComponent(std::make_shared<cmp::Name>("DoorActivator" + std::to_string(i)));

        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(activatorPosition);
        go->GetComponent<cmp::Transform>()->SetRotation(0.0, activatorRotation, 0.0f);

        go->AddComponent(std::make_shared<SphereCollider>(false, true));
        go->GetComponent<cmp::SphereCol>()->SetRadius(0.18);
        go->GetComponent<cmp::SphereCol>()->AddToCollidersManager(collidersManager);

        model = std::make_shared<cmp::Model>();
        model->Create(
            sceneInfo.resourceManager->GetMesh("Resources/models/ny/przelacznikLaserowy/przelacznikLaserowy/przelacznikDetector.obj"),
            sceneInfo.resourceManager->GetMaterial("Resources/models/ny/przelacznikLaserowy/przelacznikLaserowy/przelacznikLaserowy.mtl")
        );
        model->SetTintColor(0.88, 0.21, 0.21);
        go->AddComponent(model);
        go->AddComponent(sceneInfo.shader_d);

        go->AddComponent(std::make_shared<cmp::FrustumCulling>());
        go->GetComponent<cmp::FrustumCulling>()->Create(sceneInfo.resourceManager->GetMesh("Resources/models/ny/przelacznikLaserowy/przelacznikLaserowy/przelacznikDetector.obj"));


        go->AddComponent(std::make_shared<cmp::Scriptable>());

        auto activator = new DoorActivator();
        activator->doorTransform = doorTransform;
        activator->openedOffset = doorOffset * (openOnStart ? -1.0f : 1.0f);
        go->GetComponent<cmp::Scriptable>()->Add(activator);

        if (i == 1) activator->spawner = world->FindNode("spawner1")->GetGameObject()->GetComponent<cmp::Scriptable>()->Get<EnemySpawnerScript>();
        if (i == 3) activator->spawner = world->FindNode("spawner0")->GetGameObject()->GetComponent<cmp::Scriptable>()->Get<EnemySpawnerScript>();
        if (i == 4) activator->spawner = world->FindNode("spawner2")->GetGameObject()->GetComponent<cmp::Scriptable>()->Get<EnemySpawnerScript>();
        if (i == 2) activator->spawner = world->FindNode("spawner3")->GetGameObject()->GetComponent<cmp::Scriptable>()->Get<EnemySpawnerScript>();

        i++;

        //- frame model
        auto frameGO = std::make_shared<GameObject>();
        frameGO->AddComponent(std::make_shared<cmp::Transform>());
        model = std::make_shared<cmp::Model>();
        model->Create(
            sceneInfo.resourceManager->GetMesh("Resources/models/ny/przelacznikLaserowy/przelacznikLaserowy/przelacznikFrame.obj"),
            sceneInfo.resourceManager->GetMaterial("Resources/models/ny/przelacznikLaserowy/przelacznikLaserowy/przelacznikLaserowy.mtl")
        );
        frameGO->AddComponent(model);
        frameGO->AddComponent(sceneInfo.shader_l);
        frameGO->AddComponent(std::make_shared<cmp::FrustumCulling>());
        frameGO->GetComponent<cmp::FrustumCulling>()->Create(sceneInfo.resourceManager->GetMesh("Resources/models/ny/przelacznikLaserowy/przelacznikLaserowy/przelacznikFrame.obj"));

        levelNode->AddChild(go)->AddChild(frameGO);
    }
}

void Scene::LoadLevelPuzzle1(const SceneInfo& sceneInfo)
{
    auto levelNode = std::make_shared<SceneNode>(std::make_shared<GameObject>());
    levelNode->GetGameObject()->AddComponent(std::make_shared<cmp::Name>("LEVEL_PUZZLE1"));
    world->FindNode("LEVELS")->AddChild(levelNode);
    MapLoader::Load("Resources/maps/world_puzzle1.map", levelNode.get(),
        sceneInfo.shader_l,
        sceneInfo.shader_d,
        sceneInfo.lineShader,
        sceneInfo.displShader,
        sceneInfo.particlesShader,
        sceneInfo.shadowParticlesShader,
        sceneInfo.collidersManager,
        sceneInfo.shadowsManager,
        sceneInfo.scene,
        sceneInfo.multiToolScript,
        sceneInfo.cameraGO);

    world->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Camera>()->RestartMovement(-4, 4.5, 10);

    //restart picked blueprints
    sceneInfo.multiToolScript->Unlock(0);
    sceneInfo.multiToolScript->Unlock(1);
    sceneInfo.cameraGO->GetComponent<cmp::Scriptable>()->Get<PlayerPlaceTurret>()->unlocked[0] = true;
    sceneInfo.cameraGO->GetComponent<cmp::Scriptable>()->Get<PlayerPlaceTurret>()->unlocked[1] = true;
    {
        auto kabel = std::make_shared<GameObject>();
        kabel->AddComponent(std::make_shared<cmp::Transform>());
        kabel->GetComponent<cmp::Transform>()->SetPosition(0.0, 0.0, 0.0);
        auto model = std::make_shared<cmp::Model>();
        model->Create(
            sceneInfo.resourceManager->GetMesh("Resources/models/kabel.obj"),
            sceneInfo.resourceManager->GetMaterial("Resources/models/multitool/icon.mtl")
        );
        model->SetTintColor(0.2, 0.2, 0.2);
        kabel->AddComponent(model);
        kabel->AddComponent(sceneInfo.shader_l);
        levelNode->AddChild(kabel);
    }
    struct DoorAndActivatorPair
    {
        glm::vec3 doorPosition;
        float doorRotationY;
        glm::vec3 activatorPosition;
        float activatorRotation;
    };

    std::vector<DoorAndActivatorPair> doorsAndButtons = {  //0 rot = Z aligned; 90 rot = X aligned
        { { -15.00,  1.0,  25.0 }, 90.0f,   { -20.0,  2.5,  17.90 }, 180.0f },
    };

    int i = 0;
    for (auto& [doorPosition, doorRotation, activatorPosition, activatorRotation] : doorsAndButtons)
    {
        //create door
        auto go = std::make_shared<GameObject>();

        auto doorTransform = std::make_shared<cmp::Transform>();
        doorTransform->SetPosition(doorPosition);
        doorTransform->SetRotation(0, doorRotation, 0);
        float scl = 2.0f;
        doorTransform->SetScale(scl);
        go->AddComponent(doorTransform);

        go->AddComponent(std::make_shared<BoxCollider>(false, true));
        if (doorRotation < 0.001f) go->GetComponent<cmp::BoxCol>()->SetLengths({ 1.0 * scl, 5.0 * scl, 5.0 * scl });
        else go->GetComponent<cmp::BoxCol>()->SetLengths({ 5.0 * scl, 5.0 * scl, 1.0 * scl });
        go->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collidersManager);

        auto model = std::make_shared<cmp::Model>();
        model->Create(
            sceneInfo.resourceManager->GetMesh("Resources/models/ny/drzwi1/drzwi1/drzwi1.obj"),
            sceneInfo.resourceManager->GetMaterial("Resources/models/ny/drzwi1/drzwi1/drzwi1.mtl")
        );

        go->AddComponent(model);
        go->AddComponent(sceneInfo.shader_l);

        levelNode->AddChild(go);

        //create activator
        go = std::make_shared<GameObject>();
        go->AddComponent(std::make_shared<cmp::Name>("DoorActivator" + std::to_string(i)));

        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(activatorPosition);
        go->GetComponent<cmp::Transform>()->SetRotation(0.0, activatorRotation, 0.0f);

        go->AddComponent(std::make_shared<SphereCollider>(false, true));
        go->GetComponent<cmp::SphereCol>()->SetRadius(0.18);
        go->GetComponent<cmp::SphereCol>()->AddToCollidersManager(collidersManager);

        model = std::make_shared<cmp::Model>();
        model->Create(
            sceneInfo.resourceManager->GetMesh("Resources/models/ny/przelacznikLaserowy/przelacznikLaserowy/przelacznikDetector.obj"),
            sceneInfo.resourceManager->GetMaterial("Resources/models/ny/przelacznikLaserowy/przelacznikLaserowy/przelacznikLaserowy.mtl")
        );
        model->SetTintColor(0.88, 0.21, 0.21);
        go->AddComponent(model);
        go->AddComponent(sceneInfo.shader_d);

        go->AddComponent(std::make_shared<cmp::FrustumCulling>());
        go->GetComponent<cmp::FrustumCulling>()->Create(sceneInfo.resourceManager->GetMesh("Resources/models/ny/przelacznikLaserowy/przelacznikLaserowy/przelacznikDetector.obj"));


        go->AddComponent(std::make_shared<cmp::Scriptable>());

        auto activator = new DoorActivator();
        activator->doorTransform = doorTransform;
        activator->openedOffset = { 0.0f, 10.1f, 0.0f };
        go->GetComponent<cmp::Scriptable>()->Add(activator);

        i++;

        //- frame model
        auto frameGO = std::make_shared<GameObject>();
        frameGO->AddComponent(std::make_shared<cmp::Transform>());
        model = std::make_shared<cmp::Model>();
        model->Create(
            sceneInfo.resourceManager->GetMesh("Resources/models/ny/przelacznikLaserowy/przelacznikLaserowy/przelacznikFrame.obj"),
            sceneInfo.resourceManager->GetMaterial("Resources/models/ny/przelacznikLaserowy/przelacznikLaserowy/przelacznikLaserowy.mtl")
        );
        frameGO->AddComponent(model);
        frameGO->AddComponent(sceneInfo.shader_l);
        frameGO->AddComponent(std::make_shared<cmp::FrustumCulling>());
        frameGO->GetComponent<cmp::FrustumCulling>()->Create(sceneInfo.resourceManager->GetMesh("Resources/models/ny/przelacznikLaserowy/przelacznikLaserowy/przelacznikFrame.obj"));

        levelNode->AddChild(go)->AddChild(frameGO);
    }
}

Scene::~Scene()
{
    world = nullptr;

    delete shadowsManager;
    shadowsManager = nullptr;

    delete collidersManager;
    collidersManager = nullptr;
}

void Scene::SwitchLevel(int newLevelIndex)
{
    newLevelToSwitch = -1;
    //if (newLevelIndex == currentLevelIndex) return;

    auto levelsNode = world->FindNode("LEVELS");

    switch (currentLevelIndex)
    {
    case 0:
        levelsNode->RemoveNode(levelsNode->FindNode("LEVEL_TUTORIAL"));
        break;
    case 1:
        levelsNode->RemoveNode(levelsNode->FindNode("LEVEL_1"));
        break;
    case 2:
        levelsNode->RemoveNode(levelsNode->FindNode("LEVEL_PUZZLE1"));
        break;
    default:
        printf("UNKNOWN LEVEL\n");
        break;
    }

    levelsNode->DeleteNodes();

    currentLevelIndex = newLevelIndex;
    
    switch (currentLevelIndex)
    {
    case 0:
        LoadLevelTutorial(SCENE_INFO);
        break;
    case 1:
        LoadLevel1(SCENE_INFO);
        break;
    case 2:
        LoadLevelPuzzle1(SCENE_INFO);
        break;
    default:
        printf("UNKNOWN LEVEL\n");
        break;
    }

    
    levelsNode->LoadScripts();

    auto placeTurretScript = world->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Scriptable>()->Get<PlayerPlaceTurret>();
    auto turretsHolder = placeTurretScript->turretsHolder;
    auto turretsHolderParent = world->FindNode("TURRETS_HOLDER");
    turretsHolderParent->RemoveNode(turretsHolder);
    turretsHolderParent->DeleteNodes();

    auto turretsHolderGO = std::make_shared<GameObject>();
    turretsHolderGO->AddComponent(std::make_shared<cmp::Transform>());
    auto turretsHolderNode = turretsHolderParent->AddChild(turretsHolderGO);
    placeTurretScript->turretsHolder = turretsHolderNode.get();
    placeTurretScript->PrepareNewTurrets();
    placeTurretScript->isPlacing = false;

    auto gameManager = world->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Scriptable>()->Get<GameManager>();
    gameManager->SetEnergy(0);

    auto playerHealth = world->FindNode("PlayerHealthGO")->GetGameObject()->GetComponent<cmp::Scriptable>()->Get<PlayerHealthScript>();
    playerHealth->Start();

    auto playerInteract = world->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Scriptable>()->Get<PlayerInteract>();
    playerInteract->selectedMirror = nullptr;
    world->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Camera>()->SetMovementEnable(true);
    world->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Camera>()->SetRotationEnable(true);
}

void Scene::SafeSwitchLevel(int newLevelIndex)
{
    if (newLevelIndex < 0) newLevelToSwitch = currentLevelIndex;
    else newLevelToSwitch = newLevelIndex;
}

void Scene::Update(float dt)
{
    GO_CROSSHAIR->GetComponent<cmp::Transform>()->SetPosition(GameApplication::GetAspectRatio() * 0.5f, 0.5f, 0.1f);
    GO_TOOLTIP->GetComponent<cmp::Transform>()->SetPosition(GameApplication::GetAspectRatio() * 0.5f + 0.001f, 0.453f, 0.1f);

    if (newLevelToSwitch >= 0)
    {
        SwitchLevel(newLevelToSwitch);
    }

    //if (Input()->Keyboard()->OnPressed(KeyboardKey::V))
    //{
    //    SwitchLevel((currentLevelIndex + 1) % 3);
    //}
    //
    if (Input()->Keyboard()->OnPressed(KeyboardKey::F9))
    {
        if (currentLevelIndex != 0)
            SwitchLevel((currentLevelIndex));
    }


    //Update camera
    std::shared_ptr<GameObject> goCamera = world->FindNode("CAMERA")->GetGameObject();
    std::shared_ptr<TransformComponent> transformCamera = goCamera->GetComponent<cmp::Transform>();
    std::shared_ptr<cmp::Camera> camera = goCamera->GetComponent<CameraComponent>();

    camera->Update(GameApplication::GetInputManager(), dt);
    transformCamera->SetPosition(camera->GetPosition());

    if (camera->GetPosition().y < -5.0f)
        camera->RestartMovement(-4, 4.5, 10);

    if (isPaused)
    {
        isPaused = false;
        camera->SetRotationEnable(true);
    }

    //Detect collision
    goCamera->GetNode()->ResetGlobalTransformations();
    collidersManager->CheckEverything();


    //Prevent camera jiggle and set correct position
    camera->SetPosition(transformCamera->GetPosition());
    transform = GameApplication::GetProjection() * camera->GetView();

    //SOUND
    //AudioManager::Update(dt);

    //Position multitool

    auto mtTransform = GO_MULTITOOL->GetComponent<cmp::Transform>();
    auto m = glm::inverse(camera->GetView());
    
   //if (Input()->Keyboard()->IsPressed(KeyboardKey::Up))    mtTransform->Move(0,  dt, 0);
   //if (Input()->Keyboard()->IsPressed(KeyboardKey::Down))  mtTransform->Move(0, -dt, 0);
   //if (Input()->Keyboard()->IsPressed(KeyboardKey::Right)) mtTransform->Rotate(dt, 0, 0);
   //if (Input()->Keyboard()->IsPressed(KeyboardKey::Left))  mtTransform->Rotate(-dt, 0, 0);
   //if (Input()->Keyboard()->IsPressed(KeyboardKey::LBracket)) mtTransform->Move(-dt, 0, 0);
   //if (Input()->Keyboard()->IsPressed(KeyboardKey::RBracket)) mtTransform->Move(dt, 0, 0);
   //if (Input()->Keyboard()->IsPressed(KeyboardKey::O)) mtTransform->Move(0, 0, dt);
   //if (Input()->Keyboard()->IsPressed(KeyboardKey::P)) mtTransform->Move(0, 0, -dt);
   //printf("p %f \n", mtTransform->GetPosition().z);
   //
   //printf("p %f %f \tr %f\n", mtTransform->GetPosition().x, mtTransform->GetPosition().y, mtTransform->GetRotation().x);
    /*auto displ = world->FindNode("mtDisplay")->GetGameObject()->GetComponent<cmp::Transform>();
    if (Input()->Keyboard()->IsPressed(KeyboardKey::LBracket)) displ->Move(0, 0, -dt*0.1);
    if (Input()->Keyboard()->IsPressed(KeyboardKey::RBracket))  displ->Move(0, 0,  dt * 0.1);
    if (Input()->Keyboard()->IsPressed(KeyboardKey::O)) displ->Move(0,  -dt * 0.1,0);
    if (Input()->Keyboard()->IsPressed(KeyboardKey::P))  displ->Move(0,dt * 0.1,0);
    if (Input()->Keyboard()->IsPressed(KeyboardKey::U)) displ->Move( -dt * 0.1,0, 0);
    if (Input()->Keyboard()->IsPressed(KeyboardKey::I))  displ->Move( dt * 0.1,0, 0);

    printf("p %f \tr %f \t|\t x %f y %f z %f\n", mtTransform->GetPosition().y, mtTransform->GetRotation().x,
        displ->GetPosition().x, displ->GetPosition().y, displ->GetPosition().z);*/


    glm::vec4 mtNewPosition = m * glm::vec4(mtTransform->GetPosition(), 1.0f);

    glm::vec3 swing = glm::vec3(0.0f);
    if (camera->IsMoving() && camera->GetIsGrounded())
    {
        float swingAmountX = 0.0014f;
        float swingAmountY = 0.0009f;
        float swingSpeed = camera->GetSpeed() * 0.5f;
        swing = (camera->GetRight() * swingAmountX + camera->GetUp() * swingAmountY) * (float)sin(GameApplication::GetTotalElapsedTime() * swingSpeed);
    }

    if (camera->GetShakeTime() > 0.0f)
    {
        swing += camera->GetRight() * 0.07f * camera->GetShakeAmount() * (float)cos(GameApplication::GetTotalElapsedTime() * camera->GetShakeSpeed());
    }

    m[3][0] = mtNewPosition.x + swing.x;
    m[3][1] = mtNewPosition.y + swing.y;
    m[3][2] = mtNewPosition.z + swing.z;
    m = glm::rotate(m, mtTransform->GetRotation().x, glm::vec3(1, 0, 0));
    mtTransform->SetModelMatrix(m);


    auto flashLight = GO_FLASHLIGHT->GetComponent<cmp::SpotLight>();
    flashLight->SetPosition({ mtNewPosition.x, mtNewPosition.y, mtNewPosition.z });
    flashLight->SetDirection(camera->GetForward());


    // Handle game pausing
    auto playerInteractScript = goCamera->GetComponent<cmp::Scriptable>()->Get<PlayerInteract>();
    if (!playerInteractScript->selectedMirror && !playerInteractScript->placeTurretScript->isPlacing)
    {
        if (Input()->Keyboard()->OnPressed(KeyboardKey::Escape_KB)
        || (isPaused && (Input()->Mouse()->OnPressed(MouseButton::Left_MB) 
                     ||  Input()->Mouse()->OnPressed(MouseButton::Right_MB)
                     ||  Input()->Keyboard()->OnPressed(KeyboardKey::E))))
        {
            isPaused = true;
            camera->SetRotationEnable(false);
            // camera->SetMovementEnable(!isPaused);
            
            GameApplication::inGame = false;

            //if (isPaused) glfwSetInputMode(GameApplication::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            //else glfwSetInputMode(GameApplication::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }

    //delete me
    if (Input()->Keyboard()->OnPressed(KeyboardKey::Nr0))
    {
        camera->ShakeCamera(2.0, 0.15f, 55.0f, 0.95f);
    }

    if (Input()->Keyboard()->OnPressed(KeyboardKey::K))
    {
        AudioManager::PauseQueue();
    }

    if (Input()->Keyboard()->OnPressed(KeyboardKey::L))
    {
        AudioManager::ResumeQueue();
    }

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

    glClear(GL_DEPTH_BUFFER_BIT);
    GO_MULTITOOL->GetNode()->Render(transform);
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
