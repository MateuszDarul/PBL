#include "FrustumCullingComponent.h"

#include "GameObject.h"
#include "SceneNode.h"

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
    glm::vec3 vec;
    for(uint32_t i=0; i<mesh->vertices.size(); i++)
    {
        vec = glm::vec3(mesh->vertices[i].position);
        float distance = glm::length(vec);
        if(distance > max)
        {
            max = distance;
        }
    }
    this->radius = max;

    return true;
}

bool FrustumCullingComponent::Create(float size)
{
    this->radius = size;

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
    // position = this->transformComponentPtr->GetPosition();
    auto& m = this->GetOwner()->GetNode()->GetGlobalTransformations();
    position = { m[3][0], m[3][1], m[3][2] };

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