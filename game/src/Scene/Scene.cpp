#include "Scene.h"

Scene::Scene()
{
    glfwSetInputMode(GameApplication::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    ResourceManager* resMan = GameApplication::GetResourceManager();

    TransformComponent* tc;
    ModelComponent* mc;

    scene = new SceneNode(new GameObject());
    scene->GetGameObject()->AddComponent(new cmp::Transform());
    //scene->GetGameObject()->GetComponent<cmp::Transform>();

    ///***

    goCamera.AddComponent(new CameraComponent());
    goCamera.GetComponent<cmp::Camera>()->Create(glm::vec3(0,3,10));
    goCamera.GetComponent<cmp::Camera>()->SetSpeed(5);

    ///***

    ShaderComponent *shader_d = new ShaderComponent();
    shader_d->Create("Resources/shaders/default.vert", "Resources/shaders/default.frag");

    tc = new TransformComponent();
    mc = new ModelComponent();
    mc->Create(
        resMan->GetMesh("Resources/models/Crate/Crate.obj"),
        resMan->GetMaterial("Resources/models/Crate/Crate.mtl")
    );
    GameObject* go = new GameObject();
    go->AddComponent(shader_d);
    go->AddComponent(mc);
    go->AddComponent(tc);


    tc = new TransformComponent();
    mc = new ModelComponent();
    mc->Create(
        resMan->GetMesh("Resources/models/Crate/Crate.obj"),
        resMan->GetMaterial("Resources/models/Crate/Crate.mtl")
    );
    GameObject* go2 = new GameObject();
    go2->AddComponent(shader_d);
    go2->AddComponent(mc);
    go2->AddComponent(tc);

    ///***

    /*
    ShaderComponent* shader = new ShaderComponent();
    shader->Create("Resources/shaders/inst.vert", "Resources/shaders/inst.frag");

    ModelInstancesComponent* mic = new ModelInstancesComponent();
    mic->Create(9, 
            resMan->GetMesh("Resources/models/Crate/Crate.obj"),
            resMan->GetMaterial("Resources/models/Crate/Crate.mtl")
    );
    for(int x=-4, y=-4, i=0; i<9; i++)
    {
        mic->SetTransformation(i, TransformComponent::Transform(glm::vec3(x, -4, y), glm::vec3(0, 0, 0), 1));
        x += 4;
        if((x + 1) % 3 == 0)
        {
            x = -4;
            y += 4;
        }
    }
    mic->UpdateTransformations();

    GameObject* go1 = new GameObject();
    go1->AddComponent(shader);
    go1->AddComponent(mic);
    */

    ///***


    ///***



    go_node = new SceneNode(go);

    scene->AddChild(go_node);

    go2->GetComponent<cmp::Transform>()->SetPosition(5, 0, 0);
    go_node->AddChild(go2);

    //scene->AddChild(go1);
}

Scene::~Scene()
{
    delete goCamera.GetComponent<cmp::Camera>();
}

void Scene::OnUpdate(float dt)
{
    goCamera.GetComponent<cmp::Camera>()->Update(GameApplication::GetInputManager(), dt);
    transform = GameApplication::GetProjection() * goCamera.GetComponent<cmp::Camera>()->GetView();

    go_node->GetLocalTransformations()->Rotate(0, 180*dt, 0);
    scene->Render(transform);
}