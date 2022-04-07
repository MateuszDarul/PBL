#include "Scene.h"

#include "Scripts/TestScript.h"
#include "Scripts/StatsScript.h"

Scene::Scene()
{
    glfwSetInputMode(GameApplication::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    ResourceManager* resMan = GameApplication::GetResourceManager();

    TransformComponent* tc;
    ModelComponent* mc;
    ModelInstancesComponent* mic;
    GameObject* go;

    scene = new SceneNode(new GameObject());
    scene->GetGameObject()->AddComponent(new cmp::Name("ROOT"));
    scene->GetGameObject()->AddComponent(new cmp::Transform());

    ///***

    goCamera.AddComponent(new CameraComponent());
    goCamera.GetComponent<cmp::Camera>()->Create(glm::vec3(0,3,10));
    goCamera.GetComponent<cmp::Camera>()->SetSpeed(5);

    ///***

    ShaderComponent *shader_d = new ShaderComponent();
    shader_d->Create("Resources/shaders/default.vert", "Resources/shaders/default.frag");
    ShaderComponent* shader_i = new ShaderComponent();
    shader_i->Create("Resources/shaders/inst.vert", "Resources/shaders/inst.frag");

    ///***

    tc = new TransformComponent();
    mc = new ModelComponent();
    mc->Create(
        resMan->GetMesh("Resources/models/Crate/Crate.obj"),
        resMan->GetMaterial("Resources/models/Crate/Crate.mtl")
    );
    go = new GameObject();
    go->AddComponent(shader_d);
    go->AddComponent(mc);
    go->AddComponent(tc);
    go->AddComponent(new cmp::Name("GO"));

    scene->AddChild(go);

    ///***

    tc = new TransformComponent();
    mc = new ModelComponent();
    mc->Create(
        resMan->GetMesh("Resources/models/Crate/Crate.obj"),
        resMan->GetMaterial("Resources/models/Crate/Crate.mtl")
    );
    go = new GameObject();
    go->AddComponent(shader_d);
    go->AddComponent(mc);
    go->AddComponent(tc);
    go->GetComponent<cmp::Transform>()->SetPosition(5, 0, 0);
    go->AddComponent(new cmp::Name("GO1"));

    scene->FindNode("GO")->AddChild(go);

    ///***

    tc = new TransformComponent();
    mic = new ModelInstancesComponent();
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
    go = new GameObject();
    go->AddComponent(shader_i);
    go->AddComponent(mic);
    go->AddComponent(new cmp::Name("GO2"));

    scene->AddChild(go);

    ///***

    scene->FindNode("GO")->GetLocalTransformations()->SetPosition(0, 2, 0);
}

Scene::~Scene()
{
    delete goCamera.GetComponent<cmp::Camera>();
}

void Scene::OnUpdate(float dt)
{
    goCamera.GetComponent<cmp::Camera>()->Update(GameApplication::GetInputManager(), dt);
    transform = *GameApplication::GetProjection() * goCamera.GetComponent<cmp::Camera>()->GetView();

    scene->FindNode("GO")->GetLocalTransformations()->Rotate(0, 180*dt, 0);
    scene->Render(transform);
}