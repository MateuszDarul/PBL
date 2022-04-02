#include "Scene.h"
#include "GameApplication.h"
#include "ResourceManager.h"
#include "InputManager.h"

#include "Scripts/TestScript.h"
#include "Scripts/StatsScript.h"

Scene::Scene()
{
    ResourceManager* resMan = GameApplication::GetResourceManager();

    mic = new ModelInstancesComponent();
    mic->Create(9, 
            resMan->GetMesh("Resources/models/Crate/Crate.obj"),
            resMan->GetMaterial("Resources/models/Crate/Crate.mtl")
    );
    shader = new ShaderComponent();
    shader->Create("Resources/shaders/inst.vert", "Resources/shaders/inst.frag");
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
    auto matrix = mic->GetTransformation(2);
    matrix[3][1] += 5.0f;
    mic->SetTransformation(2, matrix);
    mic->UpdateTransformations();

    ///***

    ModelComponent* mc = new ModelComponent();
    mc->Create(
        resMan->GetMesh("Resources/models/Crate/Crate.obj"),
        resMan->GetMaterial("Resources/models/Crate/Crate.mtl")
    );
    ShaderComponent *shader_d = new ShaderComponent();
    shader_d->Create("Resources/shaders/default.vert", "Resources/shaders/default.frag");
    TransformComponent* tc = new TransformComponent();


    go.AddComponent(shader_d);
    go.AddComponent(mc);
    go.AddComponent(tc);



    //==== scripts test ====

    //create ScriptComponenent that holds script instances
    ScriptComponent* scriptHolder = new ScriptComponent();

    //create script instance and assign gameobject
    TestScript* testScript = new TestScript();
    testScript->gameObject = &scrpitableGO;

    //add script to the gameobject component
    scriptHolder->Add(testScript);

    //add component to the gameobject
    scrpitableGO.AddComponent(scriptHolder);


    //------ adding another script -------

    StatsScript* stats = new StatsScript();
    stats->gameObject = &scrpitableGO;

    //simulate reading saved values from file
    stats->stat2 = 2.0f;

    //add another script to the scriptableGO
    scriptHolder->Add(stats);

    //reference a script from another script
    testScript->stats = stats;
    
    //------------------------------------


    //call all OnStart methods (for each GameObject that has ScriptComponent)
    scriptHolder->OnStart();
}

Scene::~Scene()
{

}

void Scene::OnUpdate(float dt)
{
    camX = sin(glfwGetTime()) * radius;
    camZ = cos(glfwGetTime()) * radius;
    projection = *GameApplication::GetProjection();
    view = glm::lookAt(glm::vec3(camX, 5.f, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    transform = projection * view * glm::mat4(1.f);

    shader->Use();
    shader->SetMat4("transform", transform);
    mic->Draw(shader);

    go.GetComponent<cmp::Transform>()->Rotate(0, 1, 0);
    go.GetComponent<cmp::Shader>()->Use();
    go.GetComponent<cmp::Shader>()->SetMat4("transform", 
    transform * go.GetComponent<cmp::Transform>()->GetModelMatrix());
    go.GetComponent<cmp::Model>()->Draw(go.GetComponent<cmp::Shader>());



    //call all OnUpdate methods (for each GameObject that has ScriptComponent)
    scrpitableGO.GetComponent<ScriptComponent>()->OnUpdate(dt);
}









