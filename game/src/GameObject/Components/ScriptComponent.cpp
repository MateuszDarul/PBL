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

    m_ScriptInstances.push_back(newScript);
    return true;
}

void ScriptComponent::Clear()
{
    for (int i = 0; i < m_ScriptInstances.size(); i++)
    {
        delete m_ScriptInstances[i];
    }
}

void ScriptComponent::OnStart()
{
    for (Script* s : m_ScriptInstances)
    {
        s->Start();
    }
}

void ScriptComponent::OnUpdate(float dt)
{
    for (Script* s : m_ScriptInstances)
    {
        if (s->enabled)
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
