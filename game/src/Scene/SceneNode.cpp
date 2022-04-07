#include "SceneNode.h"

SceneNode::SceneNode(GameObject* gameObject)
    :gameObject(gameObject)
{
    this->globalTransformations = glm::mat4(1.f);
}

SceneNode::~SceneNode()
{

}

void SceneNode::AddChild(GameObject* gameObject)
{
    this->children.push_back(new SceneNode(gameObject));
}

void SceneNode::AddChild(SceneNode* sceneNode)
{
    this->children.push_back(sceneNode);
}

void SceneNode::UpdateTransformations(const glm::mat4& parentTransformations)
{
    if(this->GetGameObject()->GetComponent<cmp::Transform>() != nullptr)
    {
        this->globalTransformations = (parentTransformations *
            this->GetGameObject()->GetComponent<cmp::Transform>()->GetModelMatrix());
    }
    else
    {
        this->globalTransformations = parentTransformations;
    }

    this->needUpdate = false;

    for(unsigned short int i=0; i<children.size(); i++)
    {
        this->children[i]->UpdateTransformations(this->globalTransformations);
    }
}

void SceneNode::Render(const glm::mat4& matrixPV)
{
    if(this->needUpdate)
    {
        this->UpdateTransformations(glm::mat4(1.f));
    }

    cmp::Shader* shaderPtr = this->gameObject->GetComponent<cmp::Shader>();
    if(shaderPtr != nullptr)
    {
        cmp::Model* modelPtr = this->gameObject->GetComponent<cmp::Model>();
        cmp::ModelInst* modelInstPtr = this->gameObject->GetComponent<cmp::ModelInst>();

        if(modelPtr != nullptr)
        {
            shaderPtr->Use();
            shaderPtr->SetMat4("transform", matrixPV * this->globalTransformations);
            modelPtr->Draw(shaderPtr);
        }
        else if(modelInstPtr != nullptr)
        {
            shaderPtr->Use();
            shaderPtr->SetMat4("transform", matrixPV);
            modelInstPtr->Draw(shaderPtr);
        }
    }

    for(unsigned short int i=0; i<this->children.size(); i++)
    {
        this->children[i]->Render(matrixPV);
    }
}

GameObject* SceneNode::GetGameObject()
{
    return this->gameObject;
}

TransformComponent* SceneNode::GetLocalTransformations()
{
    this->needUpdate = true;
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