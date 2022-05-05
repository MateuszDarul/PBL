#ifndef __SCRIPT_COMPONENT_H__
#define __SCRIPT_COMPONENT_H__

#include "Component.h"

#include <typeinfo>
#include <memory>

class GameObject;

/**
 * @brief Interfejs dla skryptow
 * 
 */
class Script
{
public:
    /**
     * @brief Metoda wykonywana raz przy tworzeniu instancji skryptu
     * 
     */
    virtual void Start() = 0;

    /**
     * @brief Metoda wykonywana raz na klatke
     * 
     * @param dt - krok czasu
     */
    virtual void Update(float dt) = 0;
    
    //On Destroy ?

    /**
     * @brief GameObject posiadajacy instancje skryptu
     */
    std::shared_ptr<GameObject> gameObject;

    /**
     * @brief Flaga ustalajaca czy wykonywac daną instancje skryptu
     */
    bool enabled = true;
};


/** @brief Komponent przechowujacy instancje skryptow
 */
class ScriptComponent : public Component
{
public:
    /** @brief Domyslny konstruktor.
     */
    ScriptComponent();

    /**
     * @brief Dodanie nowego skryptu
     * 
     * @param newScript - skrypt do dodanie
     * @return true - Dodano nowy skrypt
     * @return false - Nie dodano skryptu (skrypt takiego typu juz jest przypisany)
     */
    bool Add(Script* newScript);

    /**
     * @brief Zwraca wskazanie na skrypt danego typu
     * 
     * @tparam ScriptType - Typ skryptu
     * @return ScriptType* - Wskazanie na skrypt (nullptr w przypadku nie znalezienia)
     */
    template<class ScriptType>
    ScriptType* Get();

    /**
     * @brief Niszczy wszystkie przypisane skrypty
     * 
     */
    void Clear();

    /**
     * @brief Wywoluje wszystkie metody Start() przypisanych skryptow
     * 
     */
    void OnStart();

    /**
     * @brief Wywoluje wszystkie metody Update() przypisanych skryptow
     * 
     * @param dt - krok czasu
     */
    void OnUpdate(float dt);

private:
    std::vector<Script*> m_ScriptInstances;
};


//--- template definition
template<class ScriptType>
ScriptType* ScriptComponent::Get()
{
    for (Script* s : m_ScriptInstances)
    {
        if (typeid(*s) == typeid(ScriptType)) 
        {
           return (ScriptType*)s;
        }
    }

    return nullptr;
}

#endif // __SCRIPT_COMPONENT_H__