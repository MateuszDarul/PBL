#include "NameComponent.h"

NameComponent::NameComponent(GameObject* gameObject, const std::string& name)
    :Component(1, gameObject), name(name)
{
    ;
}

void NameComponent::Set(const std::string& name)
{
    this->name = name;
}

std::string NameComponent::Get()
{
    return this->name;
}