#include "ShadowComponent.h"

ShadowComponent::ShadowComponent()
    :Component(150)
{

}

ShadowComponent::~ShadowComponent()
{

}

void ShadowComponent::Create(bool enable)
{
    this->makeShadow = enable;
}

bool ShadowComponent::ShadowTest()
{
    return this->makeShadow;
}
    
void ShadowComponent::EnableShadow()
{
    this->makeShadow = true;
}

void ShadowComponent::DisableShadow()
{
    this->makeShadow = false;
}
