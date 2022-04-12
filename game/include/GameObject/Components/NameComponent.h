#ifndef __NAME_COMPONENT_H__
#define __NAME_COMPONENT_H__

#include "Component.h"


/** @brief Komponent przechowywujacy nazwe.
 */
class NameComponent :public Component
{
private:
    std::string name; //!< Nazwa.

public:
    /** @brief Konstruktor, ktory pozwala na ustawienie nazwy.
     * @param gameObject - GameObject do kt�rego nale�y komponent.
     * @param name - Nazwa.
     */
    NameComponent(GameObject* gameObject, const std::string& name = "DefaultName");

    /** @brief Ustawia nowa nazwe.
     * @param name - Nowa nazwa.
     */
    void Set(const std::string& name);

    /** @brief Pobiera nazwe.
     * @return std::string - Nazwa.
     */
    std::string Get();
};

#endif // __NAME_COMPONENT_H__