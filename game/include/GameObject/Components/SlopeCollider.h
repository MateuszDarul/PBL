#ifndef __SLOPE_COLLIDER_H__
#define __SLOPE_COLLIDER_H__
#include "ColliderComponent.h"

#include <glm/glm.hpp>


class SlopeCollider : public ColliderComponent
{
public:

	enum class Direction {
		Z, Z_NEG, X, X_NEG
	};


	/** @brief Konstruktor komponentu kolider�w.
	 */
	SlopeCollider();

	/** @brief Konstruktor komponent kolider�w.
	 * @param isTrigger - Czy ten kolider ma by� wyzwalaczem.
	 * @param isStatic -Czy ten kolider ma by� statyczny.
	 * @param layer - warstwa kolidera.
	 */
	SlopeCollider(bool isTrigger, bool isStatic, int layer = CollisionLayer::Default);

	/** @brief Bazowy destruktor.
	 */
	~SlopeCollider();

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

	
	void SetDirection(Direction dir);

    void SetDimensions(float x, float y, float z);
    void SetDimensions(const glm::vec3& dimensions);
	const glm::vec3& GetDimensions() const;

	const glm::vec3& GetNormal() const;
	const glm::vec3& GetAlongX() const;
	const glm::vec3& GetAlongZ() const;

	float GetLengthX() const;
	float GetLengthZ() const;


private:
    glm::vec3 dimensions;
	glm::vec3 normal, alongX, alongZ;
	float lenX, lenZ;
	Direction direction;

	void CalculateNormal();
};
#endif