#pragma once

#include "Components.h"
#include "DoorActivator.h"
#include "Health.h"
#include "ShadowsManager.h"
#include "SoundPlayer.h"

enum CutsceneStages
{
    stage1, stage2, stage3, stage4, stage5, stage6, stage7, stage8, end
};
class Cutscenexd : public Script
{
public:

    DoorActivator* doorsToShut;
    std::shared_ptr<GameObject> doorsToOpen;
    std::shared_ptr<GameObject> blueprint;
    std::shared_ptr<cmp::Shader> lightShader;
    std::shared_ptr<cmp::Shader> dispShader;
    ShadowsManager* shadowManager;
    ResourceManager* resMan;
    CollidersManager* colMan;
    std::shared_ptr<cmp::Camera> camera;
    std::shared_ptr<GameObject> enemy;
    std::shared_ptr<GameObject> generator;
    std::shared_ptr<cmp::PointLight> generatorLight;
    std::shared_ptr<GameObject> shooter1;
    std::shared_ptr<GameObject> shooter2;
    std::shared_ptr<GameObject> shootingDoor;


private:
    std::shared_ptr<GameObject> darkGO;
    SoundPlayer* explode = new SoundPlayer("Resources/sounds/bigimpact.wav");
    SoundPlayer* doorClose = new SoundPlayer("Resources/sounds/metalimpact.wav");
    SoundPlayer* doorOpen = new SoundPlayer("Resources/sounds/dooropen.wav");
    std::shared_ptr<cmp::PointLight> dark;
    std::shared_ptr<GameObject> alarmLightGO1;
    std::shared_ptr<GameObject> alarmLightGO2;
    std::shared_ptr<cmp::SpotLight> lightCmp1;
    std::shared_ptr<cmp::SpotLight> lightCmp2;
    std::shared_ptr<cmp::Shader> tempGenShader;
    std::shared_ptr<GameObject> dummy;
    std::shared_ptr<cmp::Transform> dummyTrans;
    std::vector<std::shared_ptr<GameObject>> balls;
    std::vector<std::shared_ptr<GameObject>> doorBalls;
    int dummyWalkMod = 1;
    bool stage4Init = true;
    float ballSpawnTime = 1.0f;
    float ballSpawnTimer = 1.0f;
    float ballSpawnTime2 = 0.67f;
    float ballSpawnTimer2 = 0.67f;
    Health* enemyHealth;
    glm::mat4 rotationMat = glm::mat4(1.f);
    glm::mat4 newRotationMat;
    float rotation = 0.0f;
    float phase2Timer = 2.0f;
    float phase3Timer = 2.0f;
    float phase4Timer = 6.0f;
    float phase5Timer = 4.0f;
    float phase6Timer = 5.0f;
    float phase7Timer = 9.0f;
    

    bool hasStarted, hasFinished, lightUsed = false;
    ModelComponent* alarmModel;
    
    bool isFadingOutAnimation = true;
    CutsceneStages currentStage = stage1;
    SoundPlayer* siren;

    void RotateLights(float dt)
    {
        rotation = 360.0f * dt;
        newRotationMat = glm::rotate(rotationMat, glm::radians(rotation), glm::vec3(0, 0, 1));
        lightCmp1->SetDirection(glm::vec3(newRotationMat * glm::vec4(lightCmp1->GetDirection(), 1.0f)));
        lightCmp2->SetDirection(glm::vec3(newRotationMat * glm::vec4(lightCmp2->GetDirection(), 1.0f)));
    }

public:

    void Start()
    {
        hasStarted = false;
        hasFinished = false;

        if (enemy) enemyHealth = enemy->GetComponent<cmp::Scriptable>()->Get<Health>();
        siren = new SoundPlayer("Resources/sounds/emergency.wav");
        siren->SetVolume(0.2f);

        alarmModel = gameObject->GetNode()->GetParent()->FindNode("CutscenexdAlarmGO")->GetGameObject()->GetComponent<cmp::Model>().get();
    }

    void Update(float dt)
    {
        if (currentStage == stage2)
        {
            phase2Timer -= dt;
            if (phase2Timer <= 0)
            {
                currentStage = stage3;
                siren->SetVolume(0.6f);
                siren->Play();

                alarmLightGO1 = std::make_shared<GameObject>();
                gameObject->GetNode()->GetRoot()->FindNode("MAIN")->AddChild(alarmLightGO1);
                lightCmp1 = std::make_shared<cmp::SpotLight>();
                alarmLightGO1->AddComponent(lightCmp1);
                lightCmp1->Create();
                lightCmp1->AddShader(lightShader);
                lightCmp1->SetPosition({ -83.0f, 5.75f, 4.5f });
                lightCmp1->SetLightColor({ 1.0f, 0.0f, 0.0f });
                lightCmp1->SetDamping(20.0f);
                lightCmp1->SetCutOff({ 38.5f, 40.0f });
                lightCmp1->SetDirection({ 0, -1, 0 });

                alarmLightGO2 = std::make_shared<GameObject>();
                gameObject->GetNode()->GetRoot()->FindNode("MAIN")->AddChild(alarmLightGO2);
                lightCmp2 = std::make_shared<cmp::SpotLight>();
                alarmLightGO2->AddComponent(lightCmp2);
                lightCmp2->Create();
                lightCmp2->AddShader(lightShader);
                lightCmp2->SetPosition({ -83.0f, 5.75f, 4.5f });
                lightCmp2->SetLightColor({ 1.0f, 0.0f, 0.0f });
                lightCmp2->SetDamping(20.0f);
                lightCmp2->SetCutOff({ 60.0f, 70.5f });
                lightCmp2->SetDirection({ 0, 1, 0 });
                shadowManager->AddLight(alarmLightGO1.get());
                shadowManager->AddLight(alarmLightGO2.get());
            }
            /*
            float current = lightCmp1->GetLightColor().r;
            float current = lightCmp2->GetLightColor().r;
            if (current < 0.11f) isFadingOutAnimation = false;
            else if (current > 0.999f) isFadingOutAnimation = true;

            if (isFadingOutAnimation)
            {
                lightCmp1->SetLightColor({ current - dt, 0.0f, 0.0f });
                lightCmp2->SetLightColor({ current - dt, 0.0f, 0.0f });
            }
            else
            {
                lightCmp1->SetLightColor({ current + dt, 0.0f, 0.0f });
                lightCmp2->SetLightColor({ current + dt, 0.0f, 0.0f });
            }
            */
        }
        else if (currentStage == stage3)
        {
            RotateLights(dt);
            phase3Timer -= dt;
            if (phase3Timer <= 0)
            {
                currentStage = stage4;
            }
        }
        else if (currentStage == stage4)
        {
            RotateLights(dt);
            if (stage4Init)
            {
                generatorLight->SetPosition(generator->GetComponent<cmp::Transform>()->GetPosition());
                generator->GetComponent<cmp::Shader>() = tempGenShader;
                dummy = std::make_shared<GameObject>();
                dummy->AddComponent(std::make_shared<cmp::Transform>());
                dummyTrans = dummy->GetComponent<cmp::Transform>();
                dummyTrans->SetPosition({ -100,2,25 });
                dummy->AddComponent(std::make_shared<BoxCollider>(true, false, CollisionLayer::ENEMY));
                dummy->GetComponent<cmp::BoxCol>()->SetLengths(glm::vec3(2, 2, 2));
                dummy->GetComponent<cmp::BoxCol>()->isOptimized = false;
                dummy->GetComponent<cmp::BoxCol>()->AddToCollidersManager(colMan);

                dummy->AddComponent(std::make_shared<cmp::Scriptable>());
                Health* health = new Health();
                health->SetMaxHealth(1000.0f);
                dummy->GetComponent<cmp::Scriptable>()->Add(health);
                gameObject->GetNode()->GetRoot()->FindNode("MAIN")->AddChild(dummy);
                stage4Init = false;
            }
            phase4Timer -= dt;
            dummyTrans->Move({ 0,0,2.0f * dt * dummyWalkMod });
            if (dummyTrans->GetPosition().z >= 30)
            {
                dummyWalkMod = -1;
            }
            else if (dummyTrans->GetPosition().z <= 20)
            {
                dummyWalkMod = 1;
            }
            if (phase4Timer <= 0)
            {
                currentStage = stage5;
            }
        }
        else if (currentStage == stage5)
        {
            RotateLights(dt);
            dummyTrans->Move({ 0,0,2.0f * dt * dummyWalkMod });
            if (dummyTrans->GetPosition().z >= 30)
            {
                dummyWalkMod = -1;
            }
            else if (dummyTrans->GetPosition().z <= 20)
            {
                dummyWalkMod = 1;
            }
            ballSpawnTimer += dt;
            if (ballSpawnTimer >= ballSpawnTime)
            {
                for (int i = 0; i < 2; i++)
                {
                    balls.push_back(std::make_shared<GameObject>());
                    balls.back()->AddComponent(std::make_shared<cmp::Transform>());
                    std::shared_ptr<cmp::Transform> shooterTrans = nullptr;
                    if (i == 0)
                    {
                        shooterTrans = shooter1->GetComponent<cmp::Transform>();
                    }
                    else
                    {
                        shooterTrans = shooter2->GetComponent<cmp::Transform>();
                    }
                    balls.back()->GetComponent<cmp::Transform>()->SetPosition(shooterTrans->GetPosition().x - 4, shooterTrans->GetPosition().y, shooterTrans->GetPosition().z);
                    balls.back()->GetComponent<cmp::Transform>()->SetScale(0.1);
                    std::shared_ptr<ModelComponent> mc = std::make_shared<ModelComponent>();
                    mc->Create(
                        //sceneInfo.resourceManager->GetMesh("Resources/models/ny/przeciwnik/przeciwnik/przeciwniksmooth.obj"),
                        resMan->GetMesh("Resources/models/Sphere/Sphere.obj"),
                        resMan->GetMaterial("Resources/models/displacement test/capsule.mtl")
                    );
                    balls.back()->AddComponent(dispShader);
                    balls.back()->AddComponent(mc);

                    balls.back()->AddComponent(std::make_shared<FrustumCullingComponent>());
                    balls.back()->GetComponent<cmp::FrustumCulling>()->Create(
                        resMan->GetMesh("Resources/models/Sphere/Sphere.obj")
                    );
                    gameObject->GetNode()->GetRoot()->FindNode("MAIN")->AddChild(balls.back());
                }
                ballSpawnTimer = 0;
            }
            for (std::shared_ptr<GameObject> ball : balls)
            {
                std::shared_ptr<cmp::Transform> balltrans = ball->GetComponent<cmp::Transform>();
                balltrans->Move({ 1 * dt,0,0 });
                balltrans->SetScale(balltrans->GetScale() + 1 * dt);
            }
            phase5Timer -= dt;
            if (phase5Timer <= 0)
            {
                currentStage = stage6;
                shooter1->GetComponent<cmp::Scriptable>()->DisableAll();
                shooter2->GetComponent<cmp::Scriptable>()->DisableAll();
                dummy->GetComponent<cmp::Scriptable>()->Get<Health>()->DecreaseHealth(1000);
                shootingDoor->GetComponent<cmp::Transform>()->Move(0, -6, 0);
                doorClose->Play();
                std::cout << shootingDoor->GetComponent<cmp::Transform>()->GetPosition().x << " " << shootingDoor->GetComponent<cmp::Transform>()->GetPosition().y << " " << shootingDoor->GetComponent<cmp::Transform>()->GetPosition().z << std::endl;
                balls.clear();
            }

        }
        else if (currentStage == stage6)
        {
            RotateLights(dt);
            phase6Timer -= dt;
            if (phase6Timer <= 0)
            {
                for (int i = 0; i < 2; i++)
                {
                    doorBalls.push_back(std::make_shared<GameObject>());
                    doorBalls.back()->AddComponent(std::make_shared<cmp::Transform>());
                    std::shared_ptr<cmp::Transform> doorTrans = doorsToOpen->GetComponent<cmp::Transform>();
                    doorBalls.back()->GetComponent<cmp::Transform>()->SetPosition(doorTrans->GetPosition().x + (-1 + 2 * i), 0, doorTrans->GetPosition().z - 4);
                    doorBalls.back()->GetComponent<cmp::Transform>()->SetScale(4);
                    std::shared_ptr<ModelComponent> mc = std::make_shared<ModelComponent>();
                    mc->Create(
                        //sceneInfo.resourceManager->GetMesh("Resources/models/ny/przeciwnik/przeciwnik/przeciwniksmooth.obj"),
                        resMan->GetMesh("Resources/models/Sphere/Sphere.obj"),
                        resMan->GetMaterial("Resources/models/displacement test/capsule.mtl")
                    );
                    doorBalls.back()->AddComponent(dispShader);
                    doorBalls.back()->AddComponent(mc);

                    doorBalls.back()->AddComponent(std::make_shared<FrustumCullingComponent>());
                    doorBalls.back()->GetComponent<cmp::FrustumCulling>()->Create(
                        resMan->GetMesh("Resources/models/Sphere/Sphere.obj")
                    );
                    gameObject->GetNode()->GetRoot()->FindNode("MAIN")->AddChild(doorBalls.back());
                }
                doorsToOpen->GetComponent<cmp::Transform>()->Move(0, 3, 0);
                doorOpen->Play();
                currentStage = stage7;
            }
        }
        else if (currentStage == stage7)
        {
            RotateLights(dt);
            ballSpawnTimer2 += dt;
            if (ballSpawnTimer2 >= ballSpawnTime2)
            {
                for (int i = 0; i < 4; i++)
                {
                    balls.push_back(std::make_shared<GameObject>());
                    balls.back()->AddComponent(std::make_shared<cmp::Transform>());
                    std::shared_ptr<cmp::Transform> doorTrans = doorsToOpen->GetComponent<cmp::Transform>();
                    balls.back()->GetComponent<cmp::Transform>()->SetPosition(doorTrans->GetPosition().x + (-3 + 2 * i), 0, doorTrans->GetPosition().z - 4);
                    balls.back()->GetComponent<cmp::Transform>()->SetScale(2);
                    std::shared_ptr<ModelComponent> mc = std::make_shared<ModelComponent>();
                    mc->Create(
                        //sceneInfo.resourceManager->GetMesh("Resources/models/ny/przeciwnik/przeciwnik/przeciwniksmooth.obj"),
                        resMan->GetMesh("Resources/models/Sphere/Sphere.obj"),
                        resMan->GetMaterial("Resources/models/displacement test/capsule.mtl")
                    );
                    balls.back()->AddComponent(dispShader);
                    balls.back()->AddComponent(mc);

                    balls.back()->AddComponent(std::make_shared<FrustumCullingComponent>());
                    balls.back()->GetComponent<cmp::FrustumCulling>()->Create(
                        resMan->GetMesh("Resources/models/Sphere/Sphere.obj")
                    );
                    gameObject->GetNode()->GetRoot()->FindNode("MAIN")->AddChild(balls.back());
                }
                ballSpawnTimer2 = 0;
            }
            for (std::shared_ptr<GameObject> ball : balls)
            {
                std::shared_ptr<cmp::Transform> balltrans = ball->GetComponent<cmp::Transform>();
                balltrans->Move({0,0,3*dt });
            }
            phase7Timer -= dt;
            if (phase7Timer <= 0)
            {
                currentStage = stage8;
            }
        }
        else if (currentStage == stage8)
        {
            RotateLights(dt);
            enemy->GetComponent<cmp::Transform>()->Move(0, 3 * dt, 0);
            if (enemy->GetComponent<cmp::Transform>()->GetPosition().y >= 2)
            {
                enemy->GetComponent<cmp::BoxCol>()->AddToCollidersManager(colMan);
                enemy->GetComponent<cmp::Scriptable>()->EnableAll();
                currentStage = end;
            }
        }
        else if (currentStage == end)
        {
            if (!lightUsed)
            {
                gameObject->GetNode()->GetRoot()->FindNode("TutorialFlash")->GetGameObject()->GetComponent<cmp::Transform>()->SetScale(0.1);
            }
            if (Input()->Keyboard()->IsPressed(KeyboardKey::T))
            {
                gameObject->GetNode()->GetRoot()->FindNode("TutorialFlash")->GetGameObject()->GetComponent<cmp::Transform>()->SetScale(0.0);
                lightUsed = true;
                camera->GetOwner()->GetComponent<cmp::Scriptable>()->Get<GameManager>()->immortal = false;
            }
            RotateLights(dt);
            if (enemy && enemy->IsDestroyed() && !hasFinished)
            {
                EndCutscene();
            }
        }
    }

    void EndCutscene()
    {
        hasFinished = true;
        AudioManager::ResumeQueue();

        lightCmp1->SetLightColor({0,1,0});
        lightCmp2->SetLightColor({ 0,1,0 });
        dark->SetLightColor({ 0.001f,0,0 });
        gameObject->GetNode()->GetRoot()->FindNode("Flashlight")->GetGameObject()->GetComponent<cmp::SpotLight>()->SetLightColor({ 0.25f, 0.02f, 0.58f });
        for (std::shared_ptr<GameObject> ball : balls)
        {
            ball->GetNode()->GetParent()->RemoveNode(ball->GetNode());
        }
        balls.clear();
        for (std::shared_ptr<GameObject> ball : doorBalls)
        {
            ball->GetNode()->GetParent()->RemoveNode(ball->GetNode());
        }
        doorBalls.clear();
        doorsToOpen->GetComponent<cmp::Transform>()->Move(0, 3, 0);
        doorOpen->Play();
        alarmModel->SetTintColor(0.01, 0.51, 0.01);
    }

    void TriggerEnter(std::shared_ptr<ColliderComponent> other) override
    {
        if (other->layer & CollisionLayer::Player && !hasStarted && blueprint->GetComponent<cmp::Transform>()->GetPosition() == glm::vec3(0, -100, 0))
        {
            // Player entered trigger
            hasStarted = true;
            currentStage = stage2;
            gameObject->GetNode()->GetRoot()->FindNode("MAIN")->FindNode("Flashlight")->GetGameObject()->GetComponent<cmp::SpotLight>()->SetLightColor({1.0f, 0.0f, 1.0f});

            darkGO = std::make_shared<GameObject>();
            gameObject->GetNode()->GetRoot()->FindNode("MAIN")->AddChild(darkGO);
            dark = std::make_shared<cmp::PointLight>();
            darkGO->AddComponent(dark);
            dark->Create();
            dark->AddShader(lightShader);
            dark->SetPosition({ -83.0f, 5.75f, 0.0f });
            dark->SetLightColor({ 0.0f, 0.0f, 0.0f });
            dark->SetDamping(100.0f);
            tempGenShader = generator->GetComponent<cmp::Shader>();
            generator->GetComponent<cmp::Shader>() = lightShader;
            AudioManager::PauseQueue();
            explode->SetVolume(0.6f);
            explode->Play();
            doorsToShut->ForceShutdown();
            camera->ShakeCamera(2.0f, 2.0f, 4.0f);
            generatorLight->SetPosition({ -999, -999, -999 });

            alarmModel->SetTintColor(0.51, 0.01, 0.01);
        }
    }

    void turnOffLight()
    {
        lightCmp1->SetDamping(1.0f);
        lightCmp1->SetPosition(glm::vec3(999.99f, 999.99f, 999.99f));
        lightCmp2->SetDamping(1.0f);
        lightCmp2->SetPosition(glm::vec3(999.99f, 999.99f, 999.99f));
    }
};