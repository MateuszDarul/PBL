#include "EnemySpawnerScript.h"
#include "ResourceManager.h"
#include "GameApplication.h"
#include "EnemyScript.h"
#include "Components.h"
#include "Scripts/Health.h"

void EnemySpawnerScript::Start() {

    SpawnEnemy(1);

}

void EnemySpawnerScript::Update(float dt) {

}

void EnemySpawnerScript::SpawnEnemy(int nr)
{
    ResourceManager* resMan = GameApplication::GetResourceManager();

    std::shared_ptr<GameObject> go = std::make_shared<GameObject>();
    std::shared_ptr<ModelComponent> mc = std::make_shared<ModelComponent>();
    mc->Create(
        resMan->GetMesh("Resources/models/ny/przeciwnik/przeciwnik/przeciwniksmooth.obj"),
        resMan->GetMaterial("Resources/models/displacement test/capsule.mtl")
    );
    go->AddComponent(displShader);
    go->AddComponent(mc);

    go->AddComponent(std::make_shared<FrustumCullingComponent>());
    go->GetComponent<cmp::FrustumCulling>()->Create(
        resMan->GetMesh("Resources/models/ny/przeciwnik/przeciwnik/przeciwniksmooth.obj")
    );

    go->AddComponent(std::make_shared<cmp::Transform>());
    go->GetComponent<cmp::Transform>()->SetRotation(0, 90, 0);

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

    std::shared_ptr<Path> path = std::make_shared<Path>(true);
    path->Clear();

    for (int i = 0; i < wayPoints.size(); i++)
    {
        path->AddWayPoint(wayPoints.at(i));
    }

    float randomDispl = 3.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (-3.0f)));

    go->GetComponent<cmp::Transform>()->SetPosition(wayPoints.at(0) + glm::vec3(randomDispl, randomDispl, randomDispl));
    path->Set();

    auto enemyScript = new EnemyScript(playerGO);
    enemyScript->health = health;
    enemyScript->multitool = multiToolScript;
    enemyScript->SetPath(path);
    go->GetComponent<cmp::Scriptable>()->Add(enemyScript);

    root->AddChild(go);

    go->GetComponent<cmp::Scriptable>()->OnStart();

    enemyEnumerator++;
}