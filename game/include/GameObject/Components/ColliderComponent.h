#ifndef __COLLIDER_COMPONENT_H__
#define __COLLIDER_COMPONENT_H__
#include "Component.h"

#include <glm/glm.hpp>


class CollidersManager;
class GameObject;

/** @brief Abstrakcyjny komponent koliderów.
 */
class ColliderComponent : public Component
{
protected:
	glm::vec3 offset; //!< Przesuniêcie kolidera wzglêdem bazowego obiektu.

public:
	bool isStatic; //!< Czy ten kolider ma byæ statyczny.
	bool isTrigger; //!< Czy ten kolider ma byæ wyzwalaczem.

	/** @brief Konstruktor komponent koliderów.
	 * @param UUID - Unikalny indentyfikator klasy komponentu.
	 * @param gameObject - WskaŸnik na GameObject do którego nale¿y komponent.
	 * @param collidersManager - WskaŸnik na menad¿er koliderów.
	 * @param isTrigger - Czy ten kolider ma byæ wyzwalaczem.
	 * @param isStatic -Czy ten kolider ma byæ statyczny.
	 */
	ColliderComponent(uint32_t UUID, GameObject* gameObject, CollidersManager* collidersManager, bool isTrigger, bool isStatic);
	
	/** @brief Bazowy destruktor
	*/
	~ColliderComponent();

	/** @brief Zwraca przesuniêcie kolidera.
	* @return glm::vec3 - Wektor przesuniêcia.
	*/
	glm::vec3 GetOffset();
	
	/** @brief Ustawia przesuniêcie kolidera.
	* @param glm::vec3 - Wektor przesuniêcia.
	*/
	void SetOffset(glm::vec3 offset);

	/** @brief Abstrakcyjna metoda sprawdzaj¹ca kolizjê z innym obiektem.
	* @param collider - wskaŸnik na kolider z którym trzeba sprawdziæ kolizjê.
	* @return true - zasz³a kolizja.
	* @return flase - nie zasz³a kolizja.
	*/
	bool virtual CheckCollision(ColliderComponent* collider) = 0;
};
#endif