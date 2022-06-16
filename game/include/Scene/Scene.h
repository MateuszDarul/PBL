#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>

#include <glm/matrix.hpp>

#include "SceneNode.h"
#include "GameObject.h"
#include "GameApplication.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "MapLoader.h"
#include "Skybox.h"
#include "CollidersManager.h"
#include "MusicBuffer.h"

class ShadowsManager;

/** @brief Scena do zarzadzania SceneNode'ami
 */
class Scene
{
private:
    SceneNode* world; //!< Swiat gry.
    Skybox* skybox;
    CollidersManager* collidersManager;
    ShadowsManager* shadowsManager;
    glm::mat4 transform; //!< Macierz transformacji widoku i kamery.
    MusicBuffer* musicBuffer;

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

    //void AddGameObject(GameObject* parent = nullptr);
    void AddGameObject(std::shared_ptr<GameObject> child, std::shared_ptr<GameObject> parent = std::shared_ptr<GameObject>());
    SceneNode* GetWorldNode();
    CollidersManager* GetCollidersManager();

};

#endif // __SCENE_H__
