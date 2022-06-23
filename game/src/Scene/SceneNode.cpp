#include "SceneNode.h"
#include "Scene.h"
#include "ShadowsManager.h"

//#define ENABLE_DEBUG_INFO

#ifdef ENABLE_DEBUG_INFO

static unsigned int boxVAO;
static unsigned int boxVBO;
static bool isDebugInitialized = false;
static ShaderComponent debugShader;

static void initializeDebug()
{
    isDebugInitialized = true;

    glGenVertexArrays(1, &boxVAO);
    glGenBuffers(1, &boxVBO);
    glBindVertexArray(boxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 3, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    debugShader.Create("Resources/shaders/default.vert", "Resources/shaders/default.frag");
}

static void drawDebugBox(const glm::mat4& model, const glm::mat4& vp, const glm::vec3& size, const glm::vec3& offset, const glm::vec4& color)
{
    float cx = model[3][0]+offset.x;
    float cy = model[3][1]+offset.y;
    float cz = model[3][2]+offset.z;
    float x1 = cx + size.x * 0.5f;
    float y1 = cy + size.y * 0.5f;
    float z1 = cz + size.z * 0.5f;
    float x2 = cx - size.x * 0.5f;
    float y2 = cy - size.y * 0.5f;
    float z2 = cz - size.z * 0.5f;
    float vertices[36 * 3] = {
        x1, y1, z1,  x1, y2, z1,  x1, y2, z2,    x1, y1, z1,  x1, y2, z2,  x1, y1, z2,
        x2, y1, z1,  x2, y2, z1,  x2, y2, z2,    x2, y1, z1,  x2, y2, z2,  x2, y1, z2,
        x1, y1, z1,  x2, y1, z1,  x2, y2, z1,    x1, y1, z1,  x2, y2, z1,  x1, y2, z1,
        x1, y1, z2,  x2, y1, z2,  x2, y2, z2,    x1, y1, z2,  x2, y2, z2,  x1, y2, z2,
        x1, y1, z1,  x2, y1, z1,  x2, y1, z2,    x1, y1, z1,  x2, y1, z2,  x1, y1, z2,
        x1, y2, z1,  x2, y2, z1,  x2, y2, z2,    x1, y2, z1,  x2, y2, z2,  x1, y2, z2,
    };

    debugShader.Use();
    debugShader.SetMat4("transform", vp);
    debugShader.SetMat4("model", glm::mat4(1.0f));
    debugShader.SetVec4("u_TintColor", color);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //enable wireframe
    glBindVertexArray(boxVAO);

    glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(GL_TRIANGLES, 0, 36*3);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

static void drawDebugSlope(const glm::mat4& model, const glm::mat4& vp, const std::shared_ptr<SlopeCollider> col, const glm::vec4& color)
{
    float cx = model[3][0]+col->GetOffset().x;
    float cy = model[3][1]+col->GetOffset().y;
    float cz = model[3][2]+col->GetOffset().z;

    glm::vec3 c = { cx, cy, cz };
    const auto& alongX = col->GetAlongX();    
    const auto& alongZ = col->GetAlongZ();

    glm::vec3 p1 = c + col->GetLengthX() * 0.5f * alongX + col->GetLengthZ() * 0.5f * alongZ;
    glm::vec3 p2 = c - col->GetLengthX() * 0.5f * alongX + col->GetLengthZ() * 0.5f * alongZ;
    glm::vec3 p3 = c - col->GetLengthX() * 0.5f * alongX - col->GetLengthZ() * 0.5f * alongZ;
    glm::vec3 p4 = c + col->GetLengthX() * 0.5f * alongX - col->GetLengthZ() * 0.5f * alongZ;

    
    const auto& n = col->GetNormal();
    float nx = cx + n.x * 2.0f;
    float ny = cy + n.y * 2.0f;
    float nz = cz + n.z * 2.0f;
     
    // printf("X: %.2f %.2f %.2f\tZ: %.2f %.2f %.2f\t Normal: %.2f %.2f %.2f\n", alongX.x, alongX.y, alongX.z, alongZ.x, alongZ.y, alongZ.z, n.x, n.y, n.z);

    float vertices[36 * 3] = {
        p1.x, p1.y, p1.z,  p2.x, p2.y, p2.z,  p3.x, p3.y, p3.z,    p3.x, p3.y, p3.z,  p2.x, p2.y, p2.z,  p4.x, p4.y, p4.z,
        
        cx, cy, cz,  cx, cy, cz,  nx, ny, nz,    cx, cy, cz,  cx, cy, cz,  nx, ny, nz,
        cx, cy, cz,  cx, cy, cz,  nx, ny, nz,    cx, cy, cz,  cx, cy, cz,  nx, ny, nz,
        cx, cy, cz,  cx, cy, cz,  nx, ny, nz,    cx, cy, cz,  cx, cy, cz,  nx, ny, nz,
        
        p1.x, p1.y, p1.z,  p4.x, p4.y, p4.z,  p1.x, p1.y, p1.z,p1.x, p1.y, p1.z,  p4.x, p4.y, p4.z,  p1.x, p1.y, p1.z,
        cx, cy, cz,  cx, cy, cz,  nx, ny, nz,    cx, cy, cz,  cx, cy, cz,  nx, ny, nz,
    };

    debugShader.Use();
    debugShader.SetMat4("transform", vp);
    debugShader.SetMat4("model", glm::mat4(1.0f));
    debugShader.SetVec4("u_TintColor", color);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //enable wireframe
    glBindVertexArray(boxVAO);

    glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(GL_TRIANGLES, 0, 36*3);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

//not a sphere lol
static void drawDebugSphere(const glm::mat4& model, const glm::mat4& vp, float radius, const glm::vec3& offset, const glm::vec4& color)
{
    float cx = model[3][0]+offset.x;
    float cy = model[3][1]+offset.y;
    float cz = model[3][2]+offset.z;
    float x1 = cx + radius;
    float y1 = cy + radius;
    float z1 = cz + radius;
    float x2 = cx - radius;
    float y2 = cy - radius;
    float z2 = cz - radius;
    float vertices[36 * 3] = {
        x1, y1, z1,  x1, y2, z1,  x1, y2, z2,    x1, y1, z1,  x1, y2, z2,  x1, y1, z2,
        x2, y1, z1,  x2, y2, z1,  x2, y2, z2,    x2, y1, z1,  x2, y2, z2,  x2, y1, z2,
        x1, y1, z1,  x2, y1, z1,  x2, y2, z1,    x1, y1, z1,  x2, y2, z1,  x1, y2, z1,
        x1, y1, z2,  x2, y1, z2,  x2, y2, z2,    x1, y1, z2,  x2, y2, z2,  x1, y2, z2,
        x1, y1, z1,  x2, y1, z1,  x2, y1, z2,    x1, y1, z1,  x2, y1, z2,  x1, y1, z2,
        x1, y2, z1,  x2, y2, z1,  x2, y2, z2,    x1, y2, z1,  x2, y2, z2,  x1, y2, z2,
    };

    debugShader.Use();
    debugShader.SetMat4("transform", vp);
    debugShader.SetMat4("model", glm::mat4(1.0f));
    debugShader.SetVec4("u_TintColor", color);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //enable wireframe
    glBindVertexArray(boxVAO);

    glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(GL_TRIANGLES, 0, 36*3);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


#endif


// defined in Scene.cpp
extern std::shared_ptr<GameObject> GO_MULTITOOL;
extern std::shared_ptr<GameObject> GO_FLASHLIGHT;
extern std::shared_ptr<GameObject> GO_CROSSHAIR;

Frustum SceneNode::cameraFrustum;
std::vector<SceneNode::NodeToDelete> SceneNode::nodesToDelete;

SceneNode::SceneNode(std::shared_ptr<GameObject> gameObject)
    :parent(nullptr), gameObject(gameObject)
{
    this->globalTransformations = glm::mat4(1.f);
    gameObject->SetNode(this);

#ifdef ENABLE_DEBUG_INFO
    if(!isDebugInitialized)
        initializeDebug();
#endif
}

SceneNode::~SceneNode()
{

}

std::shared_ptr<SceneNode> SceneNode::AddChild(std::shared_ptr<GameObject> gameObject)
{
    auto node = std::make_shared<SceneNode>(gameObject);
    return this->AddChild(node);
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

void SceneNode::Update(float dt)
{
#ifdef ENABLE_DEBUG_INFO
    if (Input()->Keyboard()->IsPressed(KeyboardKey::F11))
    {
        const auto cam = this->GetRoot()->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Camera>();
        const auto& pos = cam->GetPosition();
        float pitch = cam->GetPitch();
        float yaw = cam->GetYaw();
        printf("Camera position: %f %f %f \tPitch = %f, Yaw = %f\n", pos.x, pos.y, pos.z, pitch, yaw);
    }
#endif

    this->PrivateUpdate(dt, glm::mat4(1.f));

    SceneNode::cameraFrustum = 
    this->GetRoot()->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Camera>()->GetFrustum(
        (float)GameApplication::GetWindowSize().x/GameApplication::GetWindowSize().y,
        GameApplication::GetFov(),
        GameApplication::GetProjectionRange().x,
        GameApplication::GetProjectionRange().y
    );

    this->DeleteNodes();
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
            shaderPtr->SetFloat("u_Time", GameApplication::GetTotalElapsedTime());
            shaderPtr->SetVec3("u_CameraPos", this->GetRoot()->FindNode("CAMERA")->GetGameObject()->GetComponent<cmp::Transform>()->GetPosition());

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
                            textPtr->Draw(shaderPtr);
                        }
                        else
                        {
                            std::shared_ptr<cmp::Line> linePtr = this->gameObject->GetComponent<cmp::Line>();
                            if (linePtr != nullptr)
                            {
                                linePtr->Draw(shaderPtr);
                            }
                            else
                            {
                                std::shared_ptr<cmp::ModelGui> modelGui = this->gameObject->GetComponent<cmp::ModelGui>();
                                if (modelGui != nullptr)
                                {
                                    modelGui->Draw(shaderPtr);
                                }
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

#ifdef ENABLE_DEBUG_INFO
    /*if (Input()->Keyboard()->IsPressed(KeyboardKey::F9))
    {
        if (auto collider = gameObject->GetComponent<cmp::BoxCol>())
        {
            glm::vec4 color = (collider->isTrigger) ? glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) : glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
            drawDebugBox(this->globalTransformations, matrixPV, collider->GetLengths(), collider->GetOffset(), color);
        }
        else if (auto collider = gameObject->GetComponent<cmp::SphereCol>())
        {
            glm::vec4 color = (collider->isTrigger) ? glm::vec4(1.0f, 1.0f, 0.7f, 1.0f) : glm::vec4(0.7f, 1.0f, 0.7f, 1.0f);
            drawDebugSphere(this->globalTransformations, matrixPV, collider->GetRadius(), collider->GetOffset(), color);
        }
        else if (auto collider = gameObject->GetComponent<cmp::SlopeCol>())
        {
            glm::vec4 color = (collider->isTrigger) ? glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) : glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
            drawDebugSlope(this->globalTransformations, matrixPV, collider, color);
        }
    }*/

#endif

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

const glm::mat4& SceneNode::GetGlobalTransformations()
{
    return this->globalTransformations;
}

glm::vec3 SceneNode::GetGlobalPosition()
{
    return { globalTransformations[3][0], globalTransformations[3][1], globalTransformations[3][2] };
}

void SceneNode::SetGlobalTransformations(const glm::mat4& newTransformations)
{
    this->globalTransformations = newTransformations;
}

void SceneNode::ResetGlobalTransformations()
{
    this->globalTransformations = glm::mat4(1.0f) * GetLocalTransformations()->GetModelMatrix();
}

SceneNode* SceneNode::FindNode(const std::string& name)
{
    SceneNode* lastSearchedNode = nullptr;
    FindByName(name, &lastSearchedNode);
    return lastSearchedNode;
}

bool SceneNode::RemoveNode(std::shared_ptr<SceneNode> node)
{
    return RemoveNode(node.get());;
}

bool SceneNode::RemoveNode(SceneNode* node)
{
    if (node != nullptr && !this->Is(node))
    {
        for (unsigned short int i = 0; i < this->children.size(); i++)
        {
            if (this->children[i] && this->children[i]->Is(node))
            {
                if (std::find_if(nodesToDelete.begin(), nodesToDelete.end(), [&](NodeToDelete n) { return n.child->Is(node); }) != nodesToDelete.end()) return true;

                nodesToDelete.push_back({ this, node });
                return true;
            }
        }
    }
    return false;
}

void SceneNode::DeleteNodes()
{
    for (auto& nodeToDelete : nodesToDelete)
    {
        nodeToDelete.child->PrivateDelete();
        for (unsigned short int i = 0; i < nodeToDelete.parent->children.size(); i++)
        {
            if (nodeToDelete.parent->children[i] && nodeToDelete.parent->children[i]->Is(nodeToDelete.child))
            {
                nodeToDelete.parent->children[i].reset();
                nodeToDelete.parent->children.erase(nodeToDelete.parent->children.begin() + i, nodeToDelete.parent->children.begin() + i + 1);
                break;
            }
        }
    }

    nodesToDelete.clear();
}

void SceneNode::PrivateDelete()
{
    if (auto boxCol = this->GetGameObject()->GetComponent<cmp::BoxCol>())
    {
        boxCol->RemoveFromCollidersManager(boxCol);
    }
    if (auto sphereCol = this->GetGameObject()->GetComponent<cmp::SphereCol>())
    {
        sphereCol->RemoveFromCollidersManager(sphereCol);
    }
    if (auto slopeCol = this->GetGameObject()->GetComponent<cmp::SlopeCol>())
    {
        slopeCol->RemoveFromCollidersManager(slopeCol);
    }
    if (auto scriptable = this->GetGameObject()->GetComponent<cmp::Scriptable>())
    {
        scriptable->Clear();
    }
    if (auto pointLight = this->GetGameObject()->GetComponent<cmp::PointLight>())
    {
        Scene::GetSceneInfo().shadowsManager->RemoveLight(this->GetGameObject().get());
        pointLight->Destroy();
    }

    this->GetGameObject()->SetDestroyed();
    
    for (unsigned short int i = 0; i < this->children.size(); i++)
    {
        children[i]->PrivateDelete();
    }
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
