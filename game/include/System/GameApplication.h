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
private:
    static void FramebufferResizeCallback(GLFWwindow*, int width, int height);

    static GLFWwindow* s_Window;

    static float s_Fov;
    static int s_ScreenWidth, s_ScreenHeight;
    static float s_NearPlane, s_FarPlane;
    static glm::mat4 s_ProjectionMatrix;

    static Scene* s_Scene;
    static InputManager* s_InputManager;
    static ResourceManager* s_ResourceManager;

private:
    /**
     * @brief Sprząta po sobie, wywoływane po zakończeniu pętli gry.
     * 
     */
    static void OnStop();

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

    /**
     * @brief Zwraca rozmiar okna
     * 
     * @return glm::uvec2 - Rozmiar okna.
     */
    static glm::uvec2 GetWindowSize();

    /**
     * @brief Get the Projection Range
     * @return glm::vec2::x - Near plane
     * @return glm::vec2::y - Far plane
     */
    static glm::vec2 GetProjectionRange();

    /**
     * @brief Set the Projection Range
     * @return float - Near plane
     * @return float - Far plane
     */
    static void SetProjectionRange(float nearPlane, float farPlane);

    static float GetFov();

    static void SetFov(float fov);
};

/**
 * @brief Skrót do GameApplication::GetInputManager
 * 
 */
constexpr auto Input = GameApplication::GetInputManager;