#include "SceneNode.h"

Frustum SceneNode::cameraFrustum;

SceneNode::SceneNode(std::shared_ptr<GameObject> gameObject)
    :parent(nullptr), gameObject(gameObject)
{
    this->globalTransformations = glm::mat4(1.f);
    gameObject->SetNode(this);
}

SceneNode::~SceneNode()
{

}

std::shared_ptr<SceneNode> SceneNode::AddChild(std::shared_ptr<GameObject> gameObject)
{
    auto node = std::make_shared<SceneNode>(gameObject);
    this->AddChild(node);
    return node;
}

std::shared_ptr<SceneNode> SceneNode::AddChild(std::shared_ptr<SceneNode> sceneNode)
{
    sceneNode->SetParent(this);
    this->children.push_back(sceneNode);
    return sceneNode;
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
static float GlobalElapsedTime = 0.0f;
void SceneNode::Update(float dt)
{
    GlobalElapsedTime += dt;
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

    std::shared_ptr<cmp::Particles> particlePtr = this->gameObject->GetComponent<cmp::Particles>();
    if (particlePtr != nullptr)
    {
        particlePtr->Update(dt);
    }
    ///***

    for(unsigned short int i=0; i<children.size(); i++)
    {
        this->children[i]->PrivateUpdate(dt, this->globalTransformations);
    }
    
}

void SceneNode::RenderDepthMap(std::shared_ptr<ShaderComponent> shader)
{
    std::shared_ptr<cmp::Shade> shadePtr = this->gameObject->GetComponent<cmp::Shade>();
    if(shadePtr != nullptr && shadePtr->ShadowTest())
    {
        shader->Use();
        shader->SetMat4("model", this->globalTransformations);

        std::shared_ptr<cmp::Model> modelPtr = this->gameObject->GetComponent<cmp::Model>();
        std::shared_ptr<cmp::ModelInst> modelInstPtr = this->gameObject->GetComponent<cmp::ModelInst>();

        if(modelPtr != nullptr)
        {
            shader->SetBool("inst", false);
            modelPtr->Draw(shader);
        }
        else if(modelInstPtr != nullptr)
        {
            shader->SetBool("inst", true);
            modelInstPtr->Draw(shader);
        }
    }

    for(unsigned short int i=0; i<this->children.size(); i++)
    {
        this->children[i]->RenderDepthMap(shader);
    }
}

void SceneNode::Render(const glm::mat4& matrixPV)
{
    std::shared_ptr<cmp::Shader> shaderPtr = this->gameObject->GetComponent<cmp::Shader>();
    if(shaderPtr != nullptr)
    {
        bool display = true;
        std::shared_ptr<FrustumCullingComponent> frustumCullingPtr = this->gameObject->GetComponent<FrustumCullingComponent>();
        if(frustumCullingPtr != nullptr)
        {
            display = frustumCullingPtr->IsVisible(SceneNode::cameraFrustum);
        }

        if(display)
        {
            shaderPtr->Use();
            shaderPtr->SetMat4("transform", matrixPV);
            shaderPtr->SetMat4("model", this->globalTransformations);
            shaderPtr->SetVec4("u_TintColor", {1, 1, 1, 1});
            shaderPtr->SetFloat("u_Time", GlobalElapsedTime);
            shaderPtr->SetVec3("u_CameraPos", GetRoot()->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Transform>()->GetPosition());

            std::shared_ptr<cmp::Model> modelPtr = this->gameObject->GetComponent<cmp::Model>();
            if(modelPtr != nullptr)
            {
                modelPtr->Draw(shaderPtr);
            }
            else
            {
                std::shared_ptr<cmp::Refract> refractPtr = this->gameObject->GetComponent<cmp::Refract>();
                if (refractPtr != nullptr)
                {
                    glm::vec3 camPos = this->GetRoot()->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Camera>()->GetPosition();
                    shaderPtr->SetVec3("cameraPos", camPos);
                    refractPtr->Draw(shaderPtr);
                }
                else
                {
                    std::shared_ptr<cmp::ModelInst> modelInstPtr = this->gameObject->GetComponent<cmp::ModelInst>();
                    if (modelInstPtr != nullptr)
                    {
                        modelInstPtr->Draw(shaderPtr);
                    }
                    else
                    {
                        std::shared_ptr<cmp::Text> textPtr = this->gameObject->GetComponent<cmp::Text>();
                        if (textPtr != nullptr)
                        {
                            shaderPtr->SetMat4("transform", glm::ortho(0.0f, 16.0f, 0.0f, 9.0f)); //ta kolejnosc naprawia tekst do góry nogami

                            textPtr->Draw(shaderPtr);
                        }
                        else
                        {
                            std::shared_ptr<cmp::Line> linePtr = this->gameObject->GetComponent<cmp::Line>();
                            if (linePtr != nullptr)
                            {
                                linePtr->Draw(shaderPtr);
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        std::shared_ptr<cmp::PointLight> pointLightPtr = this->gameObject->GetComponent<cmp::PointLight>();
        if(pointLightPtr != nullptr)
        {
            pointLightPtr->Use();
        }
        else 
        {
            std::shared_ptr<cmp::SpotLight> spotLightPtr = this->gameObject->GetComponent<cmp::SpotLight>();
            if(spotLightPtr != nullptr)
            {
                spotLightPtr->Use();
            }
        }
    }

    std::shared_ptr<cmp::Particles> particlePtr = this->gameObject->GetComponent<cmp::Particles>();
    if (particlePtr != nullptr)
    {
        particlePtr->Draw(matrixPV);
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
