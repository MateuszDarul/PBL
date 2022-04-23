#ifndef __MODEL_INSTANCED_COMPONENT_H__
#define __MODEL_INSTANCED_COMPONENT_H__

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Component.h"

#include "ShaderComponent.h"
#include "Mesh.h"
#include "Material.h"

/** @brief Klasa pozwalajaca na wyswietlanie wielu instancji tego samego modelu w roznych miejscach, rozmiarach i rotacji.
 */
class ModelInstancesComponent :public Component
{
private:
    uint32_t amount; //!< Liczba instancji, ktore beda wyswietlane.
    unsigned int VAO; //!< Vertex Array Object.
    unsigned int VBO; //!< Vertex Buffer Object.
    uint32_t instanceVBO; //! VBO, ktore przechowuje transformacje dla kazdej instancji modelu.

    Mesh* mesh; //!< Siatka wykorzysywana przez model.
    Material* material; //!< Material wykorzysywany przez model.
    glm::mat4 *transformations; //!< Tablica transformacji kazdej instancji modelu.

public:
    /** @brief Domyslny konstruktor.
     */
    ModelInstancesComponent();
    
    /** @brief Domyslny destruktor.
     */
    ~ModelInstancesComponent();

    /** @brief Tworzy model i jego instancje.
     * @param amount - Liczba instancji modelu.
     * @param mesh - Wskazanie na siatke, ktora ma zostac wykorzystana.
     * @param material - Wskazanie na material, ktory ma byc wykorzystywany.
     * @return true - Udalo sie stworzyc nowe instancje modelu.
     * @return false - Wystapil blad podczas tworzenia instancji modelu.
     */
    bool Create(uint32_t amount, Mesh* mesh, Material* material);

    /** @brief Aktualizuje transformacje dla wszystkich instancji.
     */
    void UpdateTransformations();

    /** @brief Wyswietla wszystkie instancje modelu.
     * @param shader - Wskazanie na Shader,z ktorego korzysta model.
     * @return true - Udalo sie wyswietlic instancje modelu.
     * @return false - Nie udalo sie wyswietlic instancji modelu.
     */
    bool Draw(std::shared_ptr<ShaderComponent> shader);

    /** @brief Zwraca ile instancji posiada ten model.
     * @return uint32_t - Liczba instancji.
     */
    uint32_t GetAmount();

    /** @brief Zmienia transformacje wybranej instancji modelu.
     * @param instanceID - Numer instancji.
     * @param newTransformations - Nowe transformacjie.
     * @return true - Zmieniono transformacjie (Pamietaj o ModelInstancesComponent::UpdateTransformations()).
     * @return false - Nie zmieniono transformacji.
     */
    bool SetTransformation(uint32_t instanceID, glm::mat4 newTransformations);

    /** @brief Zmienia transformacje wybranej instancji modelu, po przez podanie skladowych.
     * @param instanceID - Numer instancji.
     * @param position - Nowa pozycja instancji.
     * @param rotation - Nowa rotacja instancji.
     * @param scale - Nowa skala instancji.
     * @return true - Zmieniono transformacjie (Pamietaj o ModelInstancesComponent::UpdateTransformations()).
     * @return false - Nie zmieniono transformacji.
     */
    bool SetTransformation(uint32_t instanceID, glm::vec3 position = glm::vec3(0,0,0), glm::vec3 rotation = glm::vec3(0,0,0), float scale = 1);

    /** @brief Zwraca macierz transformacji wybranej instancji modelu.
     * @param instanceID - Numer instancji.
     * @return glm::mat4 - Macierz transformacji.
     */
    glm::mat4 GetTransformation(uint32_t instanceID);
};

#endif // __MODEL_INSTANCED_COMPONENT_H__