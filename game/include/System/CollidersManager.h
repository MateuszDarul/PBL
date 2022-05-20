#ifndef __COLLIDERS_MANAGER_H__
#define __COLLIDERS_MANAGER_H__
#include "ColliderComponent.h"
#include "Frustum.h"
#include <vector>
#include <utility>



enum CollidingType
{
	collision, trigger
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

	std::vector<std::pair<std::shared_ptr<ColliderComponent>,std::shared_ptr<ColliderComponent>>> recentCollisions;

	float distanceFromPlayer; //!< Dystans od gracza, do kt�rej optymalizowane kolidery s� sprawdzane.
	std::shared_ptr<GameObject> player;

	/** @brief Usuwa komponent kolider�w.
	 * @param vector - Z kt�rej listy ma usun�� kolider.
	 * @param col - Wska�nik kolideru do usuni�cia.
	 */
	void RemoveFromVector(std::vector<std::weak_ptr<ColliderComponent>> vector, std::shared_ptr<ColliderComponent> col);
	
	/** @brief Sprawdza czy kolider jest optymalizowany i czy powinien być sprawdzony.
	 * @param collider - potencjalny kolider do sprawdzenia optymalizacji.
	 * @param Frustum - Płaszczyzny widzenia kamery.
	 * @param playerPos - pozycja gracza.
	 * @return true - kolider jest do sprawdzenia.
	 * @return false - kolideru nie trzeba sprawdzać.
	 */
	bool CheckOptimalization(std::shared_ptr<ColliderComponent> collider, Frustum frustum, glm::vec3 playerPos);

	/** @brief Sprawdza czy kolider jest optymalizowany i czy powinien być sprawdzony.
	 * @param currentCollisions -  Lista kolizji które zaszły w tej iteracji pętli gry.
	 * @param firstCollision - Pierwszy sprawdzany kolider.
	 * @param secondCollision - Drugi sprawdzany kolider.
	 * @param areColliding - Czy oba kolidery się kolidują.
	 * @param type - typ kolizji na którego podstawie mają się wykonać odpowiednie skrypty (zderzacz lub wyzwalacz).
	 */
	void ExecuteOnCollidingScripts(std::vector<std::pair<std::shared_ptr<ColliderComponent>, std::shared_ptr<ColliderComponent>>> currentCollisions,
		std::shared_ptr<ColliderComponent> firstCollider, std::shared_ptr<ColliderComponent> secondCollider, bool areColliding, CollidingType type);

public:

	/** @brief Bazowy konstruktor.
	 */
	CollidersManager(std::shared_ptr<GameObject> player);

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
	 * @param currentCollisions - Lista kolizji które zaszły w tej iteracji pętli gry.
	 */
	void CheckCollisions(std::vector<std::pair<std::shared_ptr<ColliderComponent>, std::shared_ptr<ColliderComponent>>> currentCollisions);

	/** @brief Sprawdza kolizje kolejno: dynamicze wyzwalacze z dynamicznymi wyzwalaczami, dynamicze wyzwalacze ze statycznymi zderzaczami, dynamicze zderzacze ze statycznymi wyzwalaczami.
	 * @param currentCollisions - Lista kolizji które zaszły w tej iteracji pętli gry.
	 */
	void CheckTriggers(std::vector<std::pair<std::shared_ptr<ColliderComponent>, std::shared_ptr<ColliderComponent>>> currentCollisions);

	/** @brief Sprawdza kolizje zderzaczy i wyzwalaczy.
	 */
	void CheckEverything();

	/**
	 * @brief Metoda sprawdzajaca kolizje z promieniem.
	 * 
	 * @param origin Poczatek promienia.
	 * @param dir Kierunek promienia.
	 * @param hitInfo Dane o punkcie kolizji.
	 * @param maxDistance Maksymalna oldeglosc promienia.
	 * @param shouldHitTriggers Czy powinien sprawdzac wyzwalacze.
	 * @param layerMask Maska kolizji.
	 * @return true Zachodzi kolizja.
	 * @return false Nie zachodzi kolizja.
	 */
	bool Raycast(const glm::vec3& origin, const glm::vec3 dir, RayHitInfo& hitInfo, float maxDistance = 100000.0f, bool shouldHitTriggers = false, int layerMask = 0xffffffff);

	/** @brief Ustawia dystans od gracza, do kt�rego dla optymalizowanych kolider�w sprawdzana jest kolizja.
	 * @param distance - Nowy dystans od gracza do optymalizacji kolizji.
	 */
	void SetDistanceFromPlayer(float distance);

	/** @brief Zwraca dystans od gracza, do kt�rego dla optymalizowanych kolider�w sprawdzana jest kolizja.
	 * @return float - Dystans od gracza do optymalizacji kolizji.
	 */
	float GetDistanceFromPlayer();
};
#endif