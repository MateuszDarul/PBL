#include "ScriptComponent.h"
#include "ColliderComponent.h"

ScriptComponent::ScriptComponent()
    : Component(100)
{
}

bool ScriptComponent::Add(Script* newScript)
{
    for (Script* s : m_ScriptInstances)
    {
        //check if script of that type is already assigned
        if (typeid(*s) == typeid(*newScript)) 
        {
            return false;
        }
    }
    
    newScript->gameObject = GetOwner();
    m_ScriptInstances.push_back(newScript);
    return true;
}

void ScriptComponent::Clear()
{
    for (int i = 0; i < m_ScriptInstances.size(); i++)
    {
        delete m_ScriptInstances[i];
        m_ScriptInstances[i] = nullptr;
    }
}
#include "GameObject.h"
void ScriptComponent::OnStart()
{
    for (Script* s : m_ScriptInstances)
    {
        if (!s->enabled) continue;
        const char* scriptName = typeid(*s).name();

        auto nameCmp = s->gameObject->GetComponent<cmp::Name>();
        const char* goName = (nameCmp) ? nameCmp->Get().c_str() : "Unnamed";

        printf("Starting script of type %s at GO: %s\n", scriptName, goName);
        s->Start();
    }
}

void ScriptComponent::OnUpdate(float dt)
{
    for (Script* s : m_ScriptInstances)
    {
        if (s != nullptr && s->enabled)
            s->Update(dt);
    }
}

void ScriptComponent::OnCollisionEnter(std::shared_ptr<ColliderComponent> collider)
{
    for (Script* s : m_ScriptInstances)
    {
        if (s->enabled)
            s->CollisionEnter(collider);
    }
}

void ScriptComponent::OnCollisionStay(std::shared_ptr<ColliderComponent> collider)
{
    for (Script* s : m_ScriptInstances)
    {
        if (s->enabled)
            s->CollisionStay(collider);
    }
}

void ScriptComponent::OnCollisionExit(std::shared_ptr<ColliderComponent> collider)
{
    for (Script* s : m_ScriptInstances)
    {
        if (s->enabled)
            s->CollisionExit(collider);
    }
}

void ScriptComponent::OnTriggerEnter(std::shared_ptr<ColliderComponent> collider)
{
    for (Script* s : m_ScriptInstances)
    {
        if (s->enabled)
            s->TriggerEnter(collider);
    }
}

void ScriptComponent::OnTriggerStay(std::shared_ptr<ColliderComponent> collider)
{
    for (Script* s : m_ScriptInstances)
    {
        if (s->enabled)
            s->TriggerStay(collider);
    }
}

void ScriptComponent::OnTriggerExit(std::shared_ptr<ColliderComponent> collider)
{
    for (Script* s : m_ScriptInstances)
    {
        if (s->enabled)
            s->TriggerExit(collider);
    }
}

void ScriptComponent::EnableAll()
{
    for (Script* s : m_ScriptInstances)
    {
        s->SetEnabled(true);
    }
}

void ScriptComponent::DisableAll()
{
    for (Script* s : m_ScriptInstances)
    {
        s->SetEnabled(false);
    }
}

void Script::SetEnabled(bool enabled)
{
    if (!this->enabled && enabled)
        Start();

    this->enabled = enabled;
}

void Script::CollisionEnter(std::shared_ptr<ColliderComponent> collider)
{
}

void Script::CollisionStay(std::shared_ptr<ColliderComponent> collider)
{
}

void Script::CollisionExit(std::shared_ptr<ColliderComponent> collider)
{
}

void Script::TriggerEnter(std::shared_ptr<ColliderComponent> collider)
{
}

void Script::TriggerStay(std::shared_ptr<ColliderComponent> collider)
{
}

void Script::TriggerExit(std::shared_ptr<ColliderComponent> collider)
{
}
