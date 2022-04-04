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

void SceneNode::UpdateTransformations(glm::mat4 parentTransformations)
{
    this->globalTransformations = (parentTransformations *
        this->GetGameObject()->GetComponent<cmp::Transform>()->GetModelMatrix());

    for(unsigned short int i=0; i<children.size(); i++)
    {
        children[i]->UpdateTransformations(this->globalTransformations);
    }
}

void SceneNode::Render(glm::mat4 matrixPV)
{
    if(this->needUpdate)
    {
        this->UpdateTransformations(glm::mat4(1.f));
        this->needUpdate = false;
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

SceneNode* SceneNode::Find(std::string name)
{
    if(this->gameObject->GetComponent<cmp::Name>() != nullptr)
        std::cout << this->gameObject->GetComponent<cmp::Name>()->Get() << " " << name << "\n";

    if(this->gameObject->GetComponent<cmp::Name>() != nullptr &&
        this->gameObject->GetComponent<cmp::Name>()->Get() == name)
    {
        return this;
    }

    for(unsigned short int i=0; i<this->children.size(); i++)
    {
        this->children[i]->Find(name);
    }
    
    return nullptr;
}