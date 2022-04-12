#ifndef __COLLIDER_COMPONENT_H__
#define __COLLIDER_COMPONENT_H__
#include "Component.h"

#include <glm/glm.hpp>


class CollidersManager;
class GameObject;

/** @brief Abstrakcyjny komponent kolider�w.
 */
class ColliderComponent : public Component
{
protected:
	glm::vec3 offset; //!< Przesuni�cie kolidera wzgl�dem bazowego obiektu.

public:
	bool isStatic; //!< Czy ten kolider ma by� statyczny.
	bool isTrigger; //!< Czy ten kolider ma by� wyzwalaczem.

	/** @brief Konstruktor komponent kolider�w.
	 * @param UUID - Unikalny indentyfikator klasy komponentu.
	 * @param gameObject - Wska�nik na GameObject do kt�rego nale�y komponent.
	 * @param collidersManager - Wska�nik na menad�er kolider�w.
	 * @param isTrigger - Czy ten kolider ma by� wyzwalaczem.
	 * @param isStatic -Czy ten kolider ma by� statyczny.
	 */
	ColliderComponent(uint32_t UUID, GameObject* gameObject, CollidersManager* collidersManager, bool isTrigger, bool isStatic);
	
	/** @brief Bazowy destruktor
	*/
	~ColliderComponent();

	/** @brief Zwraca przesuni�cie kolidera.
	* @return glm::vec3 - Wektor przesuni�cia.
	*/
	glm::vec3 GetOffset();
	
	/** @brief Ustawia przesuni�cie kolidera.
	* @param glm::vec3 - Wektor przesuni�cia.
	*/
	void SetOffset(glm::vec3 offset);

	/** @brief Abstrakcyjna metoda sprawdzaj�ca kolizj� z innym obiektem.
	* @param collider - wska�nik na kolider z kt�rym trzeba sprawdzi� kolizj�.
	* @return true - zasz�a kolizja.
	* @return flase - nie zasz�a kolizja.
	*/
	bool virtual CheckCollision(ColliderComponent* collider) = 0;
};
#endif