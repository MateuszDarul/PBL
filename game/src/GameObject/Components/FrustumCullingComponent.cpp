#include "FrustumCullingComponent.h"

#include "GameObject.h"

FrustumCullingComponent::FrustumCullingComponent()
    :Component(10)
{
    
}

FrustumCullingComponent::~FrustumCullingComponent()
{

}

bool FrustumCullingComponent::Create(Mesh* mesh)
{
    if(mesh == nullptr)
        return false;

    float max = 0;
    float* vecPtr;
    for(uint32_t i=0; i<mesh->vertices.size(); i++)
    {
        vecPtr = &mesh->vertices[i].position.x;
        for(uint8_t j=0; j<3; j++)
            if(*(vecPtr+j) > max)
                max = *(vecPtr+j);
    }
    this->radius = max;

    return true;
}

bool FrustumCullingComponent::IsVisible(Frustum frustum)
{
    if(GetOwner() == nullptr)
        return false;

    glm::vec3 position = glm::vec3(0,0,0);
    float scaledRadius = this->radius + 1;

    if(this->transformComponentPtr == nullptr)
        this->transformComponentPtr = GetOwner()->GetComponent<cmp::Transform>();
    if(this->transformComponentPtr == nullptr)
        this->transformComponentPtr = std::make_shared<cmp::Transform>();

    scaledRadius *= this->transformComponentPtr->GetScale();
    position = this->transformComponentPtr->GetPosition();

    if(frustum.topFace.Distance(position) > -scaledRadius && 
        frustum.bottomFace.Distance(position) > -scaledRadius && 
        frustum.leftFace.Distance(position) > -scaledRadius && 
        frustum.rightFace.Distance(position) > -scaledRadius && 
        frustum.farFace.Distance(position) > -scaledRadius && 
        frustum.nearFace.Distance(position) > -scaledRadius)
    {
        return true;
    }
    else 
    {
        return false;
    }
}