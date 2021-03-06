#ifndef __SCENE_NODE_H__
#define __SCENE_NODE_H__

#include <vector>
#include <memory>

#include <glm/matrix.hpp>

#include "GameObject.h"
#include "Components.h"
#include "GameApplication.h"

/** @brief Wezly, ktore pozwalaja na hierarchizacje GameObject'ow w aplikacji.
 */
class SceneNode : public std::enable_shared_from_this<SceneNode>
{
private:

    SceneNode* parent; //!< Wskazanie na wezel nadrzedny.
    std::shared_ptr<GameObject> gameObject; //!< GameObject w scenie.
    std::vector<std::shared_ptr<SceneNode>> children; //!< Potomkowie tego wezla.
    glm::mat4 globalTransformations; //!< Globalne transformacje wezla (iloczyn transformacji wezlow wyzszego stopnia i lokalnych transformacji danego GameObject'u).

private:
    /** @brief Pozwala na wyszukanie wybranego wezla z posrod wszystkich potomkow danego wezla. Aby metoda dzialala, potomkowie (ich GameObject) musza posiadac komponent NameComponent.
     * @param name - Nazwa GameObject'u (NameComponent) dla ktorego chcemy uzyskac wskazanie na wezel.
     * @param result - Argument w ktorym zapisany jest wynik poszukiwan.
     */
    void FindByName(const std::string& name, SceneNode** result);

    /** @brief Ustawia rodzica danego wezla.
     * @param parent 
     */
    void SetParent(SceneNode* parent);

    /** @brief Aktualizuje transformacje danego wezla i wszystkich jego podwezlow.
     */
    void PrivateUpdate(float dt, const glm::mat4& parentTransformations);

    struct NodeToDelete
    {
        SceneNode* parent;
        SceneNode* child;
    };
    static std::vector<NodeToDelete> nodesToDelete;
    void PrivateDelete();
public:
    void DeleteNodes();
    static Frustum cameraFrustum; //!< Ostroslup widzenia kamery.
    /** @brief Tworzy nowy wezel.
     * @param gameObject - Wskazanie na GameObject nowego wezla.
     */
    SceneNode(std::shared_ptr<GameObject> gameObject);

    /** @brief domyslny destruktor.
     */
    ~SceneNode();

    /** @brief Tworzy nowy wezel na podstawie otrzymanego GameObject'u i dodaje go do listy potomkow.
     * @param gameObject - Wskazanie na GameObject, ktory ma zostac umieszczony w wezle potomnym.
     */
    std::shared_ptr<SceneNode> AddChild(std::shared_ptr<GameObject> gameObject);
    
    /** @brief Dodaje nowy wezel (potomek) do tego wezla.
     * @param sceneNode - Wskazanie na SceneNode, ktory ma zostac dodany jako potomek.
     */
    std::shared_ptr<SceneNode> AddChild(std::shared_ptr<SceneNode> sceneNode);

    /** @brief Zwraca wskazanie na GameObject danego wezla.
     * @return GameObject* - Wskazanie na GameObject.
     */
    std::shared_ptr<GameObject> GetGameObject();

    /** @brief Pozwala na zmiane transformacji danego wezla, jak i zapewnia, ze transformacje potomkowow zostana zaktualizowane.  
     * @return TransformComponent* - Wskazanie na komponent lokalnych transformacji GameObject'u danego wezla.
     */
    std::shared_ptr<TransformComponent> GetLocalTransformations();

    /** @brief Pobiera macierz globalnych transformacji danego wezla.
     * @return const glm::mat4& - Macierz globalnych transformacji.
     */
    const glm::mat4& GetGlobalTransformations();

    glm::vec3 GetGlobalPosition();

    /**
     * @brief Ustawia globalna macierz na zadana macierz przeksztalcen.
     */
    void SetGlobalTransformations(const glm::mat4& newTransformations);

    /**
     * @brief Ustawia globalna macierz na iloczyn lokalnej macierzy z macierza jednostkowa.
     */
    void ResetGlobalTransformations();

    /** @brief Wyswietla wszystkie GameObject'y, ktore mog?? zostac wyswietlone.
     * @param matrixPV - Iloczyn macierzy transformacji perspektywy i kamery.
     */
    void Render(const glm::mat4& matrixPV);

    /** @brief Wykonuje metode OnStart skryptow wszystkich GameObject'ow na scenie.
     */
    void LoadScripts();

    /** @brief Aktualizuje transformacje danego wezla i wszystkich jego podwezlow.
     */
    void Update(float dt);

    /** @brief Szuka SceneNode, gdzie GameObject posiada komponent NameComponent o podanej zawartosci.
     * @param name - Nazwa poszukiwanego Noda.
     * @return SceneNode* - Wskazanie na wezel sceny.
     * @return nullptr - Nie znaleziono wezla z GameObject'em o podanej nazwie.
     */
    SceneNode* FindNode(const std::string& name);

    /** @brief Usuwa wskazany wezel ze swoich dzieci.
     * @param node - Wskazanie na wezel do usuniecia.
     * @return true - Usuni??to wezel.
     * @return false - Nie usuni??to wezla.
     */
    bool RemoveNode(std::shared_ptr<SceneNode> node);

    /** @brief Usuwa wskazany wezel ze swoich dzieci.
     * @param node - Wskazanie na wezel do usuniecia.
     * @return true - Usuni??to wezel.
     * @return false - Nie usuni??to wezla.
     */
    bool RemoveNode(SceneNode* node);

    /** @brief Zwraca wskazanie na rodzica danego wezla.
     * @return SceneNode* - rodzic.
     */
    SceneNode* GetParent();

    /** @brief Zwraca wskazanie na wezel glowny danego drzewa.
     * @return SceneNode* - Wezel glowny.
     */
    SceneNode* GetRoot();

    void RenderDepthMap(std::shared_ptr<ShaderComponent> shader);

    /** @brief Porownuje SceneNode'y.
     * @param second - Wezel z ktorym chcemy porownac.
     * @return true - To jest ten sam wezel.
     * @return false - Sa to rozne wezly.
     */
    bool Is(SceneNode* second);

    /** @brief Porownuje SceneNode'y.
     * @param second - Wezel z ktorym chcemy porownac.
     * @return true - To jest ten sam wezel.
     * @return false - Sa to rozne wezly.
     */
    bool Is(std::shared_ptr<SceneNode> second);
};

#endif // __SCENE_NODE_H__