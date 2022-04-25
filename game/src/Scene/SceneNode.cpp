#include "SceneNode.h"

SceneNode::SceneNode(std::shared_ptr<GameObject> gameObject)
    :parent(nullptr), gameObject(gameObject)
{
    this->globalTransformations = glm::mat4(1.f);
}

SceneNode::~SceneNode()
{

}

void SceneNode::AddChild(std::shared_ptr<GameObject> gameObject)
{
    this->AddChild(std::make_shared<SceneNode>(gameObject));
}

void SceneNode::AddChild(std::shared_ptr<SceneNode> sceneNode)
{
    sceneNode->SetParent(this);
    this->children.push_back(sceneNode);
}

void SceneNode::LoadScripts()
{
    std::shared_ptr<ScriptComponent> scriptPtr = this->gameObject->GetComponent<cmp::Scriptable>();
    if(scriptPtr)
    {
        scriptPtr->OnStart();
    }

    for(unsigned short int i=0; i<children.size(); i++)
    {
        this->children[i]->LoadScripts();
    }
}

void SceneNode::Update(float dt)
{
    this->PrivateUpdate(dt, glm::mat4(1.f));
}

void SceneNode::PrivateUpdate(float dt, const glm::mat4& parentTransformations)
{
    std::shared_ptr<ScriptComponent> scriptPtr = this->gameObject->GetComponent<cmp::Scriptable>();
    if(scriptPtr)
    {
        scriptPtr->OnUpdate(dt);
    }

    std::shared_ptr<TransformComponent> transformPtr = this->gameObject->GetComponent<cmp::Transform>();
    if(transformPtr)
    {
        this->globalTransformations = (parentTransformations * transformPtr->GetModelMatrix());
    }
    else
    {
        this->globalTransformations = parentTransformations;
    }

    ///***

    for(unsigned short int i=0; i<children.size(); i++)
    {
        this->children[i]->PrivateUpdate(dt, this->globalTransformations);
    }
}

void SceneNode::Render(const glm::mat4& matrixPV)
{
    std::shared_ptr<cmp::Shader> shaderPtr = this->gameObject->GetComponent<cmp::Shader>();
    if(shaderPtr != nullptr)
    {
        std::shared_ptr<cmp::Model> modelPtr = this->gameObject->GetComponent<cmp::Model>();
        std::shared_ptr<cmp::ModelInst> modelInstPtr = this->gameObject->GetComponent<cmp::ModelInst>();

        shaderPtr->Use();
        shaderPtr->SetMat4("transform", matrixPV * this->globalTransformations);

        if(modelPtr != nullptr)
        {
            modelPtr->Draw(shaderPtr);
        }
        else if(modelInstPtr != nullptr)
        {
            modelInstPtr->Draw(shaderPtr);
        }
    }

    for(unsigned short int i=0; i<this->children.size(); i++)
    {
        this->children[i]->Render(matrixPV);
    }
}

std::shared_ptr<GameObject> SceneNode::GetGameObject()
{
    return this->gameObject;
}

std::shared_ptr<TransformComponent> SceneNode::GetLocalTransformations()
{
    return this->gameObject->GetComponent<cmp::Transform>();
}

SceneNode* SceneNode::FindNode(const std::string& name)
{
    SceneNode* lastSearchedNode = nullptr;
    FindByName(name, &lastSearchedNode);
    return lastSearchedNode;
}

void SceneNode::FindByName(const std::string& name, SceneNode** result)
{
    if(this->gameObject->GetComponent<cmp::Name>() != nullptr &&
        this->gameObject->GetComponent<cmp::Name>()->Get() == name)
    {
        *result = this;
    }

    if(*result != nullptr)
    {
        return;
    }

    for(unsigned short int i=0; i<this->children.size(); i++)
    {
        this->children[i]->FindByName(name, result);

        if(*result != nullptr)
            break;
    }
}

void SceneNode::SetParent(SceneNode* parent)
{
    this->parent = parent;
}

SceneNode* SceneNode::GetParent()
{
    return this->parent;
}

bool SceneNode::Is(SceneNode* second)
{
    return this == second;
}

bool SceneNode::Is(std::shared_ptr<SceneNode> second)
{
    return this == second.get();
}
