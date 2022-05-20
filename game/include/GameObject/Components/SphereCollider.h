#ifndef __SPHERE_COLLIDER_H__
#define __SPHERE_COLLIDER_H__
#include "ColliderComponent.h"
#include <glm/glm.hpp>

/** @brief Komponent sferycznych kolider�w.
 */
class SphereCollider : public ColliderComponent
{
private:
	float radius;

public:

	/** @brief Konstruktor komponentu kolider�w.
	 * @param gameObject - Wska�nik na GameObject do kt�rego nale�y komponent.
	 */
	SphereCollider();

	/** @brief Konstruktor komponent kolider�w.
	 * @param isTrigger - Czy ten kolider ma by� wyzwalaczem.
	 * @param isStatic -Czy ten kolider ma by� statyczny.
	 */
	SphereCollider(bool isTrigger, bool isStatic, int layer = CollisionLayer::Default);

	/** @brief Bazowy destruktor.
	 */
	~SphereCollider();

	/** @brief Metoda sprawdzaj�ca kolizj� z innym obiektem.
	* @param collider - wska�nik na kolider z kt�rym trzeba sprawdzi� kolizj�.
	* @return true - zasz�a kolizja.
	* @return flase - nie zasz�a kolizja.
	*/
	bool virtual CheckCollision(std::shared_ptr<ColliderComponent> collider);
	virtual bool RayCollision(const glm::vec3& origin, const glm::vec3& dir, RayHitInfo& hitInfo, float maxDistance);
	
	/** @brief Ustawia promie� sferycznego kolidera.
	* @param radius - Promie�.
	*/
	void SetRadius(float radius);

	/** @brief Zwraca promie� sferycznego kolidera.
	* @return float - Promie�.
	*/
	float GetRadius();
};
#endif