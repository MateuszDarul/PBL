#include "GameObject.h"

bool GameObject::AddComponent(Component *component)
{
    for(uint32_t i=0; i<components.size(); i++)
    {
        if(components[i]->GetClassUUID() == component->GetClassUUID())
        {
            return false;
        }
    }
    components.push_back(component);
    return true;
}

bool GameObject::RemoveComponent(Component *component)
{
    for(uint32_t i=0; i<components.size(); i++)
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
    for (uint32_t i=0; i<components.size(); i++)
    {
        if (components[i]->GetClassUUID() == tmp.GetClassUUID())
        {
            components.erase(components.begin() + i);
            return true;
        }
    }
    return false;
}

template<typename T>
T* GameObject::GetComponent()
{
    T tmp;
    for(uint32_t i=0; i<components.size(); i++)
    {
        if(components[i]->GetClassUUID() == tmp.GetClassUUID())
        {
            return (T*)components[i];
        }
    }
    return nullptr;
}

///***

template NameComponent* GameObject::GetComponent<NameComponent>();
template ListComponent* GameObject::GetComponent<ListComponent>();
template ModelComponent* GameObject::GetComponent<ModelComponent>();
template ModelInstancesComponent* GameObject::GetComponent<ModelInstancesComponent>();
template ShaderComponent* GameObject::GetComponent<ShaderComponent>();
template TransformComponent* GameObject::GetComponent<TransformComponent>();

template bool GameObject::RemoveComponent<NameComponent>();
template bool GameObject::RemoveComponent<ListComponent>();
template bool GameObject::RemoveComponent<ModelComponent>();
template bool GameObject::RemoveComponent<ModelInstancesComponent>();
template bool GameObject::RemoveComponent<ShaderComponent>();
template bool GameObject::RemoveComponent<TransformComponent>();