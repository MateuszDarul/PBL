#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <string>
#include <vector>

class GameObject;

class Component
{
private:
    static uint32_t id_update; //!< Wartosc wykorzystywana do nadawania unikalnych wartosci ID.
    uint32_t component_type_uuid; //!< Wartosc sluzaca do rozrozniania typu komponenta, kazda klasa dziedziczaca z Component musi miec inna wartosc.
    uint32_t component_id; //!< Unikalna wartosc komponenta.
    GameObject* gameObject; //!<Wska�nik na gameObject posiadaj�cy ten komponent

public:
    /** @brief Konstruktor, ktory musi byc wykonany w konstruktorach klas dziedziczacych z klasy Component.
     * @param component_type_uuid - Wartosc, ktora nie byla wykorzystana w innych komponentach.
     * @param gameObject - GameObject do kt�rego nale�y komponent.
     */
    Component(uint32_t component_type_uuid, GameObject* gameObject);

    /** @brief Wirtualny destruktor.
     */
    virtual ~Component();

    /** @brief Zwraca wartosc ID danego komponenta.
     * @return uint32_t - ID komponenta.
     */
    uint32_t GetID();
    
    /** @brief Zwraca wartosc ID, unikalna dla calej klasy dzidziczacej z Component, ale wszystkie komponenty tej samej klasy maja wspolna wartosc.
     * @return uint32_t - ID klasy.
     */
    uint32_t GetClassUUID();

    GameObject* GetGameObject();
};

#endif // __COMPONENT_H__