#include "Scene.h"

#include "Scripts/TestScript.h"
#include "Scripts/StatsScript.h"
#include "Scripts/RotatorScript.h"

Scene::Scene()
{
    glfwSetInputMode(GameApplication::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    ResourceManager* resMan = GameApplication::GetResourceManager();

    std::shared_ptr<cmp::Model> mc;
    std::shared_ptr<cmp::ModelInst> mic;
    std::shared_ptr<GameObject> go;

    scene = new SceneNode(std::make_shared<GameObject>());
    scene->GetGameObject()->AddComponent(std::make_shared<cmp::Name>("ROOT"));
    scene->GetGameObject()->AddComponent(std::make_shared<cmp::Transform>());

    ///***

    goCamera = new GameObject();
    goCamera->AddComponent(std::make_shared<CameraComponent>());
    goCamera->GetComponent<cmp::Camera>()->Create(glm::vec3(0,3,10));
    goCamera->GetComponent<cmp::Camera>()->SetSpeed(5);

    ///***

    std::shared_ptr<ShaderComponent> shader_d = std::make_shared<ShaderComponent>();
    shader_d->Create("Resources/shaders/default.vert", "Resources/shaders/default.frag");
    std::shared_ptr<ShaderComponent> shader_i = std::make_shared<ShaderComponent>();
    shader_i->Create("Resources/shaders/inst.vert", "Resources/shaders/inst.frag");

    ///***

    mc = std::make_shared<ModelComponent>();
    mc->Create(
        resMan->GetMesh("Resources/models/Crate/Crate.obj"),
        resMan->GetMaterial("Resources/models/Crate/Crate.mtl")
    );
    go = std::make_shared<GameObject>();
    go->AddComponent(std::make_shared<cmp::Name>("GO"));
    go->AddComponent(shader_d);
    go->AddComponent(mc);
    go->AddComponent(std::make_shared<cmp::Transform>());

    scene->AddChild(go);

    ///***

    mc = std::make_shared<ModelComponent>();
    mc->Create(
        resMan->GetMesh("Resources/models/Crate/Crate.obj"),
        resMan->GetMaterial("Resources/models/Crate/Crate.mtl")
    );
    go = std::make_shared<GameObject>();
    go->AddComponent(shader_d);
    go->AddComponent(mc);
    go->AddComponent(std::make_shared<cmp::Transform>());
    go->AddComponent(std::make_shared<cmp::Name>("GO1"));

    scene->FindNode("GO")->AddChild(go);

    ///***

    mic = std::make_shared<ModelInstancesComponent>();
    mic->Create(9,
        resMan->GetMesh("Resources/models/Crate/Crate.obj"),
        resMan->GetMaterial("Resources/models/Crate/Crate.mtl")
    );
    for(int x=-4, y=-4, i=0; i<9; i++)
    {
        mic->SetTransformation(i, TransformComponent::Transform(glm::vec3(x, 0, y), glm::vec3(0, 0, 0), 1));
        x += 4;
        if((x + 1) % 3 == 0)
        {
            x = -4;
            y += 4;
        }
    }
    mic->UpdateTransformations();
    go = std::make_shared<GameObject>();
    go->AddComponent(std::make_shared<cmp::Name>("GO2"));
    go->AddComponent(std::make_shared<cmp::Transform>());
    go->AddComponent(shader_i);
    go->AddComponent(mic);

    scene->AddChild(go);

    ///***

    scene->FindNode("GO")->GetLocalTransformations()->SetPosition(0, 2, 0);
    scene->FindNode("GO1")->GetLocalTransformations()->SetPosition(5, 0, 0);
    scene->FindNode("GO2")->GetLocalTransformations()->SetPosition(0, -2, 0);


    //=== text test ===

    std::shared_ptr<ShaderComponent> textShader = std::make_shared<ShaderComponent>();
    textShader->Create("Resources/shaders/text.vert", "Resources/shaders/text.frag");
    
    std::shared_ptr<TextComponent> textComponent = std::make_shared<TextComponent>();
    textComponent->Create("Sample text", resMan->GetFont("Resources/fonts/arial.ttf"));
    textComponent->color = {1.0f, 1.0f, 0.0f};


    go = std::make_shared<GameObject>();
    go->AddComponent(std::make_shared<cmp::Name>("In world text"));
    go->AddComponent(std::make_shared<cmp::Transform>());

    go->AddComponent(textShader);
    go->AddComponent(textComponent);

    scene->AddChild(go);

    scene->FindNode("In world text")->GetLocalTransformations()->SetPosition(15, 0, 0);

    std::shared_ptr<cmp::Scriptable> scriptHolder = std::make_shared<ScriptComponent>();
    RotatorScript* rotScript = new RotatorScript();
    
    rotScript->gameObject = go.get();
    rotScript->speed = 50.0f;

    
    go->AddComponent(scriptHolder);
    scriptHolder->Add(rotScript);
    scriptHolder->OnStart();

    //-
    textComponent = std::make_shared<TextComponent>();
    textComponent->Create("Facing camera", resMan->GetFont("Resources/fonts/arial.ttf"));
    textComponent->alwaysSeen = true;


    go = std::make_shared<GameObject>();
    go->AddComponent(std::make_shared<cmp::Name>("Face camera text"));
    go->AddComponent(std::make_shared<cmp::Transform>());

    go->AddComponent(textShader);
    go->AddComponent(textComponent);

    scene->AddChild(go);

    scene->FindNode("Face camera text")->GetLocalTransformations()->SetPosition(10, 0, 0);
}

Scene::~Scene()
{
    delete scene;
    scene = nullptr;

    delete goCamera;
    goCamera = nullptr;
}

void Scene::Update(float dt)
{
    goCamera->GetComponent<cmp::Camera>()->Update(GameApplication::GetInputManager(), dt);
    transform = GameApplication::GetProjection() * goCamera->GetComponent<cmp::Camera>()->GetView();

    scene->FindNode("GO")->GetGameObject()->GetComponent<cmp::Transform>()->Rotate(0, 180*dt, 0);

    scene->FindNode("In world text")->GetGameObject()->GetComponent<ScriptComponent>()->OnUpdate(dt);

    auto faceTextNode = scene->FindNode("Face camera text");
    faceTextNode->GetGameObject()->GetComponent<cmp::Text>()->FaceCamera(goCamera->GetComponent<cmp::Camera>(), faceTextNode);
}

void Scene::Render()
{
    scene->Update();
    scene->Render(transform);
}