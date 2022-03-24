#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include <vector>
#include <memory>

#include "Component.h"

class GameObject
{
private:
    std::vector<Component*> components;

public:
    bool AddComponent(Component* component);

    template<typename T> T* GetComponent();

    bool RemoveComponent(Component* component);
    
    template<typename T> bool RemoveComponent();
};

#endif // __GAME_OBJECT_H__