#include "Component.h"

uint32_t Component::id_update = 0;

Component::Component(uint32_t component_type_uuid)
    :component_type_uuid(component_type_uuid), component_id(id_update)
{
    id_update++;
    this->owner = nullptr;
}

Component::~Component()
{
    ;
}

uint32_t Component::GetID()
{
    return component_id;
}

uint32_t Component::GetClassUUID()
{
    return component_type_uuid;
}

GameObject* Component::GetOwner()
{
    return owner;
}

bool Component::SetOwner(GameObject* owner)
{
    if(this->owner == nullptr)
    {
        this->owner = owner;
        return true;
    }
    return false;
}