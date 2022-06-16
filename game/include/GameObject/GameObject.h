#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "Components.h"
#include "Component.h"

/** @brief Klasa reprezentujaca dowolny "byt" w silniku.
 */
class GameObject : public std::enable_shared_from_this<GameObject>
{
private:
    std::vector<std::shared_ptr<Component>> components; //!< Lista komponentow nalezacych to GameObjectu.
    SceneNode* node;
    bool isDestroyed;

public:
    /** @brief Dodaje nowy komponent.
     * @param component - Wskazanie na komponent, ktory chcemy dodac.
     * @return true - Dodano nowy komponent.
     * @return false - Nie dodano komponenta (prawdopodobnie taki komponent juz jest przypisany do GameObjectu).
     */
    bool AddComponent(std::shared_ptr<Component> component);

    /** @brief Zwraca wskazanie na wybrany komponent.
     * @tparam T - Wybrany typ komponenta.
     * @return T* - Wskazanie na komponent.
     */
    template<typename T> std::shared_ptr<T> GetComponent();

    /** @brief Usuwa komponent z GameObjectu po przez podanie, ktory komponent chcemy usunac.
     * @param component - Wskazanie na komponent do usuniecia.
     * @return true - Usunieto wybrany komponent.
     * @return false - Nie usunieto komponenta.
     */
    bool RemoveComponent(std::shared_ptr<Component> component);
    
    /** @brief Usuwa komponent z GameObjectu po przez podanie, typu komponenta chcemy usunac.
     * @param T - Typ komponenta do usuniecia.
     * @return true - Usunieto wybrany komponent.
     * @return false - Nie usunieto komponenta.
     */
    template<typename T> bool RemoveComponent();

    /** @brief Porownuje GameObject'y.
     * @param second - GameObject z ktorym chcemy porownac.
     * @return true - To jest ten sam GameObject.
     * @return false - Sa to rozne GameObject.
     */
    bool Is(GameObject* second);

    /** @brief Porownuje GameObject'y.
     * @param second - GameObject z ktorym chcemy porownac.
     * @return true - To jest ten sam GameObject.
     * @return false - Sa to rozne GameObject.
     */
    bool Is(std::shared_ptr<GameObject> second);

    void SetNode(SceneNode* node);

    SceneNode* GetNode();

    bool IsDestroyed() const;
    void SetDestroyed(bool destroyed = true);
};

#endif // __GAME_OBJECT_H__