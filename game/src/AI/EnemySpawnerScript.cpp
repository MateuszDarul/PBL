#include "EnemySpawnerScript.h"
#include "ResourceManager.h"
#include "GameApplication.h"
#include "EnemyScript.h"
#include "Components.h"
#include "Scripts/Health.h"

void EnemySpawnerScript::Start() {

    SpawnEnemy(1);
    std::cout << "----------------------------------------------------------------------------------------" << std::endl;

}

void EnemySpawnerScript::Update(float dt) {

}

void EnemySpawnerScript::SpawnEnemy(int nr) 
{
    ResourceManager* resMan = GameApplication::GetResourceManager();

    std::shared_ptr<GameObject> go = std::make_shared<GameObject>();
    std::shared_ptr<ModelComponent> mc = std::make_shared<ModelComponent>();
    mc->Create(
        //sceneInfo.resourceManager->GetMesh("Resources/models/ny/przeciwnik/przeciwnik/przeciwniksmooth.obj"),
        resMan->GetMesh("Resources/models/enemy/enemy.obj"),
        resMan->GetMaterial("Resources/models/displacement test/capsule.mtl")
    );
    go->AddComponent(displShader);
    go->AddComponent(mc);

    go->AddComponent(std::make_shared<FrustumCullingComponent>());
    go->GetComponent<cmp::FrustumCulling>()->Create(
        //sceneInfo.resourceManager->GetMesh("Resources/models/ny/przeciwnik/przeciwnik/przeciwnik.obj")
        resMan->GetMesh("Resources/models/enemy/enemy.obj")
    );

    go->AddComponent(std::make_shared<cmp::Transform>());
    go->GetComponent<cmp::Transform>()->SetRotation(0, 90, 0);
    go->GetComponent<cmp::Transform>()->SetScale(0.50f);


    go->AddComponent(std::make_shared<cmp::Name>("Enemy" + std::to_string(enemyEnumerator) + "," + gameObject->GetComponent<NameComponent>()->Get()));

    go->AddComponent(std::make_shared<BoxCollider>(true, false, CollisionLayer::ENEMY));
    go->GetComponent<cmp::BoxCol>()->SetLengths(glm::vec3(2, 2, 2));
    go->GetComponent<cmp::BoxCol>()->isOptimized = false;
    go->GetComponent<cmp::BoxCol>()->AddToCollidersManager(collidersManager);

    go->AddComponent(std::make_shared<cmp::Scriptable>());
    Health* health = new Health();
    health->SetMaxHealth(50.0f);
    health->scene = scene;
    go->GetComponent<cmp::Scriptable>()->Add(health);
    auto particles = std::make_shared<ParticleComponent>();

    particles->Create(playerGO->GetComponent<cmp::Camera>(), false, 40, particleShader);
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

    std::shared_ptr<Path> path = std::make_shared<Path>(true);
    path->Clear();

    for(int i = 0; i < wayPoints.size(); i++)
    {
        path->AddWayPoint(wayPoints.at(i));
    }
    go->GetComponent<cmp::Transform>()->SetPosition(wayPoints.at(0));
    path->Set();

    auto enemyScript = new EnemyScript(playerGO);
    enemyScript->health = health;
    enemyScript->multitool = multiToolScript;
    enemyScript->SetPath(path);
    go->GetComponent<cmp::Scriptable>()->Add(enemyScript);



    //right arm
    auto rightArm = std::make_shared<GameObject>();
    mc = std::make_shared<ModelComponent>();
    mc->Create(
        //sceneInfo.resourceManager->GetMesh("Resources/models/ny/przeciwnik/przeciwnik/przeciwniksmooth.obj"),
        resMan->GetMesh("Resources/models/enemy/enemyRightArm.obj"),
        resMan->GetMaterial("Resources/models/displacement test/capsule.mtl")
    );
    rightArm->AddComponent(displShader);
    rightArm->AddComponent(mc);
    rightArm->AddComponent(std::make_shared<FrustumCullingComponent>());
    rightArm->GetComponent<cmp::FrustumCulling>()->Create(
        //sceneInfo.resourceManager->GetMesh("Resources/models/ny/przeciwnik/przeciwnik/przeciwnik.obj")
        resMan->GetMesh("Resources/models/enemy/enemyRightArm.obj")
    );

    rightArm->AddComponent(std::make_shared<cmp::Transform>());
    rightArm->GetComponent<cmp::Transform>()->SetPosition(1, 4, 0);
    rightArm->AddComponent(std::make_shared<cmp::Name>("EnemyRightArm"));

    //left arm
    auto leftArm = std::make_shared<GameObject>();
    mc = std::make_shared<ModelComponent>();
    mc->Create(
        //sceneInfo.resourceManager->GetMesh("Resources/models/ny/przeciwnik/przeciwnik/przeciwniksmooth.obj"),
        resMan->GetMesh("Resources/models/enemy/enemyLeftArm.obj"),
        resMan->GetMaterial("Resources/models/displacement test/capsule.mtl")
    );
    leftArm->AddComponent(displShader);
    leftArm->AddComponent(mc);
    leftArm->AddComponent(std::make_shared<FrustumCullingComponent>());
    leftArm->GetComponent<cmp::FrustumCulling>()->Create(
        //sceneInfo.resourceManager->GetMesh("Resources/models/ny/przeciwnik/przeciwnik/przeciwnik.obj")
        resMan->GetMesh("Resources/models/enemy/enemyLeftArm.obj")
    );

    leftArm->AddComponent(std::make_shared<cmp::Transform>());
    leftArm->GetComponent<cmp::Transform>()->SetPosition(-1, 4, 0);
    leftArm->AddComponent(std::make_shared<cmp::Name>("EnemyLeftArm"));

    root->AddChild(go)->AddChild(rightArm);
    go->GetNode()->AddChild(leftArm);

    enemyEnumerator++;
}