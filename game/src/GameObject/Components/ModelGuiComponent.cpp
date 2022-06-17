#include "ModelGuiComponent.h"
#include "GameApplication.h"

ModelGuiComponent::ModelGuiComponent()
    :Component(50)
{
    ;
}

ModelGuiComponent::~ModelGuiComponent()
{
    ;
}

bool ModelGuiComponent::Create(const std::string& pathToTexture)
{
    glGenTextures(1, &TexID);
    glBindTexture(GL_TEXTURE_2D, TexID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int nrComponents;
    //stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(pathToTexture.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Texture failed to load at path: " << pathToTexture << std::endl;
        return false;
    }
    stbi_image_free(data);

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

    float xpos = 0.0f;
    float ypos = 0.0f;
    float scale = 0.001f;
    float w = width * scale;
    float h = height * scale;
    Mesh::Vertex vertices[6] = {
        { {xpos,     ypos + h, 0.0},{ 0.0, 0.0,0.0},{   0.0f, 0.0f }},
        { {xpos,     ypos,     0.0},{ 0.0, 0.0,0.0},{   0.0f, 1.0f }},
        { {xpos + w, ypos,     0.0},{ 0.0, 0.0,0.0},{   1.0f, 1.0f }},

        { {xpos,     ypos + h, 0.0},{ 0.0, 0.0,0.0},{   0.0f, 0.0f }},
        { {xpos + w, ypos,     0.0},{ 0.0, 0.0,0.0},{   1.0f, 1.0f }},
        { {xpos + w, ypos + h, 0.0},{ 0.0, 0.0,0.0},{   1.0f, 0.0f }}
    };

    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Mesh::Vertex), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, texCoords));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}

void ModelGuiComponent::Clear()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

bool ModelGuiComponent::Draw(std::shared_ptr<ShaderComponent> shader)
{
    if (alwaysSeen)
    {
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_ALWAYS);
    }

    shader->Use();
    if (isGuiElement) shader->SetMat4("transform", GameApplication::GetOrthoProjection());

    shader->SetInt("diffuseMapData", 0);
    shader->SetVec4("u_TintColor", tintColor);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->TexID);


    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);

    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);


    return true;
}