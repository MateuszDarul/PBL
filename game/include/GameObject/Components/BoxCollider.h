#ifndef __BOX_COLLIDER_H__
#define __BOX_COLLIDER_H__
#include "ColliderComponent.h"

#include <glm/glm.hpp>

/** @brief Komponent pude�kowych kolider�w.
 */
class BoxCollider : public ColliderComponent
{
public:

	/** @brief Konstruktor komponentu kolider�w.
	 * @param gameObject - Wska�nik na GameObject do kt�rego nale�y komponent.
	 */
	BoxCollider(GameObject*);

	/** @brief Konstruktor komponent kolider�w.
	 * @param gameObject - Wska�nik na GameObject do kt�rego nale�y komponent.
	 * @param collidersManager - Wska�nik na menad�er kolider�w.
	 * @param isTrigger - Czy ten kolider ma by� wyzwalaczem.
	 * @param isStatic -Czy ten kolider ma by� statyczny.
	 */
	BoxCollider(GameObject* gameObject, CollidersManager* collidersManager, bool isTrigger, bool isStatic);

	/** @brief Bazowy destruktor.
	 */
	~BoxCollider();

	/** @brief Metoda sprawdzaj�ca kolizj� z innym obiektem.
	* @param collider - wska�nik na kolider z kt�rym trzeba sprawdzi� kolizj�.
	* @return true - zasz�a kolizja.
	* @return flase - nie zasz�a kolizja.
	*/
	bool virtual CheckCollision(ColliderComponent* collider);
};
#endif