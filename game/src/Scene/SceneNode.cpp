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

void SceneNode::Update()
{
    this->UpdateTransformations(glm::mat4(1.f));
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

    if (auto scriptHolder = this->GetGameObject()->GetComponent<ScriptComponent>())
    {
        //scriptHolder->OnUpdate(dt);
    }

    for(unsigned short int i=0; i<children.size(); i++)
    {
        this->children[i]->UpdateTransformations(this->globalTransformations);
    }
}

void SceneNode::Render(const glm::mat4& matrixPV)
{
    std::shared_ptr<cmp::Shader> shaderPtr = this->gameObject->GetComponent<cmp::Shader>();
    if(shaderPtr != nullptr)
    {
        std::shared_ptr<cmp::Model> modelPtr = this->gameObject->GetComponent<cmp::Model>();
        std::shared_ptr<cmp::ModelInst> modelInstPtr = this->gameObject->GetComponent<cmp::ModelInst>();
        std::shared_ptr<cmp::Text> textPtr = this->gameObject->GetComponent<cmp::Text>();
        std::shared_ptr<cmp::Line> linePtr = this->gameObject->GetComponent<cmp::Line>();

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
        else if (textPtr != nullptr)
        {
            if(textPtr->alwaysSeen)
            {
                glDepthMask(GL_FALSE);
                glDepthFunc(GL_ALWAYS);
                textPtr->Draw(shaderPtr);
                glDepthFunc(GL_LESS);
                glDepthMask(GL_TRUE);
            }
            else
            {
                textPtr->Draw(shaderPtr);
            }
        }
        else if (linePtr != nullptr)
        {
            linePtr->Draw(shaderPtr);
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
