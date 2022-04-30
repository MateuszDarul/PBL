#ifndef __FRUSTUM_CULLING_COMPONENT_H__
#define __FRUSTUM_CULLING_COMPONENT_H__

#include "Component.h"
#include "ModelComponent.h"
#include "TransformComponent.h"
#include "Mesh.h"
#include "Frustum.h"

/** @brief Komponent decydujacy czy informacje o danym GameObject'cie nalezy przeslac na karte graficzna.
 */
class FrustumCullingComponent :public Component
{
private:
    float radius; //!< Dystans do najbardziej wystunietego wierzcholka.
    std::shared_ptr<TransformComponent> transformComponentPtr; //!< Wskaznik na komponent trtansformacji.

public:
    /** @brief Domyslny konstruktor.
     */
    FrustumCullingComponent();
    
    /** @brief Domyslny destruktor.
     */
    ~FrustumCullingComponent();

    /** @brief 
     * @param mesh - Siatka, dla ktorej ma zostac stworzony komponent.
     * @return true - Komponent zostal stworzony.
     * @return false - Komponent nie zostal stworzony.
     */
    bool Create(Mesh* mesh);

    /** @brief Sprawdza, czy dany obiekt znajduje sie w polu widzenia kamery.
     * @param frustum - Pole widzenia kamery.
     * @return true - Obiekt jest w polu widzenia kamery.
     * @return false - Obiekt nie znajduje sie w polu widzenia kamery.
     */
    bool IsVisible(Frustum frustum);
};

#endif // __FRUSTUM_CULLING_COMPONENT_H__