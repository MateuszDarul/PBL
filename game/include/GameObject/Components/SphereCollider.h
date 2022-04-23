#ifndef __SPHERE_COLLIDER_H__
#define __SPHERE_COLLIDER_H__
#include "ColliderComponent.h"
#include <glm/glm.hpp>

/** @brief Komponent sferycznych koliderów.
 */
class SphereCollider : public ColliderComponent
{
private:
	float radius;

public:

	/** @brief Konstruktor komponentu koliderów.
	 * @param gameObject - WskaŸnik na GameObject do którego nale¿y komponent.
	 */
	SphereCollider();

	/** @brief Konstruktor komponent koliderów.
	 * @param isTrigger - Czy ten kolider ma byæ wyzwalaczem.
	 * @param isStatic -Czy ten kolider ma byæ statyczny.
	 */
	SphereCollider(bool isTrigger, bool isStatic);

	/** @brief Bazowy destruktor.
	 */
	~SphereCollider();

	/** @brief Metoda sprawdzaj¹ca kolizjê z innym obiektem.
	* @param collider - wskaŸnik na kolider z którym trzeba sprawdziæ kolizjê.
	* @return true - zasz³a kolizja.
	* @return flase - nie zasz³a kolizja.
	*/
	bool virtual CheckCollision(std::shared_ptr<ColliderComponent> collider);
	
	/** @brief Ustawia promieñ sferycznego kolidera.
	* @param radius - Promieñ.
	*/
	void SetRadius(float radius);

	/** @brief Zwraca promieñ sferycznego kolidera.
	* @return float - Promieñ.
	*/
	float GetRadius();
};
#endif