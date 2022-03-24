#include "NameComponent.h"

NameComponent::NameComponent(const std::string& name)
    :Component(1), name(name)
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