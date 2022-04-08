#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>

#include <glm/matrix.hpp>

#include "SceneNode.h"
#include "GameObject.h"
#include "Components.h"
#include "GameApplication.h"
#include "ResourceManager.h"
#include "InputManager.h"

/** @brief Scena do zarzadzania SceneNode'ami
 */
class Scene
{
private:
    SceneNode* scene;
    GameObject* goCamera;

    glm::mat4 transform; //!< Macierz transformacji widoku i kamery.

public:
    /** @brief Domyslny konstruktor.
     */
    Scene();
    
    /** @brief Domyslny destruktor.
     */
    ~Scene();

    /** @brief Metoda wykonywana w kazdej klatce.
     * @param deltaTime - Taktowanie (delta time).
     */
    void Update(float deltaTime);

    /** @brief Metoda wyswietlajaca obiekty na ekranie.
     */
    void Render();
};

#endif // __SCENE_H__
