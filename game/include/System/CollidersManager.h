#ifndef __COLLIDERS_MANAGER_H__
#define __COLLIDERS_MANAGER_H__
#include "ColliderComponent.h"
#include <vector>

struct RayHitInfo
{
	glm::vec3 point;
	glm::vec3 normal;
	float distance;
	GameObject* gameObject;
};

/** @brief Menad�er kolizji.
 */
class CollidersManager
{
private:
	std::vector<std::weak_ptr<ColliderComponent>> staticColliders; //!< Lista statycznych zderzaczy.
	std::vector<std::weak_ptr<ColliderComponent>> staticTriggers; //!< Lista statycznych wyzwalaczy.
	std::vector<std::weak_ptr<ColliderComponent>> dynamicColliders; //!< Lista statycznych zderzaczy.
	std::vector<std::weak_ptr<ColliderComponent>> dynamicTriggers; //!< Lista statycznych wyzwalaczy.

	/** @brief Usuwa komponent kolider�w.
	 * @param vector - Z kt�rej listy ma usun�� kolider.
	 * @param col - Wska�nik kolideru do usuni�cia.
	 */
	void RemoveFromVector(std::vector<std::weak_ptr<ColliderComponent>> vector, std::shared_ptr<ColliderComponent> col);
public:

	/** @brief Bazowy konstruktor.
	 */
	CollidersManager();

	/** @brief Bazowy destruktor.
	 */
	~CollidersManager();

	/** @brief Dodaje wska�nik statycznego zderzacza do listy.
	 * @param col - Wska�nik na statyczny zderzacz do dodania.
	 */
	void AddStaticCollider(std::shared_ptr<ColliderComponent> collider);

	/** @brief Dodaje wska�nik dynamicznego zderzacza do listy.
	 * @param col - Wska�nik na dynamiczny zderzacz do dodania.
	 */
	void AddDynamicCollider(std::shared_ptr<ColliderComponent> collider);

	/** @brief Dodaje wska�nik statycznego wyzwalacza do listy.
	 * @param col - Wska�nik na statyczny wyzwalacz do dodania.
	 */
	void AddStaticTrigger(std::shared_ptr<ColliderComponent> trigger);

	/** @brief Dodaje wska�nik dynamicznego wyzwalacza do listy.
	 * @param col - Wska�nik na dynamiczny wyzwalacz do dodania.
	 */
	void AddDynamicTrigger(std::shared_ptr<ColliderComponent> trigger);

	/** @brief Usuwa wska�nik dynamicznego zderzacza z listy.
	 * @param col - Wska�nik na dynamiczny zderzacz do usuni�cia.
	 */
	void RemoveDynamicColllider(std::shared_ptr<ColliderComponent> collider);

	/** @brief Usuwa wska�nik statycznego zderzacza z listy.
	 * @param col - Wska�nik na statyczny zderzacz do usuni�cia.
	 */
	void RemoveStaticColllider(std::shared_ptr<ColliderComponent> collider);

	/** @brief Usuwa wska�nik dynamicznego wyzywalacza z listy.
	 * @param col - Wska�nik na dynamiczny wyzwalacz do usuni�cia.
	 */
	void RemoveDynamicTrigger(std::shared_ptr<ColliderComponent> trigger);

	/** @brief Usuwa wska�nik dynamicznego wyzwalacza z listy.
	 * @param col - Wska�nik na statyczny wyzwalacz do usuni�cia.
	 */
	void RemoveStaticTrigger(std::shared_ptr<ColliderComponent> trigger);
	
	/** @brief Sprawdza kolizje kolejno: dynamicze zderzacze z dynamicznymi zderzaczami, dynamicze zderzacze ze statycznymi zderzaczami, dynamicze zderzacze z dynamicznymi wyzwalaczami, dynamicze zderzacze ze statycznymi wyzwalaczami.
	 */
	void CheckCollisions();

	/** @brief Sprawdza kolizje kolejno: dynamicze wyzwalacze z dynamicznymi wyzwalaczami, dynamicze wyzwalacze ze statycznymi zderzaczami, dynamicze zderzacze ze statycznymi wyzwalaczami.
	 */
	void CheckTriggers();


	bool Raycast(const glm::vec3& origin, const glm::vec3 dir, RayHitInfo& hitInfo, float maxDistance = 100000.0f, bool shouldHitTriggers = false /*, layer*/ );
};
#endif