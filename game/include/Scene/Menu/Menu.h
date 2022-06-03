#ifndef __MENU_H__
#define __MENU_H__

#include "SceneNode.h"

class MenuScene
{
protected:
    SceneNode* menu;
    std::shared_ptr<ShaderComponent> shader_d;
    glm::vec2 prev_pos;
    int begin = 0;

protected:
    void UpdateMenu()
    {
        menu->Update(1);
        glm::vec2 pos = GetCursorOffser(GameApplication::GetInputManager()->Mouse()->GetPosition());
        menu->FindNode("CURSOR")->GetLocalTransformations()->Move(glm::vec3(pos.x, pos.y, 0));
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
        go->GetComponent<cmp::BoxCol>()->setLengths(glm::vec3(0, 0, 10));
        go->GetComponent<cmp::BoxCol>()->SetOffset(glm::vec3(-0.1, 0.1, 0));
        go->AddComponent(std::make_shared<cmp::Name>("CURSOR"));
        menu->AddChild(go);
        
        glfwMaximizeWindow(GameApplication::GetWindow());
        glfwRestoreWindow(GameApplication::GetWindow());
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
        go->GetComponent<cmp::Transform>()->SetPosition(0.0, 0.0, -3.0);
        go->AddComponent(std::make_shared<cmp::BoxCol>(true, true));
        go->GetComponent<cmp::BoxCol>()->setLengths(glm::vec3(2, 0.6, 10));
        go->AddComponent(std::make_shared<cmp::Name>("Menu_b1"));
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
                ChangeMenu("SettingsMenu");
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
        go->GetComponent<cmp::BoxCol>()->setLengths(glm::vec3(2, 0.6, 10));
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
        go->GetComponent<cmp::BoxCol>()->setLengths(glm::vec3(2, 0.6, 10));
        go->AddComponent(std::make_shared<cmp::Name>("Menu_b2"));
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

void ChangeMenu(std::string newMenu)
{
    if(newMenu == "SettingsMenu")
    {
        delete GameApplication::s_Menu;
        GameApplication::s_Menu = new SettingsMenu();
    }
    else
    {
        delete GameApplication::s_Menu;
        GameApplication::s_Menu = new MainMenu();
    }
}

#endif // __MENU_H__