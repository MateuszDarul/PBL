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
	float mass; //!< Masa obiektu do którego podczepiony jest kolider.

	/** @brief Zwraca indeks najmniejszej wartoœci z tablicy z d³ugoœciami wektorów rozsuniêcia.
	 * @param array - Tablica z 6 d³ugoœciami potencjalnych wektorów rozsuniêcia.
	 * @return int - indeks najmniejszej wartoœci z tablicy.
	 */
	int GetLowestValueIndex(float array[6]);

	/** @brief Ustawia wektory rozsuniêcia na podstawie tablicy z d³ugoœciami potencjalnych wektorów rozsuniêcia, gdy przebiega ono wzd³u¿ osi X, Y lub Z.
	 * @param array - Tablica z 6 d³ugoœciami potencjalnych wektorów rozsuniêcia.
	 * @param thisMoveVec - Referencja do wektoru, który pos³u¿y do odsuniêcia tego obiektu.
	 * @param thisMoveVec - Referencja do wektoru, który pos³u¿y do odsuniêcia drugiego obiektu.
	 */
	void GetSeparationVectors(float array[6], glm::vec3& boxMoveVec, glm::vec3& sphereMoveVec);

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

	/** @brief Metoda sprawdzaj¹ca czy sfera znajduje siê miêdzy dwoma równoleg³ymi œcianami pude³ka na danej osi.
	* @param spherePos - wartoœæ X, Y lub z œrodka sfery.
	* @param boxMinSidePos - wartoœæ tej œciany le¿¹cej na danej osi, która ma j¹ mniejsz¹.
	* @param boxMaxSidePos - wartoœæ tej œciany le¿¹cej na danej osi, która ma j¹ wiêksz¹.
	* @return float - wartoœæ X, Y lub Z œrodka sfery, je¿eli le¿y miêdzy dwiema œcianami pude³ka, lub wartoœæ X, Y lub Z œciany bli¿szej tego œrodka.
	*/
	float Clamp(float spherePos, float boxMinSidePos, float boxMaxSidePos);

	/** @brief Ustawia dodatni¹ masê obiektu, do którego jest podczepiony kolider.
	* @param float - masa obiektu.
	*/
	void SetMass(float mass);

	/** @brief Zwraca masê obiektu, do którego jest podczepiony kolider.
	* @return float - masa obiektu.
	*/
	float GetMass();
};
#endif