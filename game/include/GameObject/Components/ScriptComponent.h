#ifndef __SCRIPT_COMPONENT_H__
#define __SCRIPT_COMPONENT_H__

#include "Component.h"

#include <typeinfo>
#include <memory>

class GameObject;
class ColliderComponent;
class ScriptComponent;

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

    /**
     * @brief Metoda wykonywana na spotkaniu się koliderów, kiedy każdy z nich jest zderzaczem
     *
     * @param collision - zderzacz z którym doszło do kolizji
     */
    virtual void CollisionEnter(std::shared_ptr<ColliderComponent> collider);

    /**
     * @brief Metoda wykonywana kiedy kolidery się ze sobą zderzają, a każdy z nich jest zderzaczem
     *
     * @param collision - zderzacz z którym doszło do kolizji
     */
    virtual void CollisionStay(std::shared_ptr<ColliderComponent> collider);

    /**
     * @brief Metoda wykonywana na rozejściu się koliderów, kiedy każdy z nich jest zderzaczem
     *
     * @param collision - zderzacz z którym doszło do kolizji
     */
    virtual void CollisionExit(std::shared_ptr<ColliderComponent> collider);

    /**
     * @brief Metoda wykonywana na spotkaniu się koliderów, kiedy jeden z nich jest wyzwalaczem
     *
     * @param collision - wyzwalacz z którym doszło do kolizji
     */
    virtual void TriggerEnter(std::shared_ptr<ColliderComponent> collider);

    /**
     * @brief Metoda wykonywana kiedy kolidery się ze sobą zderzają, a jeden z nich jest wyzwalaczem
     *
     * @param collision - wyzwalacz z którym doszło do kolizji
     */
    virtual void TriggerStay(std::shared_ptr<ColliderComponent> collider);

    /**
     * @brief Metoda wykonywana na rozejściu się koliderów, kiedy jeden z nich jest wyzwalaczem
     *
     * @param collision - wyzwalacz z którym doszło do kolizji
     */
    virtual void TriggerExit(std::shared_ptr<ColliderComponent> collider);
    
    /**
     * @brief Ustawia flage enabled. Przy ustawieniu na true, gdy wczesniej bylo false, wywoluje metode Start
     */
    void SetEnabled(bool enabled);

    /**
     * @brief GameObject posiadajacy instancje skryptu
     */
    std::shared_ptr<GameObject> gameObject;

protected:
    /**
     * @brief Flaga ustalajaca czy wykonywac Update danej instancji skryptu
     */
    bool enabled = true;

    friend ScriptComponent;

public:
    virtual ~Script() = default;
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
     * @brief Wywoluje SetEnable(true) dla wszystkich przypisanych skryptow
     */
    void EnableAll();

    /**
     * @brief Wywoluje SetEnable(false) dla wszystkich przypisanych skryptow
     */
    void DisableAll();

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

    /**
     * @brief Metoda wykonywana na spotkaniu się koliderów, kiedy każdy z nich jest zderzaczem
     *
     * @param collision - zderzacz z którym doszło do kolizji
     */
    void OnCollisionEnter(std::shared_ptr<ColliderComponent> collider);

    /**
     * @brief Metoda wykonywana kiedy kolidery się ze sobą zderzają, a każdy z nich jest zderzaczem
     *
     * @param collision - zderzacz z którym doszło do kolizji
     */
    void OnCollisionStay(std::shared_ptr<ColliderComponent> collider);

    /**
     * @brief Metoda wykonywana na rozejściu się koliderów, kiedy każdy z nich jest zderzaczem
     *
     * @param collision - zderzacz z którym doszło do kolizji
     */
    void OnCollisionExit(std::shared_ptr<ColliderComponent> collider);

    /**
     * @brief Metoda wykonywana na spotkaniu się koliderów, kiedy jeden z nich jest wyzwalaczem
     *
     * @param collision - wyzwalacz z którym doszło do kolizji
     */
    void OnTriggerEnter(std::shared_ptr<ColliderComponent> collider);

    /**
     * @brief Metoda wykonywana kiedy kolidery się ze sobą zderzają, a jeden z nich jest wyzwalaczem
     *
     * @param collision - wyzwalacz z którym doszło do kolizji
     */
    void OnTriggerStay(std::shared_ptr<ColliderComponent> collider);

    /**
     * @brief Metoda wykonywana na rozejściu się koliderów, kiedy jeden z nich jest wyzwalaczem
     *
     * @param collision - wyzwalacz z którym doszło do kolizji
     */
    void OnTriggerExit(std::shared_ptr<ColliderComponent> collider);

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