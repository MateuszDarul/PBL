#ifndef __NAME_COMPONENT_H__
#define __NAME_COMPONENT_H__

#include "Component.h"

class NameComponent :public Component
{
private:
    std::string name;

public:
    NameComponent(const std::string& name = "DefaultName");

    void Set(const std::string& name);

    std::string Get();
};

#endif // __NAME_COMPONENT_H__