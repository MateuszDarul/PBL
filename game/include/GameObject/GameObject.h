#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "Components.h"
#include "Component.h"

/** @brief Klasa reprezentujaca dowolny "byt" w silniku.
 */
class GameObject
{
private:
    std::vector<Component*> components; //!< Lista komponentow nalezacych to GameObjectu.

public:
    /** @brief Dodaje nowy komponent.
     * @param component - Wskazanie na komponent, ktory chcemy dodac.
     * @return true - Dodano nowy komponent.
     * @return false - Nie dodano komponenta (prawdopodobnie taki komponent juz jest przypisany do GameObjectu).
     */
    bool AddComponent(Component* component);

    /** @brief Zwraca wskazanie na wybrany komponent.
     * @tparam T - Wybrany typ komponenta.
     * @return T* - Wskazanie na komponent.
     */
    template<typename T> T* GetComponent();

    /** @brief Usuwa komponent z GameObjectu po przez podanie, ktory komponent chcemy usunac.
     * @param component - Wskazanie na komponent do usuniecia.
     * @return true - Usunieto wybrany komponent.
     * @return false - Nie usunieto komponenta.
     */
    bool RemoveComponent(Component* component);
    
    /** @brief Usuwa komponent z GameObjectu po przez podanie, typu komponenta chcemy usunac.
     * @param T - Typ komponenta do usuniecia.
     * @return true - Usunieto wybrany komponent.
     * @return false - Nie usunieto komponenta.
     */
    template<typename T> bool RemoveComponent();
};

#endif // __GAME_OBJECT_H__