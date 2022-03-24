#include "Component.h"

uint32_t Component::id_update = 0;

Component::Component(uint32_t component_type_uuid)
    :component_type_uuid(component_type_uuid), component_id(id_update)
{
    id_update++;
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