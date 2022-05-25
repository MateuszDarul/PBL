#ifndef __SHADOW_COMPONENT_H__
#define __SHADOW_COMPONENT_H__

#include "Component.h"

class ShadowComponent :public Component
{
private:
    bool makeShadow;

public:
    ShadowComponent();
    ~ShadowComponent();

    void Create(bool enable);
    bool ShadowTest();
    void EnableShadow();
    void DisableShadow();
};

#endif // __SHADOW_COMPONENT_H__