#ifndef __FRUSTUM_H__
#define __FRUSTUM_H__

#include <memory>

#include <glm/glm.hpp>

#include "Plane.h"

/** @brief Klasa reprezentujaca figure o 6 scianach.
 */
class Frustum
{
public:
    Plane topFace; //!< Gorna sciana bryly.
    Plane bottomFace; //!< Dolna sciana bryly.
    Plane leftFace; //!< Lewa sciana bryly.
    Plane rightFace; //!< Prawa sciana bryly.
    Plane farFace; //!< Tylna sciana bryly.
    Plane nearFace; //!< Przednia sciana bryly.

public:
    /** @brief Domyslny konstruktor.
     */
    Frustum();
    
    /** @brief Domyslny destruktor.
     */
    ~Frustum();

    /** @brief Tworzy nowa bryle.
     * @param topFace - Gorna sciana bryly.
     * @param bottomFace - Dolna sciana bryly.
     * @param leftFace - Lewa sciana bryly.
     * @param rightFace - Prawa sciana bryly.
     * @param farFace - Tylna sciana bryly.
     * @param nearFace - Przednia sciana bryly.
     */
    void Create(Plane topFace, Plane bottomFace, Plane leftFace, Plane rightFace, Plane farFace, Plane nearFace);
};

#endif // __FRUSTUM_H__