#include "Scene.h"

#include "Scripts/TestScript.h"
#include "Scripts/StatsScript.h"
#include "Scripts/RaycastTest.h"

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
    shader_d->Use();
    shader_d->SetFloat("brightness", brightness);
    shader_d->SetFloat("gamma", gamma);
    shader_d->SetFloat("contrast", contrast);

    std::shared_ptr<ShaderComponent> shader_i = std::make_shared<ShaderComponent>();
    shader_i->Create("Resources/shaders/inst.vert", "Resources/shaders/inst.frag");
    shader_i->Use();
    shader_i->SetFloat("brightness", brightness);
    shader_i->SetFloat("gamma", gamma);
    shader_i->SetFloat("contrast", contrast);

    std::shared_ptr<ShaderComponent> shader_l = std::make_shared<ShaderComponent>();
    shader_l->Create("Resources/shaders/light.vert", "Resources/shaders/light.frag");
    shader_l->Use();
    shader_l->SetFloat("brightness", brightness);
    shader_l->SetFloat("gamma", gamma);
    shader_l->SetFloat("contrast", contrast);

    ///***

    world = new SceneNode(std::make_shared<GameObject>());
    world->GetGameObject()->AddComponent(std::make_shared<cmp::Name>("ROOT"));
    world->GetGameObject()->AddComponent(std::make_shared<cmp::Transform>());

    ///***

    go = std::make_shared<GameObject>();
    go->AddComponent(std::make_shared<NameComponent>("shader_d_obj"));
    go->AddComponent(shader_d);
    world->AddChild(go);

    go = std::make_shared<GameObject>();
    go->AddComponent(std::make_shared<NameComponent>("shader_i_obj"));
    go->AddComponent(shader_i);
    world->AddChild(go);

    go = std::make_shared<GameObject>();
    go->AddComponent(std::make_shared<NameComponent>("shader_l_obj"));
    go->AddComponent(shader_l);
    world->AddChild(go);

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

    ///***

    std::shared_ptr<cmp::Shader> shader[2];
    shader[0] = shader_l;
    shader[1] = shader_i;
    MapLoader::Load("Resources/maps/world.map", world, shader, collidersManager);

    ///***
   
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
    //postprocess
    if (GameApplication::GetInputManager()->Keyboard()->IsPressed(KeyboardKey::T))
    {
        brightness -= 0.01f;
        world->FindNode("shader_d_obj")->GetGameObject()->GetComponent<cmp::Shader>()->Use();
        world->FindNode("shader_d_obj")->GetGameObject()->GetComponent<cmp::Shader>()->SetFloat("brightness", brightness);
        world->FindNode("shader_i_obj")->GetGameObject()->GetComponent<cmp::Shader>()->Use();
        world->FindNode("shader_i_obj")->GetGameObject()->GetComponent<cmp::Shader>()->SetFloat("brightness", brightness);
        world->FindNode("shader_l_obj")->GetGameObject()->GetComponent<cmp::Shader>()->Use();
        world->FindNode("shader_l_obj")->GetGameObject()->GetComponent<cmp::Shader>()->SetFloat("brightness", brightness);
        std::cout << "b:" << brightness << std::endl;
    }

    if (GameApplication::GetInputManager()->Keyboard()->IsPressed(KeyboardKey::Y))
    {
        brightness += 0.01f;
        world->FindNode("shader_d_obj")->GetGameObject()->GetComponent<cmp::Shader>()->Use();
        world->FindNode("shader_d_obj")->GetGameObject()->GetComponent<cmp::Shader>()->SetFloat("brightness", brightness);
        world->FindNode("shader_i_obj")->GetGameObject()->GetComponent<cmp::Shader>()->Use();
        world->FindNode("shader_i_obj")->GetGameObject()->GetComponent<cmp::Shader>()->SetFloat("brightness", brightness);
        world->FindNode("shader_l_obj")->GetGameObject()->GetComponent<cmp::Shader>()->Use();
        world->FindNode("shader_l_obj")->GetGameObject()->GetComponent<cmp::Shader>()->SetFloat("brightness", brightness);
        std::cout << "b:" << brightness << std::endl;
    }

    if (GameApplication::GetInputManager()->Keyboard()->IsPressed(KeyboardKey::G))
    {
        gamma -= 0.01f;
        world->FindNode("shader_d_obj")->GetGameObject()->GetComponent<cmp::Shader>()->Use();
        world->FindNode("shader_d_obj")->GetGameObject()->GetComponent<cmp::Shader>()->SetFloat("gamma", gamma);
        world->FindNode("shader_i_obj")->GetGameObject()->GetComponent<cmp::Shader>()->Use();
        world->FindNode("shader_i_obj")->GetGameObject()->GetComponent<cmp::Shader>()->SetFloat("gamma", gamma);
        world->FindNode("shader_l_obj")->GetGameObject()->GetComponent<cmp::Shader>()->Use();
        world->FindNode("shader_l_obj")->GetGameObject()->GetComponent<cmp::Shader>()->SetFloat("gamma", gamma);
        std::cout << "g:" << gamma << std::endl;
    }

    if (GameApplication::GetInputManager()->Keyboard()->IsPressed(KeyboardKey::H))
    {
        gamma += 0.01f;
        world->FindNode("shader_d_obj")->GetGameObject()->GetComponent<cmp::Shader>()->Use();
        world->FindNode("shader_d_obj")->GetGameObject()->GetComponent<cmp::Shader>()->SetFloat("gamma", gamma);
        world->FindNode("shader_i_obj")->GetGameObject()->GetComponent<cmp::Shader>()->Use();
        world->FindNode("shader_i_obj")->GetGameObject()->GetComponent<cmp::Shader>()->SetFloat("gamma", gamma);
        world->FindNode("shader_l_obj")->GetGameObject()->GetComponent<cmp::Shader>()->Use();
        world->FindNode("shader_l_obj")->GetGameObject()->GetComponent<cmp::Shader>()->SetFloat("gamma", gamma);
        std::cout << "g:" << gamma << std::endl;
    }

    if (GameApplication::GetInputManager()->Keyboard()->IsPressed(KeyboardKey::B))
    {
        contrast -= 0.01f;
        world->FindNode("shader_d_obj")->GetGameObject()->GetComponent<cmp::Shader>()->Use();
        world->FindNode("shader_d_obj")->GetGameObject()->GetComponent<cmp::Shader>()->SetFloat("contrast", contrast);
        world->FindNode("shader_i_obj")->GetGameObject()->GetComponent<cmp::Shader>()->Use();
        world->FindNode("shader_i_obj")->GetGameObject()->GetComponent<cmp::Shader>()->SetFloat("contrast", contrast);
        world->FindNode("shader_l_obj")->GetGameObject()->GetComponent<cmp::Shader>()->Use();
        world->FindNode("shader_l_obj")->GetGameObject()->GetComponent<cmp::Shader>()->SetFloat("contrast", contrast);
        std::cout << "c:" << contrast << std::endl;
    }

    if (GameApplication::GetInputManager()->Keyboard()->IsPressed(KeyboardKey::N))
    {
        contrast += 0.01f;
        world->FindNode("shader_d_obj")->GetGameObject()->GetComponent<cmp::Shader>()->Use();
        world->FindNode("shader_d_obj")->GetGameObject()->GetComponent<cmp::Shader>()->SetFloat("contrast", contrast);
        world->FindNode("shader_i_obj")->GetGameObject()->GetComponent<cmp::Shader>()->Use();
        world->FindNode("shader_i_obj")->GetGameObject()->GetComponent<cmp::Shader>()->SetFloat("contrast", contrast);
        world->FindNode("shader_l_obj")->GetGameObject()->GetComponent<cmp::Shader>()->Use();
        world->FindNode("shader_l_obj")->GetGameObject()->GetComponent<cmp::Shader>()->SetFloat("contrast", contrast);
        std::cout << "c:" << contrast << std::endl;
    }

    std::shared_ptr<GameObject> goCamera = world->FindNode("CAMERA")->GetGameObject();
    std::shared_ptr<TransformComponent> transformCamera = goCamera->GetComponent<cmp::Transform>();

    goCamera->GetComponent<CameraComponent>()->Update(GameApplication::GetInputManager(), dt);
    transformCamera->SetPosition(goCamera->GetComponent<CameraComponent>()->GetPosition());

    transform = GameApplication::GetProjection() * goCamera->GetComponent<CameraComponent>()->GetView();

    collidersManager->CheckEverything();
    goCamera->GetComponent<CameraComponent>()->SetPosition(transformCamera->GetPosition());
    world->Update(dt);
}

void Scene::Render()
{
    world->Render(transform);
}