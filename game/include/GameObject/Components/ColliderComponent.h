#ifndef __COLLIDER_COMPONENT_H__
#define __COLLIDER_COMPONENT_H__
#include "Component.h"
#include "CollisionStructs.h"

#include <glm/glm.hpp>


class CollidersManager;


/** @brief Abstrakcyjny komponent kolider�w.
 */
class ColliderComponent : public Component
{
protected:
	glm::vec3 offset; //!< Przesuni�cie kolidera wzgl�dem bazowego obiektu.
	float mass; //!< Masa obiektu do kt�rego podczepiony jest kolider.


	/** @brief Zwraca indeks najmniejszej warto�ci z tablicy z d�ugo�ciami wektor�w rozsuni�cia.
	 * @param array - Tablica z 6 d�ugo�ciami potencjalnych wektor�w rozsuni�cia.
	 * @return int - indeks najmniejszej warto�ci z tablicy.
	 */
	int GetLowestValueIndex(float array[6]);

	/** @brief Ustawia wektory rozsuni�cia na podstawie tablicy z d�ugo�ciami potencjalnych wektor�w rozsuni�cia, gdy przebiega ono wzd�u� osi X, Y lub Z.
	 * @param array - Tablica z 6 d�ugo�ciami potencjalnych wektor�w rozsuni�cia.
	 * @param thisMoveVec - Referencja do wektoru, kt�ry pos�u�y do odsuni�cia tego obiektu.
	 * @param thisMoveVec - Referencja do wektoru, kt�ry pos�u�y do odsuni�cia drugiego obiektu.
	 */
	void GetSeparationVectors(float array[6], glm::vec3& boxMoveVec, glm::vec3& sphereMoveVec);

public:
	bool isStatic; //!< Czy ten kolider ma by� statyczny.
	bool isTrigger; //!< Czy ten kolider ma by� wyzwalaczem.
	bool isOptimized; //!< Czy ten kolider ma by� optymalizowany pod wzgl�dem odleg�o�ci od gracza.
	int layer;

	/** @brief Konstruktor komponent kolider�w.
	 * @param UUID - Unikalny indentyfikator klasy komponentu.
	 * @param isTrigger - Czy ten kolider ma by� wyzwalaczem.
	 * @param isStatic -Czy ten kolider ma by� statyczny.
	 */
	ColliderComponent(uint32_t UUID, bool isTrigger, bool isStatic, int layer);
	
	/** @brief Bazowy destruktor
	*/
	~ColliderComponent();

	/** @brief Metoda dodaje kolider do menad�era kolider�w.
	* @param collidersManager - wska�nik na menad�er kolider�w.
	*/
	void AddToCollidersManager(CollidersManager* collidersManager);

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
	bool virtual CheckCollision(std::shared_ptr<ColliderComponent> collider) = 0;

	virtual bool RayCollision(const glm::vec3& origin, const glm::vec3& dir, RayHitInfo& hitInfo, float maxDistance) = 0;

	/** @brief Metoda sprawdzaj�ca czy sfera znajduje si� mi�dzy dwoma r�wnoleg�ymi �cianami pude�ka na danej osi.
	* @param spherePos - warto�� X, Y lub z �rodka sfery.
	* @param boxMinSidePos - warto�� tej �ciany le��cej na danej osi, kt�ra ma j� mniejsz�.
	* @param boxMaxSidePos - warto�� tej �ciany le��cej na danej osi, kt�ra ma j� wi�ksz�.
	* @return float - warto�� X, Y lub Z �rodka sfery, je�eli le�y mi�dzy dwiema �cianami pude�ka, lub warto�� X, Y lub Z �ciany bli�szej tego �rodka.
	*/
	float Clamp(float spherePos, float boxMinSidePos, float boxMaxSidePos);

	/** @brief Ustawia dodatni� mas� obiektu, do kt�rego jest podczepiony kolider.
	* @param float - masa obiektu.
	*/
	void SetMass(float mass);

	/** @brief Zwraca mas� obiektu, do kt�rego jest podczepiony kolider.
	* @return float - masa obiektu.
	*/
	float GetMass();
};
#endif