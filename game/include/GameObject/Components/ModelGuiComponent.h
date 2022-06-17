#ifndef __MODEL_GUI_COMPONENT_H__
#define __MODEL_GUI_COMPONENT_H__

#include <memory>

#include <glad/glad.h>

#include "Component.h"

#include "Mesh.h"
#include "Material.h"
#include "ShaderComponent.h"

/** @brief Komponent reprezentujacy model gui.
 */
class ModelGuiComponent :public Component
{
public:
    glm::vec4 tintColor = { 1.0f, 1.0f, 1.0f, 1.0f }; //!< Kolor gui
    bool alwaysSeen = false; //!< Flaga, czy widac przez sciany
    bool isGuiElement = false; //<! Flaga, czy uzywac projekcji prostokatnej
private:
    unsigned int VAO; //!< Vertex Array Object.
    unsigned int VBO; //!< Vertex Buffer Object.
    unsigned int TexID; //!< Texture id
    int width; //!< Texture width
    int height; //!< Texture height

public:
    /** @brief Domyslny konstruktor.
     */
    ModelGuiComponent();

    /** @brief Domyslny destruktor.
     */
    ~ModelGuiComponent();

    /** @brief Tworzy nowy model.
     * @return true - Stworzono nowy model.
     * @return false - Wystapil blad podczas tworzenia modelu.
     */
    bool Create(const std::string& pathToTexture);

    /** @brief Usuwa wybrany model.
     */
    void Clear();

    /** @brief Wyswietla model.
     * @param shader - Wskazanie na shader uzywany dla danego modelu.
     * @return true - Udalo sie wyswietlic model.
     * @return false - Nie udalo sie wyswietlic modelu.
     */
    bool Draw(std::shared_ptr<ShaderComponent> shader);
};

#endif // __MODEL_COMPONENT_H__