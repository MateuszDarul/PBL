#pragma once

#include <memory>

#include <glad/glad.h>

#include "Component.h"
#include "Font.h"
#include "ShaderComponent.h"

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


    bool Draw(std::shared_ptr<ShaderComponent> shader);
};
