#ifndef __BOX_COLLIDER_H__
#define __BOX_COLLIDER_H__
#include "ColliderComponent.h"

#include <glm/glm.hpp>



/** @brief Komponent pude�kowych kolider�w.
 */
class BoxCollider : public ColliderComponent
{
private:
	glm::uvec3 lengths; //!< D�ugo�ci bok�w pude�ka, r�wnoleg�e do osi X, Y lub Z.
public:

	/** @brief Konstruktor komponentu kolider�w.
	 */
	BoxCollider();

	/** @brief Konstruktor komponent kolider�w.
	 * @param isTrigger - Czy ten kolider ma by� wyzwalaczem.
	 * @param isStatic -Czy ten kolider ma by� statyczny.
	 * @param layer - warstwa kolidera.
	 */
	BoxCollider(bool isTrigger, bool isStatic, int layer = CollisionLayer::Default);

	/** @brief Bazowy destruktor.
	 */
	~BoxCollider();

	/** @brief Metoda sprawdzaj�ca kolizj� z innym obiektem.
	* @param collider - wska�nik na kolider z kt�rym trzeba sprawdzi� kolizj�.
	* @return true - zasz�a kolizja.
	* @return flase - nie zasz�a kolizja.
	*/
	bool virtual CheckCollision(std::shared_ptr<ColliderComponent> collider);

	/*!
	 * @copybrief ColliderComponent::RayCollision()
	 */
	virtual bool RayCollision(const glm::vec3& origin, const glm::vec3& dir, RayHitInfo& hitInfo, float maxDistance);

	/** @brief Metoda ustawiaj�ca nowe d�ugo�ci kraw�dzi pude�kowego kolidera.
	* @param lengths - nowe d�ugo�ci kraw�dzi pude�kowego kolidera.
	*/
	void setLengths(glm::uvec3 lengths);

	/** @brief Metoda zwracaj�ca d�ugo�ci kraw�dzi pude�kowego kolidera.
	* @return glm::uvec3 - D�ugo�ci kraw�dzi.
	*/
	glm::uvec3 getLengths();
};
#endif