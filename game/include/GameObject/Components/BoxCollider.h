#ifndef __BOX_COLLIDER_H__
#define __BOX_COLLIDER_H__
#include "ColliderComponent.h"

#include <glm/glm.hpp>

/** @brief Komponent pude³kowych koliderów.
 */
class BoxCollider : public ColliderComponent
{
public:

	/** @brief Konstruktor komponentu koliderów.
	 * @param gameObject - WskaŸnik na GameObject do którego nale¿y komponent.
	 */
	BoxCollider(GameObject*);

	/** @brief Konstruktor komponent koliderów.
	 * @param gameObject - WskaŸnik na GameObject do którego nale¿y komponent.
	 * @param collidersManager - WskaŸnik na menad¿er koliderów.
	 * @param isTrigger - Czy ten kolider ma byæ wyzwalaczem.
	 * @param isStatic -Czy ten kolider ma byæ statyczny.
	 */
	BoxCollider(GameObject* gameObject, CollidersManager* collidersManager, bool isTrigger, bool isStatic);

	/** @brief Bazowy destruktor.
	 */
	~BoxCollider();

	/** @brief Metoda sprawdzaj¹ca kolizjê z innym obiektem.
	* @param collider - wskaŸnik na kolider z którym trzeba sprawdziæ kolizjê.
	* @return true - zasz³a kolizja.
	* @return flase - nie zasz³a kolizja.
	*/
	bool virtual CheckCollision(ColliderComponent* collider);
};
#endif