#ifndef __MODEL_COMPONENT_H__
#define __MODEL_COMPONENT_H__

#include <memory>

#include <glad/glad.h>

#include "Component.h"

#include "Mesh.h"
#include "Material.h"
#include "ShaderComponent.h"

/** @brief Komponent reprezentujacy model.
 */
class ModelComponent :public Component
{
private:
    unsigned int VAO; //!< Vertex Array Object.
    unsigned int VBO; //!< Vertex Buffer Object.

    Mesh* mesh; //!< Wskazanie na siatke, z ktorej korzysta model.
    Material* material; //!< Wskazanie na material, z ktorego korzysta model.

    glm::vec4 tintColor = {1,1,1,1};
public:
    /** @brief Domyslny konstruktor.
     */
    ModelComponent();
    
    /** @brief Domyslny destruktor.
     */
    ~ModelComponent();

    /** @brief Tworzy nowy model.
     * @param mesh - Wskazanie na siatke, z ktorej bedzie korzystal model.
     * @param material - Wskazanie na material, z ktorego bedzie korzystal model.
     * @return true - Stworzono nowy model.
     * @return false - Wystapil blad podczas tworzenia modelu.
     */
    bool Create(Mesh* mesh, Material* material);
    
    /** @brief Usuwa wybrany model.
     */
    void Clear();

    /** @brief Wyswietla model.
     * @param shader - Wskazanie na shader uzywany dla danego modelu.
     * @return true - Udalo sie wyswietlic model.
     * @return false - Nie udalo sie wyswietlic modelu.
     */
    bool Draw(std::shared_ptr<ShaderComponent> shader);


    void SetTintColor(const glm::vec4& color);
    void SetTintColor(float r, float g, float b, float a = 1.0f);
    const glm::vec4& GetTintColor();
};

#endif // __MODEL_COMPONENT_H__