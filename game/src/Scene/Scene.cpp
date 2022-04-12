#include "Scene.h"

Scene::Scene()
{
    collidersManager = new CollidersManager();

    glfwSetInputMode(GameApplication::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    ResourceManager* resMan = GameApplication::GetResourceManager();

    TransformComponent* tc;
    ModelComponent* mc;
    ModelInstancesComponent* mic;
    GameObject* go;

    scene = new SceneNode(new GameObject());
    scene->GetGameObject()->AddComponent(new cmp::Name(scene, "ROOT"));
    scene->GetGameObject()->AddComponent(new cmp::Transform(scene));

    ///***
    goCamera = new GameObject();
    goCamera->AddComponent(new CameraComponent(goCamera));
    goCamera->GetComponent<cmp::Camera>()->Create(glm::vec3(0,3,10));
    goCamera->GetComponent<cmp::Camera>()->SetSpeed(5);

    ///***

    ShaderComponent *shader_d = new ShaderComponent(scene);
    shader_d->Create("Resources/shaders/default.vert", "Resources/shaders/default.frag");
    ShaderComponent* shader_i = new ShaderComponent(scene);
    shader_i->Create("Resources/shaders/inst.vert", "Resources/shaders/inst.frag");

    ///***

    go = new GameObject();
    tc = new TransformComponent(go);
    mc = new ModelComponent(go);
    mc->Create(
        resMan->GetMesh("Resources/models/Crate/Crate.obj"),
        resMan->GetMaterial("Resources/models/Crate/Crate.mtl")
    );
    go->AddComponent(shader_d);
    go->AddComponent(mc);
    go->AddComponent(tc);
    go->AddComponent(new cmp::Name(go, "GO"));

    scene->AddChild(go);

    ///***

    go = new GameObject();
    tc = new TransformComponent(go);
    mc = new ModelComponent(go);
    mc->Create(
        resMan->GetMesh("Resources/models/Crate/Crate.obj"),
        resMan->GetMaterial("Resources/models/Crate/Crate.mtl")
    );
    go->AddComponent(shader_d);
    go->AddComponent(mc);
    go->AddComponent(tc);
    go->GetComponent<cmp::Transform>()->SetPosition(5, 0, 0);
    go->AddComponent(new cmp::Name(go, "GO1"));

    scene->FindNode("GO")->AddChild(go);

    ///***

    go = new GameObject();
    tc = new TransformComponent(go);
    mic = new ModelInstancesComponent(go);
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
    go->AddComponent(shader_i);
    go->AddComponent(mic);
    go->AddComponent(new cmp::Name(go, "GO2"));

    scene->AddChild(go);

    ///***

    go = new GameObject();
    tc = new TransformComponent(go);
    mc = new ModelComponent(go);
    mc->Create(
        resMan->GetMesh("Resources/models/sphere.obj"),
        resMan->GetMaterial("Resources/models/sphere.mtl")
    );
    go->AddComponent(shader_d);
    go->AddComponent(mc);
    go->AddComponent(tc);
    go->GetComponent<TransformComponent>()->SetPosition(0.0f, 0.0f, 20.0f);
    go->AddComponent(new cmp::Name(go, "Ball1"));
    go->AddComponent(new cmp::SphereCol(go, collidersManager, false, false));
    go->GetComponent<SphereCollider>()->radius = 5;

    scene->AddChild(go);

    ///***

    go = new GameObject();
    tc = new TransformComponent(go);
    mc = new ModelComponent(go);
    mc->Create(
        resMan->GetMesh("Resources/models/sphere.obj"),
        resMan->GetMaterial("Resources/models/sphere.mtl")
    );
    go->AddComponent(shader_d);
    go->AddComponent(mc);
    go->AddComponent(tc);
    go->GetComponent<TransformComponent>()->SetPosition(0.0f, 0.0f, -20.0f);
    go->AddComponent(new cmp::Name(go, "Ball2"));
    go->AddComponent(new cmp::SphereCol(go, collidersManager, false, false));
    go->GetComponent<SphereCollider>()->radius = 5;

    scene->AddChild(go);

    ///***

    scene->FindNode("GO")->GetLocalTransformations()->SetPosition(0, 2, 0);
}

Scene::~Scene()
{
    delete goCamera->GetComponent<cmp::Camera>();
}

void Scene::OnUpdate(float dt)
{
    goCamera->GetComponent<cmp::Camera>()->Update(GameApplication::GetInputManager(), dt);
    transform = *GameApplication::GetProjection() * goCamera->GetComponent<cmp::Camera>()->GetView();

    scene->FindNode("GO")->GetLocalTransformations()->Rotate(0, 180*dt, 0);
    scene->FindNode("Ball1")->GetComponent<TransformComponent>()->Move(0.0f, 0.0f, -0.5f);
    collidersManager->CheckCollisions();
    scene->Render(transform);
}