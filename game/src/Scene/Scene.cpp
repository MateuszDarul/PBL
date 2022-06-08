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

#include "Scripts/Cutscenexd.h"
#include "Scripts/LightActivator.h"
#include "Scripts/LanternRange.h"



// for quick access (avoiding string comparisions in FindNode)
std::shared_ptr<GameObject> GO_MULTITOOL;
std::shared_ptr<GameObject> GO_FLASHLIGHT;
std::shared_ptr<GameObject> GO_CROSSHAIR;

std::shared_ptr<SceneNode> NODE_MAIN;
std::shared_ptr<SceneNode> NODE_GUI;


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
    go->GetComponent<cmp::Camera>()->SetSpeed(5);
    collidersManager = new CollidersManager(go); //mened�er kolider�w
    collidersManager->SetDistanceFromPlayer(10.0f);
    go->AddComponent(std::make_shared<BoxCollider>(false, false));
    go->GetComponent<cmp::BoxCol>()->setLengths(glm::vec3(1.5,3,1.5));
    go->GetComponent<cmp::BoxCol>()->SetOffset(glm::vec3(0,-1,0));
    go->GetComponent<cmp::BoxCol>()->layer = CollisionLayer::Player;
    go->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collidersManager);
    go->AddComponent(std::make_shared<SphereCollider>(false, false));
    go->GetComponent<cmp::SphereCol>()->SetRadius(0.75);
    go->GetComponent<cmp::SphereCol>()->SetOffset(glm::vec3(0,-2.5,0));
    go->GetComponent<cmp::SphereCol>()->layer = CollisionLayer::Player;
    go->GetComponent<cmp::SphereCol>()->AddToCollidersManager(collidersManager);
    world->FindNode("MAIN")->AddChild(go);

    MapLoader::Load("Resources/maps/world_nolights.map", world->FindNode("MAIN"), shader_l, collidersManager, shadowsManager);

    ///***

    auto playerGO = go;

    //ground check
    {
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
    world->FindNode("MAIN")->FindNode("CAMERA")->AddChild(debugLineGO);
    
    auto playerPlace = new PlayerPlaceTurret();
 
    playerPlace->gameManager = gm;
    playerPlace->line = debugLineCmp;
    playerPlace->colMan = collidersManager;
    playerPlace->resMan = resMan;
    playerPlace->turretShader = shader_l;
    playerPlace->lineShader = lineShader;

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
    
    MultiToolController* multiToolScript;
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

        world->FindNode("MAIN")->AddChild(multiTool);
        auto multiToolNode = world->FindNode("MAIN")->FindNode("MultiTool");


        multiTool->AddComponent(std::make_shared<cmp::Scriptable>());

        multiToolScript = new MultiToolController();
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

        //multitool light
        auto flashLightGO = std::make_shared<GameObject>();
        flashLightGO->AddComponent(std::make_shared<cmp::SpotLight>());
        auto lightCmp = flashLightGO->GetComponent<cmp::SpotLight>();
        lightCmp->Create();
        lightCmp->AddShader(shader_l);
        lightCmp->SetPosition(multiTool->GetComponent<cmp::Transform>()->GetPosition());
        lightCmp->SetLightColor({1.0f, 0.5f, 0.9f});
        lightCmp->SetCutOff({38.5f, 40.0f});
        flashLightGO->AddComponent(std::make_shared<cmp::Name>("Flashlight"));
        world->FindNode("MAIN")->AddChild(flashLightGO);

        multiToolScript->flashlight = lightCmp;

        GO_MULTITOOL = multiTool;
        GO_FLASHLIGHT = flashLightGO;
    }

    //slope test
    {
        go = std::make_shared<GameObject>();
        
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(-35, 4.1-1.0, 44.5+3.0);
        go->GetComponent<cmp::Transform>()->SetRotation(90.0, 0.0, 90.0-36.87);
        go->GetComponent<cmp::Transform>()->SetScale(1.5);

        go->AddComponent(std::make_shared<SlopeCollider>(false, true));
        go->GetComponent<SlopeCollider>()->SetDimensions(64.0f/6.0f, 8.2f, 4.0f);
        go->GetComponent<SlopeCollider>()->SetOffset({0.0f, 1.0f, -3.0f});        
        go->GetComponent<SlopeCollider>()->SetDirection(SlopeCollider::Direction::X_NEG);
        go->GetComponent<SlopeCollider>()->AddToCollidersManager(collidersManager);


        mc = std::make_shared<ModelComponent>();
        mc->Create(
            resMan->GetMesh("Resources/models/Exported/Sciana.NR1.obj"),
            resMan->GetMaterial("Resources/models/Crate/Crate.mtl")
        );
        go->AddComponent(shader_l);
        go->AddComponent(mc);

        world->FindNode("MAIN")->AddChild(go);
    }
  


    //===enemy
    {      
        go = std::make_shared<GameObject>();
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
        go->GetComponent<cmp::Transform>()->SetPosition(-100, 2, 25);

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
        enemyScript->health = health;
        enemyScript->multitool = multiToolScript;
        std::shared_ptr<Path> e_path = std::make_shared<Path>();
        enemyScript->SetPath(e_path);
        go->GetComponent<cmp::Scriptable>()->Add(enemyScript);
    }
    world->FindNode("MAIN")->AddChild(go);


        
    //=== pickupable resource
    std::vector<glm::vec3> resourcePositions = {
        { -29.0f, 0.30f,  12.0f },
        { -86.8f, 0.30f,  67.6f },
        { -78.0f, 0.30f,  91.5f },
        { -78.7f, 0.25f,  92.8f },
    };

    for (int i = 0; i < resourcePositions.size(); i++)
    {
        go = std::make_shared<GameObject>();
        go->AddComponent(std::make_shared<cmp::Name>("Resource " + std::to_string(i)));
        
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(resourcePositions[i]);
        go->GetComponent<cmp::Transform>()->SetScale(0.4);

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

    
        world->FindNode("MAIN")->AddChild(go);
    }

    

    //blueprints
    struct BlueprintPosition {
        glm::vec3 position;
        PlayerPlaceTurret::TurretType type;
    };
    std::vector<BlueprintPosition> blueprints =  {
        {{ -73.0f, 0.5f, 89.5f },  PlayerPlaceTurret::TurretType::Lantern  },    //???
        {{ -83.0f, 0.5f, 69.5f },  PlayerPlaceTurret::TurretType::Shooting },    //Shooting
        {{ -26.0f, 0.5f,  5.0f },  PlayerPlaceTurret::TurretType::Laser    }     //Laser
    };
    for (int i = 0; i < 3; i++)
    {
        go = std::make_shared<GameObject>();
        go->AddComponent(std::make_shared<cmp::Name>("Blueprint " + std::to_string(i)));
        
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(blueprints[i].position);
        go->GetComponent<cmp::Transform>()->SetScale(0.5);

        go->AddComponent(std::make_shared<BoxCollider>(true, true));
        go->GetComponent<cmp::BoxCol>()->setLengths({1.1, 1.1, 1.1});
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
        resourceScript->type = blueprints[i].type;
        go->AddComponent(std::make_shared<cmp::Scriptable>());
        go->GetComponent<cmp::Scriptable>()->Add(resourceScript);

    
        world->FindNode("MAIN")->AddChild(go);
    }

    //mirrors

    //- room 2
    {
        go = std::make_shared<GameObject>();
        go->AddComponent(std::make_shared<cmp::Name>("Mirror"));

        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(-9.5, 2.5, 35.5);
        go->GetComponent<cmp::Transform>()->SetRotation(0.0, 90.0, 0.0);

        go->AddComponent(std::make_shared<BoxCollider>(false, true, CollisionLayer::Mirror));
        go->GetComponent<cmp::BoxCol>()->setLengths({2.0, 2.0, 2.0});
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
        mirrorScript->initialRotationOffsetY = 90.0f;
        mirrorScript->maxRotationX = 10.0f;
        mirrorScript->maxRotationY = 15.0f;
        go->GetComponent<cmp::Scriptable>()->Add(mirrorScript);

        world->FindNode("MAIN")->AddChild(go);
    }

    //- room 3
    {
        go = std::make_shared<GameObject>();
        go->AddComponent(std::make_shared<cmp::Name>("Mirror"));

        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(-20.0, 2.5, 64.5);
        go->GetComponent<cmp::Transform>()->SetRotation(-11.0, 0.0, 0.0);

        go->AddComponent(std::make_shared<BoxCollider>(false, true, CollisionLayer::Mirror));
        go->GetComponent<cmp::BoxCol>()->setLengths({2.0, 2.0, 2.0});
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
        mirrorScript->initialRotationOffsetY = 30.0f;
        go->GetComponent<cmp::Scriptable>()->Add(mirrorScript);

        world->FindNode("MAIN")->AddChild(go);
    }
    {
        go = std::make_shared<GameObject>();
        go->AddComponent(std::make_shared<cmp::Name>("Mirror"));

        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(-37.5, 4.0, 55.5);
        go->GetComponent<cmp::Transform>()->SetRotation(0.0, 180.0, 0.0);

        go->AddComponent(std::make_shared<BoxCollider>(false, true, CollisionLayer::Mirror));
        go->GetComponent<cmp::BoxCol>()->setLengths({2.0, 2.0, 2.0});
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
        mirrorScript->initialRotationOffsetY = 180.0f;
        go->GetComponent<cmp::Scriptable>()->Add(mirrorScript);

        world->FindNode("MAIN")->AddChild(go);
    }
    
    //doors and activators
    struct DoorAndActivatorPair
    {
        glm::vec3 doorPosition;
        float doorRotationY;
        glm::vec3 activatorPosition;
    };

    std::vector<DoorAndActivatorPair> doorsAndButtons = {  //0 rot = Z aligned; 90 rot = X aligned
        { { -15.00,  1.0,  25.0 }, 90.0f,   { -20.0,  2.5,  20.49 } },  //room 1
        { { -25.00,  1.0,  50.5 }, 90.0f,   { -20.0,  2.5,  47.49 } },  //room 2
        { { -64.75,  1.0,  60.0 },  0.0f,   { -61.0,  3.0,  64.60 } },  //room 3 - cutscene close (important id)
        { { -73.00,  1.0,  74.5 }, 90.0f,   { -61.0, -9.0,  64.60 } },  //room 4 - cutscene open  (important id)
    };

    int i = 0;
    DoorActivator* cutsceneDoorActivator = nullptr;
    DoorActivator* openDoorAfterEnemyDies = nullptr;
    for (auto& [doorPosition, doorRotation, activatorPosition] : doorsAndButtons)
    {
        //create door
        go = std::make_shared<GameObject>();

        auto doorTransform = std::make_shared<cmp::Transform>();
        doorTransform->SetPosition(doorPosition);
        doorTransform->SetRotation(0, doorRotation, 0);
        float scl = 2.0f;
        doorTransform->SetScale(scl);
        go->AddComponent(doorTransform);

        go->AddComponent(std::make_shared<BoxCollider>(false, true));
        if (doorRotation < 0.001f) go->GetComponent<cmp::BoxCol>()->setLengths({1.0 * scl, 5.0 * scl, 5.0 * scl});
        else go->GetComponent<cmp::BoxCol>()->setLengths({5.0*scl, 5.0*scl, 1.0 *scl});
        go->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collidersManager);

        auto model = std::make_shared<cmp::Model>();
        model->Create(
            resMan->GetMesh("Resources/models/wall/wall.obj"),
            resMan->GetMaterial("Resources/models/wall/wall.mtl")
        );
        model->SetTintColor(1.0, 0.5, 0.0);
        go->AddComponent(model);
        go->AddComponent(shader_d);

        world->FindNode("MAIN")->AddChild(go);

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
        model->SetTintColor(1.0, 1.0, 0.0);
        go->AddComponent(model);
        go->AddComponent(shader_d);

        go->AddComponent(std::make_shared<cmp::FrustumCulling>());
        go->GetComponent<cmp::FrustumCulling>()->Create(resMan->GetMesh("Resources/models/Crate/Crate.obj"));


        go->AddComponent(std::make_shared<cmp::Scriptable>());

        auto activator = new DoorActivator();
        activator->doorTransform = doorTransform;
        activator->openedOffset = { 0.0f, 10.1f, 0.0f };
        go->GetComponent<cmp::Scriptable>()->Add(activator);

        if (i == 2) cutsceneDoorActivator = activator;
        if (i == 3) openDoorAfterEnemyDies = activator;
        i++;

        world->FindNode("MAIN")->AddChild(go);
    }

    //predefined lasers
    {
        go = std::make_shared<GameObject>();
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->AddComponent(std::make_shared<cmp::Name>("debugturet"));
        go->GetComponent<cmp::Transform>()->SetPosition(-30.0f, 0.0f, 28.5f);
        go->GetComponent<cmp::Transform>()->SetRotation(0.0, -20.0, 0.0);


        auto scriptHolder = std::make_shared<cmp::Scriptable>();
        go->AddComponent(scriptHolder);

        auto turretScript = new TurretLaser();
        turretScript->colMan = collidersManager;
        
        auto line = std::make_shared<cmp::Line>();
        line->Create();
        line->thickness = 2.0f;
        line->color1 = { 1.0f, 1.0f, 0.0f };
        line->color2 = { 1.0f, 0.7f, 0.0f };

        go->AddComponent(line);
        go->AddComponent(lineShader);
        turretScript->line = line.get(); 

        scriptHolder->Add(turretScript);

        go->AddComponent(std::make_shared<cmp::BoxCol>(true, true, CollisionLayer::GUI));
        std::shared_ptr<cmp::BoxCol> col = go->GetComponent<cmp::BoxCol>();
        col->setLengths({ 2.0, 2.5, 2.0 });
        col->SetOffset({ 0.0, 1.75, 0.0 });
        col->AddToCollidersManager(collidersManager);

        auto gfxGO = std::make_shared<GameObject>();
        gfxGO->AddComponent(std::make_shared<cmp::Transform>());
        gfxGO->AddComponent(std::make_shared<cmp::Name>("gfx"));

        auto mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/Wieze/Laser.obj"),
            resMan->GetMaterial("Resources/models/Wieze/Laser.mtl")
        );
        gfxGO->AddComponent(mc);
        
        gfxGO->AddComponent(std::make_shared<cmp::FrustumCulling>());
        gfxGO->GetComponent<cmp::FrustumCulling>()->Create(
            resMan->GetMesh("Resources/models/Wieze/Laser.obj")
        );
        gfxGO->AddComponent(shader_d);


        world->FindNode("MAIN")->AddChild(go)->AddChild(gfxGO);
    }

    //generator
    struct GeneratorInfo
    {
        glm::vec3 position;
        bool isEnabled;
    };
    std::vector<GeneratorInfo> genPositions = {
        {{ -10.0f, 0.0f, 10.0f }, true  },
        {{ -20.0f, 0.0f, 29.0f }, false },
    };
    for (auto& [position, isEnabled] : genPositions)
    {
        float lightbulbOffset = 2.5f;
        float lightRange = 11.0f;

        auto bulbPos = glm::vec3(position.x, position.y + lightbulbOffset, position.z);

        go = std::make_shared<GameObject>();
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(bulbPos);
        go->AddComponent(std::make_shared<cmp::Name>("debugbulb"));


        auto bulbModel = std::make_shared<cmp::Model>();
        bulbModel->Create(
            resMan->GetMesh("Resources/models/Sphere/Sphere.obj"),
            resMan->GetMaterial("Resources/models/wall/wall.mtl")
        );
        go->AddComponent(bulbModel);
        go->AddComponent(shader_d);

        go->AddComponent(std::make_shared<SphereCollider>(false, true, CollisionLayer::GUI));
        go->GetComponent<SphereCollider>()->SetRadius(0.6f);
        go->GetComponent<SphereCollider>()->AddToCollidersManager(collidersManager);


        auto light = std::make_shared<cmp::PointLight>();
        auto lightGO = std::make_shared<GameObject>();
        lightGO->AddComponent(light);
        light->Create();
        light->AddShader(shader_l);
        light->SetPosition(bulbPos);
        light->SetDamping(lightRange);
        light->SetLightColor({ 0.8f, 0.8f, 1.0f });
        shadowsManager->AddLight(lightGO.get());

        if (!isEnabled)
        {
            bulbModel->SetTintColor(0.4f, 0.4f, 0.8f);
            light->SetPosition({ 999, 999, 999 });
        }

        go->AddComponent(std::make_shared<cmp::Scriptable>());

        LightActivator* activator = new LightActivator();
        activator->bulbModel = bulbModel.get();
        activator->lightComponent = light.get();
        activator->isAlwaysLit = isEnabled;
        go->GetComponent<cmp::Scriptable>()->Add(activator);


        auto rangeGO = std::make_shared<GameObject>();
        rangeGO->AddComponent(std::make_shared<cmp::Transform>());
        rangeGO->AddComponent(std::make_shared<cmp::Name>("debugrange"));

        rangeGO->AddComponent(std::make_shared<SphereCollider>(true, false, CollisionLayer::Ignore));
        rangeGO->GetComponent<SphereCollider>()->SetRadius(lightRange);
        rangeGO->GetComponent<SphereCollider>()->AddToCollidersManager(collidersManager);

        rangeGO->AddComponent(std::make_shared<cmp::Scriptable>());
        LanternRange* range = new LanternRange();
        range->colMan = collidersManager;
        range->isAlwaysLit = isEnabled; 
        range->ChangeLightPower(isEnabled);
        rangeGO->GetComponent<cmp::Scriptable>()->Add(range);

        activator->range = range;

        auto gfxGO = std::make_shared<GameObject>();
        gfxGO->AddComponent(std::make_shared<cmp::Transform>());
        gfxGO->GetComponent<cmp::Transform>()->SetPosition(0.0f, -lightbulbOffset, 0.0f);
        gfxGO->AddComponent(std::make_shared<cmp::Name>("gfx"));

        auto mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/Wieze/Latarnia.obj"),
            resMan->GetMaterial("Resources/models/Wieze/Latarnia.mtl")
        );
        gfxGO->AddComponent(mc);

        gfxGO->AddComponent(std::make_shared<cmp::FrustumCulling>());
        gfxGO->GetComponent<cmp::FrustumCulling>()->Create(
            resMan->GetMesh("Resources/models/Wieze/Latarnia.obj")
        );
        gfxGO->AddComponent(shader_l);

        
        auto node = world->FindNode("MAIN")->AddChild(go);
        node->AddChild(gfxGO);
        node->AddChild(rangeGO);
        world->FindNode("MAIN")->AddChild(lightGO);
    }


    //cutscene
    {
        go = std::make_shared<GameObject>();
        go->AddComponent(std::make_shared<cmp::Name>("cutscene"));
        
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(-83.0f, 2.5f, 69.0f );
        go->GetComponent<cmp::Transform>()->SetScale(2.5);

        go->AddComponent(std::make_shared<BoxCollider>(true, true));
        go->GetComponent<cmp::BoxCol>()->setLengths({5.0, 5.0, 5.0});
        go->GetComponent<cmp::BoxCol>()->layer = CollisionLayer::Ignore;
        go->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collidersManager);


        go->AddComponent(std::make_shared<cmp::Scriptable>());

        auto cutscene = new Cutscenexd();
        cutscene->doorsToShut = cutsceneDoorActivator;
        cutscene->doorsToOpen = openDoorAfterEnemyDies;
        cutscene->lightShader = shader_l;
        cutscene->shadowManager = shadowsManager;
        cutscene->enemyHealth = world->FindNode("Enemy")->GetGameObject()->GetComponent<cmp::Scriptable>()->Get<Health>();
        go->GetComponent<cmp::Scriptable>()->Add(cutscene);
        
        
        world->FindNode("MAIN")->AddChild(go);
    }

    //=== fixing rendering order
    auto turretsHolderGO = std::make_shared<GameObject>();
    turretsHolderGO->AddComponent(std::make_shared<cmp::Transform>());
    auto turretsHolderNode = world->FindNode("MAIN")->AddChild(turretsHolderGO);

    playerPlace->turretsHolder = turretsHolderNode.get();

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
        
        world->FindNode("GUI")->AddChild(go);
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
    GO_CROSSHAIR->GetComponent<cmp::Transform>()->SetPosition(GameApplication::GetAspectRatio() * 0.5f, 0.5f, 0.1f);

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



    
    //Position multitool

    auto mtTransform = GO_MULTITOOL->GetComponent<cmp::Transform>();
    auto m = glm::inverse(goCamera->GetComponent<cmp::Camera>()->GetView());

    glm::vec4 mtNewPosition = m * glm::vec4(mtTransform->GetPosition(), 1.0f);

    m[3][0] = mtNewPosition.x;
    m[3][1] = mtNewPosition.y;
    m[3][2] = mtNewPosition.z;
    mtTransform->SetModelMatrix(m);


    auto flashLight = GO_FLASHLIGHT->GetComponent<cmp::SpotLight>();
    flashLight->SetPosition({ mtNewPosition.x, mtNewPosition.y, mtNewPosition.z });
    flashLight->SetDirection(goCamera->GetComponent<cmp::Camera>()->GetForward());


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
