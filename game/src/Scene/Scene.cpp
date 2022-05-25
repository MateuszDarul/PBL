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

    auto radialShader = std::make_shared<cmp::Shader>();
    radialShader->Create("Resources/shaders/radial.vert", "Resources/shaders/radial.frag");

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
        go->AddComponent(std::make_shared<cmp::Name>("Particle emitter"));
        std::shared_ptr<cmp::Particles> particles = go->GetComponent<cmp::Particles>();
        particles->Create(world->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Camera>(), true, 3);
        particles->SetTexture("Resources/textures/smoke.png");
        particles->SetParticlesPerSecond(50.0f);
        particles->SetOffset(glm::vec3(2.25f, 2.05f, 0));
        particles->SetDirection({0.0f, 1.0f, 0.0f});
        particles->SetDirectionVar(45);  
        particles->SetParticleLifetime(0.6f);
        particles->SetScale(0.9f, 0.6f);
        particles->SetSpeed(0.4f);

        // particles->SetColor({ 1.0f, 1.0f, 0.0f,   1.0f },   { 1.0f, 0.3f, 0.1f,   0.2f });
        particles->SetColor({ 1.0f, 1.0f, 1.0f,   0.4f },   { 0.8f, 0.8f, 0.8f,   0.01f });
        particles->SetForce({ 0.0f, -0.3f, 0.0f });
    }
    //world->AddChild(go);
    auto particlego = go;

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

    //- door
    {      
        go = std::make_shared<GameObject>();
        tc = std::make_shared<TransformComponent>();
        tc->SetPosition(-2.5f, 3.0f, -10.5f);
        tc->SetRotation(0.0f, 90.0f, 0.0f);
        
        go->AddComponent(tc);
        mc = std::make_shared<ModelComponent>();
        mc->Create(
            resMan->GetMesh("Resources/models/wall/wall.obj"),
            resMan->GetMaterial("Resources/models/wall/wall.mtl")
        );
        go->AddComponent(shader_d);
        go->AddComponent(mc);
        go->AddComponent(std::make_shared<cmp::Name>("Door"));
        go->AddComponent(std::make_shared<cmp::BoxCol>(false, true));
        go->GetComponent<BoxCollider>()->AddToCollidersManager(collidersManager);
        go->GetComponent<BoxCollider>()->setLengths({ 5.0f, 5.0f, 1.0f });
        go->AddComponent(std::make_shared<FrustumCullingComponent>());
        go->GetComponent<cmp::FrustumCulling>()->Create(
                resMan->GetMesh("Resources/models/wall/wall.obj")
            );

        world->AddChild(go);
    }

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

        doorScript->doorTransform = world->FindNode("Door")->GetGameObject()->GetComponent<cmp::Transform>();


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

    //=displacement shader test
    auto displShader = std::make_shared<ShaderComponent>();
    displShader->Create("Resources/shaders/displ.vert", "Resources/shaders/displ.frag");

    {      
        go = std::make_shared<GameObject>();
        tc = std::make_shared<TransformComponent>();
        tc->SetPosition(-5.0f, 6.5f, -5.0f);
        
        go->AddComponent(tc);
        mc = std::make_shared<ModelComponent>();
        mc->Create(
            resMan->GetMesh("Resources/models/displ/human.obj"),
            resMan->GetMaterial("Resources/models/displ/human1.mtl")
        );
        go->AddComponent(displShader);
        go->AddComponent(mc);
        go->AddComponent(std::make_shared<cmp::Name>("Displaced enemy 1"));
        go->AddComponent(std::make_shared<FrustumCullingComponent>());
        go->GetComponent<cmp::FrustumCulling>()->Create(
                resMan->GetMesh("Resources/models/displ/human.obj")
            );

        world->AddChild(go);
    }
    {      
        go = std::make_shared<GameObject>();
        tc = std::make_shared<TransformComponent>();
        tc->SetPosition(-10.0f, 6.5f, -5.0f);
        
        go->AddComponent(tc);
        mc = std::make_shared<ModelComponent>();
        mc->Create(
            resMan->GetMesh("Resources/models/displ/human.obj"),
            resMan->GetMaterial("Resources/models/displ/human2.mtl")
        );
        go->AddComponent(displShader);
        go->AddComponent(mc);
        go->AddComponent(std::make_shared<cmp::Name>("Displaced enemy 2"));
        go->AddComponent(std::make_shared<FrustumCullingComponent>());
        go->GetComponent<cmp::FrustumCulling>()->Create(
                resMan->GetMesh("Resources/models/displ/human.obj")
            );

        world->AddChild(go);
    }
    {      
        go = std::make_shared<GameObject>();
        tc = std::make_shared<TransformComponent>();
        tc->SetPosition(0.0f, 6.5f, -5.0f);
        
        go->AddComponent(tc);
        mc = std::make_shared<ModelComponent>();
        mc->Create(
            resMan->GetMesh("Resources/models/displ/human.obj"),
            resMan->GetMaterial("Resources/models/displ/human3.mtl")
        );
        go->AddComponent(displShader);
        go->AddComponent(mc);
        go->AddComponent(std::make_shared<cmp::Name>("Displaced enemy 3"));
        go->AddComponent(std::make_shared<FrustumCullingComponent>());
        go->GetComponent<cmp::FrustumCulling>()->Create(
                resMan->GetMesh("Resources/models/displ/human.obj")
            );

        world->AddChild(go);
    }

    //test - adding particle component last
    auto partnode = world->AddChild(particlego);

    {
        go = std::make_shared<GameObject>();
        tc = std::make_shared<TransformComponent>();
        tc->SetPosition(0,0.5,-5);
        
        go->AddComponent(tc);
        go->AddComponent(std::make_shared<cmp::Name>("muzzler"));

        go->AddComponent(std::make_shared<cmp::Particles>());
        std::shared_ptr<cmp::Particles> particles = go->GetComponent<cmp::Particles>();
        particles->Create(world->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Camera>(), true, 3);
        particles->SetTexture("Resources/textures/muzzle.png");
        particles->SetParticlesPerSecond(50.0f);
        particles->SetOffset(glm::vec3(2.01f, 1.95f, 0));
        particles->SetDirection({0.0f, 1.0f, 0.0f});
        particles->SetDirectionVar(45);  
        particles->SetParticleLifetime(0.15f);
        particles->SetScale(0.9f, 0.6f);
        particles->SetSpeed(0.0f);

        // particles->SetColor({ 1.0f, 1.0f, 0.0f,   1.0f },   { 1.0f, 0.3f, 0.1f,   0.2f });
        particles->SetColor({ 1.0f, 1.0f, 0.5f,   0.8f },   { 0.8f, 0.8f, 0.4f,   0.01f });
        // particles->SetForce({ 0.0f, -0.3f, 0.0f });

        partnode->AddChild(go);
    }

    {
        go = std::make_shared<GameObject>();
        tc = std::make_shared<TransformComponent>();
        tc->SetPosition(11,0.5,-5);
        
        go->AddComponent(tc);
        go->AddComponent(std::make_shared<cmp::Name>("sparks"));

        go->AddComponent(std::make_shared<cmp::Particles>());
        std::shared_ptr<cmp::Particles> particles = go->GetComponent<cmp::Particles>();
        particles->Create(world->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Camera>(), true, 10);
        particles->SetTexture("Resources/textures/particle.png");
        particles->SetParticlesPerSecond(50.0f);
        particles->SetOffset(glm::vec3(0, 1.95f, 0));
        particles->SetDirection({-1.0f, 0.4f, 0.0f});
        particles->SetDirectionVar(45);  
        particles->SetParticleLifetime(0.35f);
        particles->SetScale(0.2f, 0.01f);
        particles->SetSpeed(3.0f);

        particles->SetColor({ 1.0f, 1.0f, 0.0f,   1.0f },   { 1.0f, 0.3f, 0.1f,   0.2f });
        particles->SetForce({ 0.0f, -2.5f, 0.0f });

        world->AddChild(go);
    }

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
    // world->FindNode("Raycaster")->GetGameObject()->GetComponent<cmp::Transform>()->Rotate(0.0f, -inputHoriz * 12.74f * dt, 0.0f);
    // world->FindNode("Particle emitter")->GetGameObject()->GetComponent<cmp::Transform>()->Rotate(0.0f, -inputHoriz * 12.74f*dt, 0.0f);
    // world->FindNode("Particle emitter")->GetGameObject()->GetComponent<cmp::Transform>()->Rotate(-inputVert * 12.74f*dt,0.0f, 0.0f);
    // world->FindNode("Particle emitter")->GetGameObject()->GetComponent<cmp::Transform>()->Move(inputHoriz * dt * 5.0f, 0.0f, 0.0f);
    if (Input()->Keyboard()->OnPressed(KeyboardKey::Space)) 
    {
        auto p = world->FindNode("Particle emitter");
        p->GetGameObject()->GetComponent<cmp::Particles>()->Burst();
        p->FindNode("muzzler")->GetGameObject()->GetComponent<cmp::Particles>()->Burst();
        world->FindNode("sparks")->GetGameObject()->GetComponent<cmp::Particles>()->Burst();
    }
    

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
