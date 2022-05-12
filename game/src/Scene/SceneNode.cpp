#include "SceneNode.h"

//#define DISPLAY_FC_RESULT
Frustum SceneNode::cameraFrustum;

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

#ifdef DISPLAY_FC_RESULT
uint8_t all = 0;
uint8_t rendered = 0;
#endif  // DISPLAY_FC_RESULT
void SceneNode::Update(float dt)
{
    #ifdef DISPLAY_FC_RESULT
    std::cout << (int)rendered << " / " << (int)all << "\n";
    all = 0;
    rendered = 0;
    #endif  // DISPLAY_FC_RESULT

    this->PrivateUpdate(dt, glm::mat4(1.f));

    SceneNode::cameraFrustum = 
    this->GetRoot()->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Camera>()->GetFrustum(
        (float)GameApplication::GetWindowSize().x/GameApplication::GetWindowSize().y,
        GameApplication::GetFov(),
        GameApplication::GetProjectionRange().x,
        GameApplication::GetProjectionRange().y
    );
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
        #ifdef DISPLAY_FC_RESULT
        all++;
        #endif  // DISPLAY_FC_RESULT

        bool display = true;
        std::shared_ptr<FrustumCullingComponent> frustumCullingPtr = this->gameObject->GetComponent<FrustumCullingComponent>();
        if(frustumCullingPtr != nullptr)
        {
            display = frustumCullingPtr->IsVisible(SceneNode::cameraFrustum);
        }

        if(display)
        {
            #ifdef DISPLAY_FC_RESULT
            rendered++;
            #endif  // DISPLAY_FC_RESULT

            std::shared_ptr<cmp::PointLight> pointLightPtr = this->gameObject->GetComponent<cmp::PointLight>();
            std::shared_ptr<cmp::SpotLight> spotLightPtr = this->gameObject->GetComponent<cmp::SpotLight>();
            std::shared_ptr<cmp::Model> modelPtr = this->gameObject->GetComponent<cmp::Model>();
            std::shared_ptr<cmp::ModelInst> modelInstPtr = this->gameObject->GetComponent<cmp::ModelInst>();
            std::shared_ptr<cmp::Text> textPtr = this->gameObject->GetComponent<cmp::Text>();
            std::shared_ptr<cmp::Line> linePtr = this->gameObject->GetComponent<cmp::Line>();

            shaderPtr->Use();
            shaderPtr->SetMat4("transform", matrixPV);
            shaderPtr->SetMat4("model", this->globalTransformations);

            if(pointLightPtr != nullptr)
            {
                pointLightPtr->Use(shaderPtr);
            }
            else if(spotLightPtr != nullptr)
            {
                spotLightPtr->Use(shaderPtr);
            }
            else if(modelPtr != nullptr)
            {
                modelPtr->Draw(shaderPtr);
            }
            else if(modelInstPtr != nullptr)
            {
                modelInstPtr->Draw(shaderPtr);
            }
            else if (textPtr != nullptr)
            {
                textPtr->Draw(shaderPtr);
            }
            else if (linePtr != nullptr)
            {
                linePtr->Draw(shaderPtr);
            }
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

SceneNode* SceneNode::GetRoot()
{
    SceneNode* root = this;
    while(root->GetParent() != nullptr)
    {
        root = root->GetParent();
    }
    return root;
}

bool SceneNode::Is(SceneNode* second)
{
    return this == second;
}

bool SceneNode::Is(std::shared_ptr<SceneNode> second)
{
    return this == second.get();
}
