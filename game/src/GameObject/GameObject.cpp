#include "GameObject.h"

bool GameObject::AddComponent(Component *component)
{
    for(int i=0; i<components.size(); i++)
    {
        if(components[i]->GetClassUUID() == component->GetClassUUID())
        {
            return false;
        }
    }
    components.push_back(component);
    return true;
}

template<typename T>
T* GameObject::GetComponent()
{
    T tmp;
    for(int i=0; i<components.size(); i++)
    {
        if(components[i]->GetClassUUID() == tmp.GetClassUUID())
        {
            return (T*)components[i];
        }
    }
    return nullptr;
}

bool GameObject::RemoveComponent(Component *component)
{
    for(int i=0; i<components.size(); i++)
    {
        if(components[i] == component)
        {
            components.erase(components.begin() + i);
            return true;
        }
    }
    return false;
}

template<typename T>
bool GameObject::RemoveComponent()
{
    T tmp;
    for (int i=0; i<components.size(); i++)
    {
        if (components[i]->GetClassUUID() == tmp.GetClassUUID())
        {
            components.erase(components.begin() + i);
            return true;
        }
    }
    return false;
}

///***

template NameComponent* GameObject::GetComponent<NameComponent>();
template ListComponent* GameObject::GetComponent<ListComponent>();

template bool GameObject::RemoveComponent<NameComponent>();
template bool GameObject::RemoveComponent<ListComponent>();