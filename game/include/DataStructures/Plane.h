#ifndef __PLANE_H__
#define __PLANE_H__

#include <glm/glm.hpp>

/** @brief Klasa reprezentujaca plaszczyzne;
 */
class Plane
{
private:
    glm::vec3 normal; //<! Znormalizowany kierunek, w ktorym zwrocona jest plaszczyzna.
    float distance; //<! Wartosc, o ktora plaszczyzna jest przesunieta od poczatku ukladu wsp.

public:
    /** @brief Domyslny konstruktor.
     */
    Plane();
    
    /** @brief Domyslny destruktor.
     */
    ~Plane();

    /** @brief Tworzy nowa plaszczyzne.
     * @param point - Punkt nalezacy do plaszczyzny.
     * @param direction - Kierunek w ktory zwrocona jest plaszczyzna.
     */
    Plane(const glm::vec3& point, const glm::vec3& direction);

    /** @brief Get the Distance To Plane object
     * @param point - Punkt, do ktorego dystans chcemy policzyc.
     * @return float - Dystans danego punktu do plaszczyzny.
     */
    float Distance(const glm::vec3& point);
};

#endif // __PLANE_H__