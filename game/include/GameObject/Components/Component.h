#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <string>
#include <vector>

class Component
{
private:
    static uint32_t id_update;
    uint32_t component_type_uuid;
    uint32_t component_id;

public:
    Component(uint32_t component_type_uuid);

    virtual ~Component();

    uint32_t GetID();
    
    uint32_t GetClassUUID();
};

#include "NameComponent.h"
#include "ListComponent.h"

#endif // __COMPONENT_H__
