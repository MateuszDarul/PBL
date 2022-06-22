#ifndef __MENU_H__
#define __MENU_H__

#include "SceneNode.h"
#include "AudioManager.h"

class MenuScene
{
protected:
    SceneNode* menu;
    std::shared_ptr<ShaderComponent> shader_d;
    glm::vec2 prev_pos;
    int begin = 0;
    float maxCursorOffset = 2.25f;

protected:
    void UpdateMenu()
    {
        menu->Update(1);
        glm::vec2 pos = GetCursorOffser(GameApplication::GetInputManager()->Mouse()->GetPosition());
        menu->FindNode("CURSOR")->GetLocalTransformations()->Move(glm::vec3(pos.x, pos.y, 0));

        auto m = menu->FindNode("CURSOR")->GetLocalTransformations()->GetPosition();
        if (m.y < -maxCursorOffset) m.y = -maxCursorOffset;
        else if (m.y > maxCursorOffset) m.y = maxCursorOffset;
        if (m.x < -maxCursorOffset * GameApplication::GetAspectRatio()) m.x = -maxCursorOffset * GameApplication::GetAspectRatio();
        else if (m.x > maxCursorOffset * GameApplication::GetAspectRatio()) m.x = maxCursorOffset * GameApplication::GetAspectRatio();
        menu->FindNode("CURSOR")->GetLocalTransformations()->SetPosition(m);
    }

    void PrepareMenu()
    {
        std::shared_ptr<GameObject> go;
        std::shared_ptr<cmp::Model> mc;
        
        go = std::make_shared<GameObject>();
        go->AddComponent(std::make_shared<cmp::Name>("Menu_root"));
        menu = new SceneNode(go);
        go = std::make_shared<GameObject>();
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->AddComponent(std::make_shared<NameComponent>("CAMERA"));
        go->AddComponent(std::make_shared<CameraComponent>());
        go->GetComponent<cmp::Camera>()->Create(glm::vec3(0,0,0));
        go->GetComponent<cmp::Camera>()->SetSpeed(0);
        menu->AddChild(go);
    }

    void FinishMenu()
    {
        ResourceManager* resMan = GameApplication::GetResourceManager();

        std::shared_ptr<GameObject> go;
        std::shared_ptr<cmp::Model> mc;

        go = std::make_shared<GameObject>();
        mc = std::make_shared<cmp::Model>();
        mc->Create(
        resMan->GetMesh("Resources/models/menu/Cursor.obj"),
        resMan->GetMaterial("Resources/models/menu/Cursor.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_d);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(0.1, 0.0, -2.99);
        go->AddComponent(std::make_shared<cmp::BoxCol>(true, false));
        go->GetComponent<cmp::BoxCol>()->SetLengths(glm::vec3(0, 0, 10));
        go->GetComponent<cmp::BoxCol>()->SetOffset(glm::vec3(-0.1, 0.1, 0));
        go->AddComponent(std::make_shared<cmp::Name>("CURSOR"));
        menu->AddChild(go);
        
        
    }

    glm::vec2 GetCursorOffser(glm::vec2 pos)
    {
        glm::vec2 offset = pos - prev_pos;
        prev_pos = pos;
        
        if(begin < 5)
        {
            begin++;
            return glm::vec2(0,0);
        }

        offset /= 150;
        offset.y *= -1;

        return offset;
    }

public:
    MenuScene()
    {
        glfwSetInputMode(GameApplication::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        prev_pos = glm::vec2(-9999, -9999);
    }

    virtual ~MenuScene()
    {
        delete menu;
    }

    void Draw()
    {
        std::shared_ptr<GameObject> goCamera = menu->FindNode("CAMERA")->GetGameObject();
        menu->Render(GameApplication::GetProjection() * goCamera->GetComponent<CameraComponent>()->GetView());
    }

    virtual void Update()=0;
};
void ChangeMenu(std::string newMenu);

class MainMenu :public MenuScene
{
public:

    MainMenu()
    {
        PrepareMenu();
        ///***
        ResourceManager* resMan = GameApplication::GetResourceManager();

        shader_d = std::make_shared<ShaderComponent>();
        shader_d->Create("Resources/shaders/default.vert", "Resources/shaders/default.frag");
        shader_d->Use();
        shader_d->SetFloat("brightness", GameApplication::GetBright());
        shader_d->SetFloat("gamma", GameApplication::GetGamma());
        shader_d->SetFloat("contrast", GameApplication::GetContrast());

        std::shared_ptr<GameObject> go;
        std::shared_ptr<cmp::Model> mc;

        go = std::make_shared<GameObject>();
        mc = std::make_shared<cmp::Model>();
        mc->Create(
        resMan->GetMesh("Resources/models/menu/Button.obj"),
        resMan->GetMaterial("Resources/models/menu/Button_NEW_GAME.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_d);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(1.0, 1.5, -3.0);
        go->AddComponent(std::make_shared<cmp::BoxCol>(true, true));
        go->GetComponent<cmp::BoxCol>()->SetLengths(glm::vec3(2, 0.6, 10));
        go->AddComponent(std::make_shared<cmp::Name>("Menu_b1"));
        menu->AddChild(go);

        go = std::make_shared<GameObject>();
        mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/menu/Button.obj"),
            resMan->GetMaterial("Resources/models/menu/Button_SETTINGS.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_d);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(1.0, 0.5, -3.0);
        go->AddComponent(std::make_shared<cmp::BoxCol>(true, true));
        go->GetComponent<cmp::BoxCol>()->SetLengths(glm::vec3(2, 0.6, 10));
        go->AddComponent(std::make_shared<cmp::Name>("Menu_b2"));
        menu->AddChild(go);

        go = std::make_shared<GameObject>();
        mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/menu/Button.obj"),
            resMan->GetMaterial("Resources/models/menu/Button_CREDITS.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_d);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(1.0, -0.5, -3.0);
        go->AddComponent(std::make_shared<cmp::BoxCol>(true, true));
        go->GetComponent<cmp::BoxCol>()->SetLengths(glm::vec3(2, 0.6, 10));
        go->AddComponent(std::make_shared<cmp::Name>("Menu_b4"));
        menu->AddChild(go);

        go = std::make_shared<GameObject>();
        mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/menu/Button.obj"),
            resMan->GetMaterial("Resources/models/menu/Button_EXIT.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_d);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(1.0, -1.5, -3.0);
        go->AddComponent(std::make_shared<cmp::BoxCol>(true, true));
        go->GetComponent<cmp::BoxCol>()->SetLengths(glm::vec3(2, 0.6, 10));
        go->AddComponent(std::make_shared<cmp::Name>("Menu_b3"));
        menu->AddChild(go);

        go = std::make_shared<GameObject>();
        mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/menu/Cursor.obj"),
            resMan->GetMaterial("Resources/models/menu/Button_LOGO.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_d);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(-1.5, 0.0f, -3.0);
        go->GetComponent<cmp::Transform>()->SetScale(25.0f);
        go->AddComponent(std::make_shared<cmp::Name>("Menu_logo"));
        menu->AddChild(go);
        ///***
        FinishMenu();
    }

    ~MainMenu()
    {

    }

    void Update()
    {
        UpdateMenu();
        if(GameApplication::GetInputManager()->Mouse()->OnPressed(MouseButton::Left_MB))
        {
            if(menu->FindNode("CURSOR")->GetGameObject()->GetComponent<cmp::BoxCol>()->CheckCollision(
                menu->FindNode("Menu_b1")->GetGameObject()->GetComponent<cmp::BoxCol>()))
            {
                ChangeMenu("LevelsMenu");
            }

            else if (menu->FindNode("CURSOR")->GetGameObject()->GetComponent<cmp::BoxCol>()->CheckCollision(
                menu->FindNode("Menu_b2")->GetGameObject()->GetComponent<cmp::BoxCol>()))
            {
                ChangeMenu("SettingsMenu");
            }

            else if (menu->FindNode("CURSOR")->GetGameObject()->GetComponent<cmp::BoxCol>()->CheckCollision(
                menu->FindNode("Menu_b3")->GetGameObject()->GetComponent<cmp::BoxCol>()))
            {
                glfwSetWindowShouldClose(GameApplication::GetWindow(), GLFW_TRUE);
            }

            else if (menu->FindNode("CURSOR")->GetGameObject()->GetComponent<cmp::BoxCol>()->CheckCollision(
                menu->FindNode("Menu_b4")->GetGameObject()->GetComponent<cmp::BoxCol>()))
            {
                ChangeMenu("CreditsMenu");
            }
        }
    }
};

class LevelsMenu :public MenuScene
{
public:
    LevelsMenu()
    {
        PrepareMenu();
        ///***
        ResourceManager* resMan = GameApplication::GetResourceManager();

        shader_d = std::make_shared<ShaderComponent>();
        shader_d->Create("Resources/shaders/default.vert", "Resources/shaders/default.frag");
        shader_d->Use();
        shader_d->SetFloat("brightness", GameApplication::GetBright());
        shader_d->SetFloat("gamma", GameApplication::GetGamma());
        shader_d->SetFloat("contrast", GameApplication::GetContrast());

        std::shared_ptr<GameObject> go;
        std::shared_ptr<cmp::Model> mc;


        go = std::make_shared<GameObject>();
        mc = std::make_shared<cmp::Model>();
        mc->Create(
        resMan->GetMesh("Resources/models/menu/Button.obj"),
        resMan->GetMaterial("Resources/models/menu/Button_PLAY.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_d);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(0.0, 0.0, -3.0);
        go->AddComponent(std::make_shared<cmp::BoxCol>(true, true));
        go->GetComponent<cmp::BoxCol>()->SetLengths(glm::vec3(2, 0.6, 10));
        go->AddComponent(std::make_shared<cmp::Name>("Menu_b1"));
        menu->AddChild(go);

        go = std::make_shared<GameObject>();
        mc = std::make_shared<cmp::Model>();
        mc->Create(
        resMan->GetMesh("Resources/models/menu/Button.obj"),
        resMan->GetMaterial("Resources/models/menu/Button_BACK.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_d);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(0.0, -1.0, -3.0);
        go->AddComponent(std::make_shared<cmp::BoxCol>(true, true));
        go->GetComponent<cmp::BoxCol>()->SetLengths(glm::vec3(2, 0.6, 10));
        go->AddComponent(std::make_shared<cmp::Name>("Menu_b2"));
        menu->AddChild(go);
        ///***
        FinishMenu();
    }

    ~LevelsMenu()
    {
        
    }

    void Update()
    {
        UpdateMenu();
        if(GameApplication::GetInputManager()->Mouse()->OnPressed(MouseButton::Left_MB))
        {
            if(menu->FindNode("CURSOR")->GetGameObject()->GetComponent<cmp::BoxCol>()->CheckCollision(
                menu->FindNode("Menu_b1")->GetGameObject()->GetComponent<cmp::BoxCol>()))
            {
                GameApplication::inGame = true;
            }
            else if(menu->FindNode("CURSOR")->GetGameObject()->GetComponent<cmp::BoxCol>()->CheckCollision(
                menu->FindNode("Menu_b2")->GetGameObject()->GetComponent<cmp::BoxCol>()))
            {
                ChangeMenu("MainMenu");
            }
        }
    }
};

class SettingsMenu :public MenuScene
{
public:
    SettingsMenu()
    {
        PrepareMenu();
        ///***
        ResourceManager* resMan = GameApplication::GetResourceManager();

        shader_d = std::make_shared<ShaderComponent>();
        shader_d->Create("Resources/shaders/default.vert", "Resources/shaders/default.frag");
        shader_d->Use();
        shader_d->SetFloat("brightness", GameApplication::GetBright());
        shader_d->SetFloat("gamma", GameApplication::GetGamma());
        shader_d->SetFloat("contrast", GameApplication::GetContrast());

        std::shared_ptr<GameObject> go;
        std::shared_ptr<cmp::Model> mc;

        //bright
        go = std::make_shared<GameObject>();
        mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/menu/Button.obj"),
            resMan->GetMaterial("Resources/models/menu/Button_BRIGHT.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_d);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(0.0, 1.5, -3.0);
        go->AddComponent(std::make_shared<cmp::Name>("Menu_BrightText"));
        menu->AddChild(go);

        //add bright
        go = std::make_shared<GameObject>();
        mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/menu/Cursor.obj"),
            resMan->GetMaterial("Resources/models/menu/Button_ADD.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_d);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(1.2, 1.5, -3.0);
        go->AddComponent(std::make_shared<cmp::BoxCol>(true, true));
        go->GetComponent<cmp::BoxCol>()->SetLengths(glm::vec3(0.6, 0.6, 10));
        go->AddComponent(std::make_shared<cmp::Name>("Menu_brightnessAdd"));
        menu->AddChild(go);

        //sub bright
        go = std::make_shared<GameObject>();
        mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/menu/Cursor.obj"),
            resMan->GetMaterial("Resources/models/menu/Button_SUB.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_d);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(-1.2, 1.5, -3.0);
        go->AddComponent(std::make_shared<cmp::BoxCol>(true, true));
        go->GetComponent<cmp::BoxCol>()->SetLengths(glm::vec3(0.6, 0.6, 10));
        go->AddComponent(std::make_shared<cmp::Name>("Menu_brightnessSub"));
        menu->AddChild(go);

        //gamma
        go = std::make_shared<GameObject>();
        mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/menu/Button.obj"),
            resMan->GetMaterial("Resources/models/menu/Button_GAMMA.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_d);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(0.0, 0.5, -3.0);
        go->AddComponent(std::make_shared<cmp::Name>("Menu_GammaText"));
        menu->AddChild(go);

        //add gamma
        go = std::make_shared<GameObject>();
        mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/menu/Cursor.obj"),
            resMan->GetMaterial("Resources/models/menu/Button_ADD.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_d);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(1.2, 0.5, -3.0);
        go->AddComponent(std::make_shared<cmp::BoxCol>(true, true));
        go->GetComponent<cmp::BoxCol>()->SetLengths(glm::vec3(0.6, 0.6, 10));
        go->AddComponent(std::make_shared<cmp::Name>("Menu_gammaAdd"));
        menu->AddChild(go);

        //sub gamma
        go = std::make_shared<GameObject>();
        mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/menu/Cursor.obj"),
            resMan->GetMaterial("Resources/models/menu/Button_SUB.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_d);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(-1.2, 0.5, -3.0);
        go->AddComponent(std::make_shared<cmp::BoxCol>(true, true));
        go->GetComponent<cmp::BoxCol>()->SetLengths(glm::vec3(0.6, 0.6, 10));
        go->AddComponent(std::make_shared<cmp::Name>("Menu_gammaSub"));
        menu->AddChild(go);

        //contrast
        go = std::make_shared<GameObject>();
        mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/menu/Button.obj"),
            resMan->GetMaterial("Resources/models/menu/Button_CONTRAST.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_d);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(0.0, -0.5, -3.0);
        go->AddComponent(std::make_shared<cmp::Name>("Menu_ContrastText"));
        menu->AddChild(go);

        //add contrast
        go = std::make_shared<GameObject>();
        mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/menu/Cursor.obj"),
            resMan->GetMaterial("Resources/models/menu/Button_ADD.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_d);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(1.2, -0.5, -3.0);
        go->AddComponent(std::make_shared<cmp::BoxCol>(true, true));
        go->GetComponent<cmp::BoxCol>()->SetLengths(glm::vec3(0.6, 0.6, 10));
        go->AddComponent(std::make_shared<cmp::Name>("Menu_contrastAdd"));
        menu->AddChild(go);

        //sub contrast
        go = std::make_shared<GameObject>();
        mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/menu/Cursor.obj"),
            resMan->GetMaterial("Resources/models/menu/Button_SUB.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_d);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(-1.2, -0.5, -3.0);
        go->AddComponent(std::make_shared<cmp::BoxCol>(true, true));
        go->GetComponent<cmp::BoxCol>()->SetLengths(glm::vec3(0.6, 0.6, 10));
        go->AddComponent(std::make_shared<cmp::Name>("Menu_contrastSub"));
        menu->AddChild(go);

        //music
        go = std::make_shared<GameObject>();
        mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/menu/Cursor.obj"),
            resMan->GetMaterial("Resources/models/menu/Button_SPEAKER.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_d);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(2.0, 1.5, -3.0);
        go->AddComponent(std::make_shared<cmp::BoxCol>(true, true));
        go->GetComponent<cmp::BoxCol>()->SetLengths(glm::vec3(0.6, 0.6, 10));
        go->AddComponent(std::make_shared<cmp::Name>("Menu_music"));
        if (AudioManager::IsMusicPlaying()) mc->SetTintColor(1.0f, 1.0f, 1.0f);
        else mc->SetTintColor(0.5f, 0.5f, 0.5f);
        menu->AddChild(go);

        //go back
        go = std::make_shared<GameObject>();
        mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/menu/Button.obj"),
            resMan->GetMaterial("Resources/models/menu/Button_BACK.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_d);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(0.0, -1.5, -3.0);
        go->AddComponent(std::make_shared<cmp::BoxCol>(true, true));
        go->GetComponent<cmp::BoxCol>()->SetLengths(glm::vec3(2, 0.6, 10));
        go->AddComponent(std::make_shared<cmp::Name>("Menu_back"));
        menu->AddChild(go);
        ///***
        FinishMenu();
    }

    ~SettingsMenu()
    {

    }

    void Update()
    {
        UpdateMenu();
        if (GameApplication::GetInputManager()->Mouse()->OnPressed(MouseButton::Left_MB))
        {
            if (menu->FindNode("CURSOR")->GetGameObject()->GetComponent<cmp::BoxCol>()->CheckCollision(
                menu->FindNode("Menu_brightnessAdd")->GetGameObject()->GetComponent<cmp::BoxCol>()))
            {
                GameApplication::SetBright(GameApplication::GetBright() + 0.05f);
            }
            else 
            if (menu->FindNode("CURSOR")->GetGameObject()->GetComponent<cmp::BoxCol>()->CheckCollision(
                menu->FindNode("Menu_brightnessSub")->GetGameObject()->GetComponent<cmp::BoxCol>()))
            {
                GameApplication::SetBright(GameApplication::GetBright() - 0.05f);
            }
            else 
            if (menu->FindNode("CURSOR")->GetGameObject()->GetComponent<cmp::BoxCol>()->CheckCollision(
                menu->FindNode("Menu_gammaAdd")->GetGameObject()->GetComponent<cmp::BoxCol>()))
            {
                GameApplication::SetGamma(GameApplication::GetGamma() + 0.05f);
            }
            else
            if (menu->FindNode("CURSOR")->GetGameObject()->GetComponent<cmp::BoxCol>()->CheckCollision(
                menu->FindNode("Menu_gammaSub")->GetGameObject()->GetComponent<cmp::BoxCol>()))
            {
                GameApplication::SetGamma(GameApplication::GetGamma() - 0.05f);
            }
            else
            if (menu->FindNode("CURSOR")->GetGameObject()->GetComponent<cmp::BoxCol>()->CheckCollision(
                menu->FindNode("Menu_contrastAdd")->GetGameObject()->GetComponent<cmp::BoxCol>()))
            {
                GameApplication::SetContrast(GameApplication::GetContrast() + 0.05f);
                std::cout << GameApplication::GetContrastWithMod() << std::endl;
            }
            else
            if (menu->FindNode("CURSOR")->GetGameObject()->GetComponent<cmp::BoxCol>()->CheckCollision(
                menu->FindNode("Menu_contrastSub")->GetGameObject()->GetComponent<cmp::BoxCol>()))
            {
                GameApplication::SetContrast(GameApplication::GetContrast() - 0.05f);
            }
            else
            if (menu->FindNode("CURSOR")->GetGameObject()->GetComponent<cmp::BoxCol>()->CheckCollision(
                menu->FindNode("Menu_music")->GetGameObject()->GetComponent<cmp::BoxCol>()))
            {
                AudioManager::ToggleMusic();
                auto mc = menu->FindNode("Menu_music")->GetGameObject()->GetComponent<cmp::Model>();
                if (AudioManager::IsMusicPlaying()) mc->SetTintColor(1.0f, 1.0f, 1.0f);
                else mc->SetTintColor(0.5f, 0.5f, 0.5f);
            }
            else 
            if (menu->FindNode("CURSOR")->GetGameObject()->GetComponent<cmp::BoxCol>()->CheckCollision(
                menu->FindNode("Menu_back")->GetGameObject()->GetComponent<cmp::BoxCol>()))
            {
                ChangeMenu("MainMenu");
            }
        }
    }
};

class CreditsMenu :public MenuScene
{
public:
    CreditsMenu()
    {
        PrepareMenu();
        ///***
        ResourceManager* resMan = GameApplication::GetResourceManager();

        shader_d = std::make_shared<ShaderComponent>();
        shader_d->Create("Resources/shaders/default.vert", "Resources/shaders/default.frag");
        shader_d->Use();
        shader_d->SetFloat("brightness", GameApplication::GetBright());
        shader_d->SetFloat("gamma", GameApplication::GetGamma());
        shader_d->SetFloat("contrast", GameApplication::GetContrast());

        std::shared_ptr<GameObject> go;
        std::shared_ptr<cmp::Model> mc;

        go = std::make_shared<GameObject>();
        mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/menu/Cursor.obj"),
            resMan->GetMaterial("Resources/models/menu/Button_CREDITSTEXT.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_d);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(0.0, 0.5f, -3.0);
        go->GetComponent<cmp::Transform>()->SetScale(15.0f);
        go->AddComponent(std::make_shared<cmp::Name>("Menu_creditsText"));
        menu->AddChild(go);

        go = std::make_shared<GameObject>();
        mc = std::make_shared<cmp::Model>();
        mc->Create(
            resMan->GetMesh("Resources/models/menu/Button.obj"),
            resMan->GetMaterial("Resources/models/menu/Button_BACK.mtl")
        );
        go->AddComponent(mc);
        go->AddComponent(shader_d);
        go->AddComponent(std::make_shared<cmp::Transform>());
        go->GetComponent<cmp::Transform>()->SetPosition(0.0, -1.5, -3.0);
        go->AddComponent(std::make_shared<cmp::BoxCol>(true, true));
        go->GetComponent<cmp::BoxCol>()->SetLengths(glm::vec3(2, 0.6, 10));
        go->AddComponent(std::make_shared<cmp::Name>("Menu_b2"));
        menu->AddChild(go);
        ///***
        FinishMenu();
    }

    ~CreditsMenu()
    {

    }

    void Update()
    {
        UpdateMenu();
        if (GameApplication::GetInputManager()->Mouse()->OnPressed(MouseButton::Left_MB))
        {
            if (menu->FindNode("CURSOR")->GetGameObject()->GetComponent<cmp::BoxCol>()->CheckCollision(
                menu->FindNode("Menu_b2")->GetGameObject()->GetComponent<cmp::BoxCol>()))
            {
                ChangeMenu("MainMenu");
            }
        }
    }
};

void ChangeMenu(std::string newMenu)
{
    if(newMenu == "LevelsMenu")
    {
        delete GameApplication::s_Menu;
        GameApplication::s_Menu = new LevelsMenu();
    }
    else if (newMenu == "SettingsMenu")
    {
        delete GameApplication::s_Menu;
        GameApplication::s_Menu = new SettingsMenu();
    }
    else if (newMenu == "CreditsMenu")
    {
        delete GameApplication::s_Menu;
        GameApplication::s_Menu = new CreditsMenu();
    }
    else
    {
        delete GameApplication::s_Menu;
        GameApplication::s_Menu = new MainMenu();
    }
}

#endif // __MENU_H__