#include "GameObject.h"

bool GameObject::AddComponent(std::shared_ptr<Component> component)
{
    /// [ component->GetClassUUID() != 5 ] Poniewaz ShaderComponent ma id rowne 5, a shadery moga byc wspolne.
    if(component->GetClassUUID() != 5 && !component->SetOwner(shared_from_this()))
    {
        return false;
    }

    for(uint32_t i=0; i<components.size(); i++)
    {
        if(components[i]->GetClassUUID() == component->GetClassUUID())
        {
            return false;
        }
    }
    components.push_back(component);
    return true;
}

bool GameObject::RemoveComponent(std::shared_ptr<Component> component)
{
    for(uint32_t i=0; i<components.size(); i++)
    {
        if(components[i] == component)
        {
            components.erase(components.begin() + i);
            return true;
        }
    }
    return false;
}

template<typename T>
bool GameObject::RemoveComponent()
{
    T tmp;
    for (uint32_t i=0; i<components.size(); i++)
    {
        if (components[i]->GetClassUUID() == tmp.GetClassUUID())
        {
            components.erase(components.begin() + i);
            return true;
        }
    }
    return false;
}

template<typename T>
std::shared_ptr<T> GameObject::GetComponent()
{
    T tmp;
    for(uint32_t i=0; i<components.size(); i++)
    {
       // std::cout << components[i]->GetClassUUID() << ' ' << tmp.GetClassUUID() << '\n';
        if(components[i]->GetClassUUID() == tmp.GetClassUUID())
        {
            return std::static_pointer_cast<T>(components[i]);
        }
    }
    return nullptr;
}

bool GameObject::Is(GameObject* second)
{
    return this == second;
}

bool GameObject::Is(std::shared_ptr<GameObject> second)
{
    return this == second.get();
}

void GameObject::SetNode(SceneNode* node)
{
    this->node = node;
}

SceneNode* GameObject::GetNode()
{
    return node;
}


///***

template std::shared_ptr<NameComponent> GameObject::GetComponent<NameComponent>();
template std::shared_ptr<ListComponent> GameObject::GetComponent<ListComponent>();
template std::shared_ptr<ModelComponent> GameObject::GetComponent<ModelComponent>();
template std::shared_ptr<ModelInstancesComponent> GameObject::GetComponent<ModelInstancesComponent>();
template std::shared_ptr<ModelGuiComponent> GameObject::GetComponent<ModelGuiComponent>();
template std::shared_ptr<ShaderComponent> GameObject::GetComponent<ShaderComponent>();
template std::shared_ptr<TransformComponent> GameObject::GetComponent<TransformComponent>();
template std::shared_ptr<CameraComponent> GameObject::GetComponent<CameraComponent>();
template std::shared_ptr<ScriptComponent> GameObject::GetComponent<ScriptComponent>();
template std::shared_ptr<TextComponent> GameObject::GetComponent<TextComponent>();
template std::shared_ptr<LineComponent> GameObject::GetComponent<LineComponent>();
template std::shared_ptr<BoxCollider> GameObject::GetComponent<BoxCollider>();
template std::shared_ptr<SphereCollider> GameObject::GetComponent<SphereCollider>();
template std::shared_ptr<PointLightComponent> GameObject::GetComponent<PointLightComponent>();
template std::shared_ptr<SpotLightComponent> GameObject::GetComponent<SpotLightComponent>();
template std::shared_ptr<FrustumCullingComponent> GameObject::GetComponent<FrustumCullingComponent>();
template std::shared_ptr<ParticleComponent> GameObject::GetComponent<ParticleComponent>();
template std::shared_ptr<ShadowComponent> GameObject::GetComponent<ShadowComponent>();

template bool GameObject::RemoveComponent<NameComponent>();
template bool GameObject::RemoveComponent<ListComponent>();
template bool GameObject::RemoveComponent<ModelComponent>();
template bool GameObject::RemoveComponent<ModelInstancesComponent>();
template bool GameObject::RemoveComponent<ModelGuiComponent>();
template bool GameObject::RemoveComponent<ShaderComponent>();
template bool GameObject::RemoveComponent<TransformComponent>();
template bool GameObject::RemoveComponent<CameraComponent>();
template bool GameObject::RemoveComponent<ScriptComponent>();
template bool GameObject::RemoveComponent<TextComponent>();
template bool GameObject::RemoveComponent<LineComponent>();
template bool GameObject::RemoveComponent<BoxCollider>();
template bool GameObject::RemoveComponent<SphereCollider>();
template bool GameObject::RemoveComponent<PointLightComponent>();
template bool GameObject::RemoveComponent<SpotLightComponent>();
template bool GameObject::RemoveComponent<FrustumCullingComponent>();
template bool GameObject::RemoveComponent<ParticleComponent>();
template bool GameObject::RemoveComponent<ShadowComponent>();
