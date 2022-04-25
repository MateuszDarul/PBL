#pragma once

#include <memory>

#include <glad/glad.h>

#include "Component.h"
#include "Font.h"
#include "ShaderComponent.h"
#include "CameraComponent.h"

class SceneNode;

/** @brief Komponent reprezentujacy tekst w swiecie.
 */
class TextComponent :public Component
{
public:
    glm::vec3 color = {1.0f, 1.0f, 1.0f}; //!< Kolor tekstu
    bool alwaysSeen = false; //!< Flaga, czy widac przez sciany

public:

    TextComponent();
    ~TextComponent();

    /**
     * @brief Tworzy komponent tekstu, generuje VBO i VBA
     * 
     * @param text Tekst do wyswietlenia
     * @param font Wskazanie na zasob czcionki
     * @return true Powodzenie
     * @return false Niepowodzenie
     */
    bool Create(const std::string& text, Font* font);

    /**
     * @brief Czysci bufory
     * 
     */
    void Clear();

    /**
     * @brief Obraca tekst w strone kamery
     * 
     * @param cam Kamera docelowa
     * @param textNode Wezel zawierajacy komponent tekstu
     */
    void FaceCamera(std::shared_ptr<CameraComponent> cam, SceneNode* textNode);

    /**
     * @brief Wyswietla tekst w swiecie
     * 
     * @param shader Shader do renderu tekstu
     * @return true Powodzenie
     * @return false Niepowodzenie
     */
    bool Draw(std::shared_ptr<ShaderComponent> shader);

private:
    unsigned int VAO; //!< Vertex Array Object.
    unsigned int VBO; //!< Vertex Buffer Object.

    std::string text; //!< Tekst do wyswietlenia
    Font* font; //!< Wskazanie na czcionke
};
