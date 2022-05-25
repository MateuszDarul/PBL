#ifndef __REFRACTIVE_GLASS_COMPONENT_H__
#define __REFRACTIVE_GLASS_COMPONENT_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Component.h"

#include "ShaderComponent.h"
#include "Mesh.h"
#include "Material.h"

/** @brief Klasa renderujaca szklo z refrakcja.
 */
class RefractiveGlassComponent :public Component
{
private:
    unsigned int VAO; //!< Vertex Array Object.
    unsigned int VBO; //!< Vertex Buffer Object.
    unsigned int skyboxTexture;
    float ratio;

    Mesh* mesh; //!< Siatka wykorzysywana przez model.

public:
    /** @brief Domyslny konstruktor.
     */
    RefractiveGlassComponent();

    /** @brief Domyslny destruktor.
     */
    ~RefractiveGlassComponent();

    /** @brief Tworzy model i jego instancje.
     * @param amount - Liczba instancji modelu.
     * @param mesh - Wskazanie na siatke, ktora ma zostac wykorzystana.
     * @param material - Wskazanie na material, ktory ma byc wykorzystywany.
     * @return true - Udalo sie stworzyc nowe instancje modelu.
     * @return false - Wystapil blad podczas tworzenia instancji modelu.
     */
    bool Create(Mesh* mesh, float ratio, unsigned int skyboxTexture);


    /** @brief Wyswietla wszystkie instancje modelu.
     * @param shader - Wskazanie na Shader,z ktorego korzysta model.
     * @return true - Udalo sie wyswietlic instancje modelu.
     * @return false - Nie udalo sie wyswietlic instancji modelu.
     */
    bool Draw(std::shared_ptr<ShaderComponent> shader);

};

#endif // __MODEL_INSTANCED_COMPONENT_H__