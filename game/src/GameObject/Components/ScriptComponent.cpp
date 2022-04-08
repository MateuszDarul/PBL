#include "ScriptComponent.h"

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