#pragma once

#include <memory>

#include <glad/glad.h>

#include "Component.h"
#include "Font.h"
#include "ShaderComponent.h"
#include "CameraComponent.h"

class SceneNode;

/** @brief Komponent reprezentujacy model.
 */
class TextComponent :public Component
{
private:
    unsigned int VAO; //!< Vertex Array Object.
    unsigned int VBO; //!< Vertex Buffer Object.

    std::string text;
    Font* font; //!< Wskazanie na siatke, z ktorej korzysta model.
    

public:

    TextComponent();


    ~TextComponent();


    bool Create(const std::string& text, Font* font);


    void Clear();

    void FaceCamera(std::shared_ptr<CameraComponent>, SceneNode*);
    bool Draw(std::shared_ptr<ShaderComponent> shader);


    glm::vec3 color = {1.0f, 1.0f, 1.0f};
    bool alwaysSeen = false;
};
