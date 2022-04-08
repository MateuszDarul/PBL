#ifndef __SCENE_NODE_H__
#define __SCENE_NODE_H__

#include <vector>
#include <memory>

#include <glm/matrix.hpp>

#include "GameObject.h"
#include "Components.h"

/** @brief Wezly, ktore pozwalaja na hierarchizacje GameObject'ow w aplikacji.
 */
class SceneNode
{
private:
    bool needUpdate; //!< Informacja czy trzeba obliczyc globalne transformacje dla wszystkich potomkow tego wezla.
    std::shared_ptr<GameObject> gameObject; //!< GameObject w scenie.
    std::vector<std::shared_ptr<SceneNode>> children; //!< Potomkowie tego wezla.
    glm::mat4 globalTransformations; //!< Globalne transformacje wezla (iloczyn transformacji wezlow wyzszego stopnia i lokalnych transformacji danego GameObject'u).

private:
    /** @brief Pozwala na wyszukanie wybranego wezla z posrod wszystkich potomkow danego wezla. Aby metoda dzialala, potomkowie (ich GameObject) musza posiadac komponent NameComponent.
     * @param name - Nazwa GameObject'u (NameComponent) dla ktorego chcemy uzyskac wskazanie na wezel.
     * @param result - Argument w ktorym zapisany jest wynik poszukiwan.
     */
    void FindByName(const std::string& name, SceneNode** result);

public:
    /** @brief Tworzy nowy wezel.
     * @param gameObject - Wskazanie na GameObject nowego wezla.
     */
    SceneNode(std::shared_ptr<GameObject> gameObject);

    /** @brief domyslny destruktor.
     */
    ~SceneNode();

    /** @brief Dodaje nowy wezel (potomek) do tego wezla.
     * @param gameObject - Wskazanie na GameObject, ktory ma zostac umieszczony w wezle potomnym.
     */
    void AddChild(std::shared_ptr<GameObject> gameObject);
    
    /** @brief Dodaje nowy wezel do tego wezla.
     * @param sceneNode - Wskazanie na SceneNode, ktory ma zostac dodany jako potomek.
     */
    void AddChild(std::shared_ptr<SceneNode> sceneNode);

    /** @brief Zwraca wskazanie na GameObject danego wezla.
     * @return GameObject* - Wskazanie na GameObject.
     */
    std::shared_ptr<GameObject> GetGameObject();

    /** @brief Aktualizuje transformacje potomkow danego wezla.
     * @param parentTransformations - Macierz transformacji rodzica.
     */
    void UpdateTransformations(const glm::mat4& parentTransformations);

    /** @brief Pozwala na zmiane transformacji danego wezla, jak i zapewnia, ze transformacje potomkowow zostana zaktualizowane.  
     * @return TransformComponent* - Wskazanie na komponent lokalnych transformacji GameObject'u danego wezla.
     */
    std::shared_ptr<TransformComponent> GetLocalTransformations();
    
    /** @brief Wyswietla wszystkie GameObject'y, ktore mogą zostac wyswietlone.
     * @param matrixPV - Iloczyn macierzy transformacji perspektywy i kamery.
     */
    void Render(const glm::mat4& matrixPV);

    /** @brief Szuka SceneNode, gdzie GameObject posiada komponent NameComponent o podanej zawartosci.
     * @param name - Nazwa poszukiwanego Noda.
     * @return SceneNode* - Wskazanie na wezel sceny.
     * @return nullptr - Nie znaleziono wezla z GameObject'em o podanej nazwie.
     */
    SceneNode* FindNode(const std::string& name);
};

#endif // __SCENE_NODE_H__