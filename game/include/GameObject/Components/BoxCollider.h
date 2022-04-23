#ifndef __BOX_COLLIDER_H__
#define __BOX_COLLIDER_H__
#include "ColliderComponent.h"

#include <glm/glm.hpp>

/** @brief Komponent pude³kowych koliderów.
 */
class BoxCollider : public ColliderComponent
{
private:
	glm::uvec3 lengths; //!< D³ugoœci boków pude³ka, równoleg³e do osi X, Y lub Z.
public:

	/** @brief Konstruktor komponentu koliderów.
	 */
	BoxCollider();

	/** @brief Konstruktor komponent koliderów.
	 * @param isTrigger - Czy ten kolider ma byæ wyzwalaczem.
	 * @param isStatic -Czy ten kolider ma byæ statyczny.
	 */
	BoxCollider(bool isTrigger, bool isStatic);

	/** @brief Bazowy destruktor.
	 */
	~BoxCollider();

	/** @brief Metoda sprawdzaj¹ca kolizjê z innym obiektem.
	* @param collider - wskaŸnik na kolider z którym trzeba sprawdziæ kolizjê.
	* @return true - zasz³a kolizja.
	* @return flase - nie zasz³a kolizja.
	*/
	bool virtual CheckCollision(std::shared_ptr<ColliderComponent> collider);

	/** @brief Metoda ustawiaj¹ca nowe d³ugoœci krawêdzi pude³kowego kolidera.
	* @param lengths - nowe d³ugoœci krawêdzi pude³kowego kolidera.
	*/
	void setLengths(glm::uvec3 lengths);

	/** @brief Metoda zwracaj¹ca d³ugoœci krawêdzi pude³kowego kolidera.
	* @return glm::uvec3 - D³ugoœci krawêdzi.
	*/
	glm::uvec3 getLengths();
};
#endif