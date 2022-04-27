#ifndef __COLLIDERS_MANAGER_H__
#define __COLLIDERS_MANAGER_H__
#include "ColliderComponent.h"
#include <vector>

/** @brief Menadøer kolizji.
 */
class CollidersManager
{
private:
	std::vector<std::weak_ptr<ColliderComponent>> staticColliders; //!< Lista statycznych zderzaczy.
	std::vector<std::weak_ptr<ColliderComponent>> staticTriggers; //!< Lista statycznych wyzwalaczy.
	std::vector<std::weak_ptr<ColliderComponent>> dynamicColliders; //!< Lista statycznych zderzaczy.
	std::vector<std::weak_ptr<ColliderComponent>> dynamicTriggers; //!< Lista statycznych wyzwalaczy.

	/** @brief Usuwa komponent koliderÛw.
	 * @param vector - Z ktÛrej listy ma usunπÊ kolider.
	 * @param col - Wskaünik kolideru do usuniÍcia.
	 */
	void RemoveFromVector(std::vector<std::weak_ptr<ColliderComponent>> vector, std::shared_ptr<ColliderComponent> col);
public:

	/** @brief Bazowy konstruktor.
	 */
	CollidersManager();

	/** @brief Bazowy destruktor.
	 */
	~CollidersManager();

	/** @brief Dodaje wskaünik statycznego zderzacza do listy.
	 * @param col - Wskaünik na statyczny zderzacz do dodania.
	 */
	void AddStaticCollider(std::shared_ptr<ColliderComponent> collider);

	/** @brief Dodaje wskaünik dynamicznego zderzacza do listy.
	 * @param col - Wskaünik na dynamiczny zderzacz do dodania.
	 */
	void AddDynamicCollider(std::shared_ptr<ColliderComponent> collider);

	/** @brief Dodaje wskaünik statycznego wyzwalacza do listy.
	 * @param col - Wskaünik na statyczny wyzwalacz do dodania.
	 */
	void AddStaticTrigger(std::shared_ptr<ColliderComponent> trigger);

	/** @brief Dodaje wskaünik dynamicznego wyzwalacza do listy.
	 * @param col - Wskaünik na dynamiczny wyzwalacz do dodania.
	 */
	void AddDynamicTrigger(std::shared_ptr<ColliderComponent> trigger);

	/** @brief Usuwa wskaünik dynamicznego zderzacza z listy.
	 * @param col - Wskaünik na dynamiczny zderzacz do usuniÍcia.
	 */
	void RemoveDynamicColllider(std::shared_ptr<ColliderComponent> collider);

	/** @brief Usuwa wskaünik statycznego zderzacza z listy.
	 * @param col - Wskaünik na statyczny zderzacz do usuniÍcia.
	 */
	void RemoveStaticColllider(std::shared_ptr<ColliderComponent> collider);

	/** @brief Usuwa wskaünik dynamicznego wyzywalacza z listy.
	 * @param col - Wskaünik na dynamiczny wyzwalacz do usuniÍcia.
	 */
	void RemoveDynamicTrigger(std::shared_ptr<ColliderComponent> trigger);

	/** @brief Usuwa wskaünik dynamicznego wyzwalacza z listy.
	 * @param col - Wskaünik na statyczny wyzwalacz do usuniÍcia.
	 */
	void RemoveStaticTrigger(std::shared_ptr<ColliderComponent> trigger);
	
	/** @brief Sprawdza kolizje kolejno: dynamicze zderzacze z dynamicznymi zderzaczami, dynamicze zderzacze ze statycznymi zderzaczami, dynamicze zderzacze z dynamicznymi wyzwalaczami, dynamicze zderzacze ze statycznymi wyzwalaczami.
	 */
	void CheckCollisions();

	/** @brief Sprawdza kolizje kolejno: dynamicze wyzwalacze z dynamicznymi wyzwalaczami, dynamicze wyzwalacze ze statycznymi zderzaczami, dynamicze zderzacze ze statycznymi wyzwalaczami.
	 */
	void CheckTriggers();
};
#endif