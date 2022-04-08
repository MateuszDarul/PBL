#pragma once

#include "InputManager.h"
#include "ResourceManager.h"

#include <glm/matrix.hpp>

class GLFWwindow;
class Scene;


/**
 * @brief Inicjalizuje biblioteki oraz tworzy pętlę gry.
 * 
 */
class GameApplication
{
public:

    /**
     * @brief Inicjalizacja bibliotek, managerów oraz resourców. Zwraca 0 przy powodzeniu.
     * 
     * @return int kod błędu
     */
    static int Init();

    /**
     * @brief Tworzy pętlę gry.
     * 
     */
    static void Run();

    /**
     * @brief Zwraca aktualne okno aplikacji.
     * 
     * @return GLFWwindow* const 
     */
    static GLFWwindow* const GetWindow();

    /**
     * @brief Zwraca obiekt InputManagera [skrót: Input()].
     * 
     * @return InputManager*
     */
    static InputManager* const GetInputManager();

    /**
     * @brief Zwraca obiekt ResourceManagera
     * 
     * @return ResourceManager*
     */
    static ResourceManager* const GetResourceManager();

    /**
     * @brief Zwraca macierz projection
     * 
     * @return glm::mat4* 
     */
    static const glm::mat4& GetProjection();

private:
    /**
     * @brief Sprząta po sobie, wywoływane po zakończeniu pętli gry.
     * 
     */
    static void OnStop();


    static void FramebufferResizeCallback(GLFWwindow*, int width, int height);

    static GLFWwindow* s_Window;
    static int s_ScreenWidth, s_ScreenHeight;
    static glm::mat4 s_ProjectionMatrix;

    static Scene* s_Scene;

    static InputManager* s_InputManager;
    static ResourceManager* s_ResourceManager;
};

/**
 * @brief Skrót do GameApplication::GetInputManager
 * 
 */
constexpr auto Input = GameApplication::GetInputManager;