#ifndef __SCENE_NODE_H__
#define __SCENE_NODE_H__

#include <vector>

#include <glm/matrix.hpp>

#include "GameObject.h"
#include "Components.h"

class SceneNode
{
private:
    bool needUpdate;
    GameObject* gameObject;
    std::vector<SceneNode*> children;
    glm::mat4 globalTransformations;

public:
    SceneNode(GameObject* gameObject);
    ~SceneNode();

    void AddChild(GameObject* gameObject);
    void AddChild(SceneNode* sceneNode);
    GameObject* GetGameObject();

    void UpdateTransformations(const glm::mat4& parentTransformations);
    TransformComponent* GetLocalTransformations();
    void Render(const glm::mat4& matrixPV);

    /** @brief Szuka SceneNode, gdzie GameObject posiada komponent NameComponent o podanej zawartosci.
     * @param name - Nazwa poszukiwanego Noda.
     * @return SceneNode* - Wskazanie na wezel sceny.
     * @return nullptr - Nie znaleziono wezla z GameObjectem o podanej nazwie.
     */
    SceneNode* FindNode(const std::string& name);
};

#endif // __SCENE_NODE_H__