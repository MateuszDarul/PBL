#ifndef __TRANSFORM_COMPONENT_H__
#define __TRANSFORM_COMPONENT_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Component.h"

/** @brief Komponent odpowiedzialny za transformacje GameObjectow.
 */
class TransformComponent :public Component
{
private:
    glm::mat4 modelMatrix; //!< Zapisana macierz transformacji.

    glm::vec3 position; //!< Pozycja obiektu.
    glm::vec3 rotation; //!< Rotacja obiektu.
    glm::vec3 scale; //!< Skala obiektu.

private:
    /** @brief Oblicza nowa macierz modelu dla aktualnych wartosci pozycji, rotacji i skali.
     * @return glm::mat4 - Nowo wyliczona macierz modelu (TransformComponent::modelMatrix jest nadpisywana automatycznie).
     */
    void CalculateModelMatrix();

public:
    /** @brief Domyslny konstruktor.
     */
    TransformComponent();
    
    /** @brief Domyslny destruktor.
     */
    ~TransformComponent();

    /** @brief Zwraca aktualna macierz transformacji.
     * @return glm::mat4 - Macierz modelu.
     */
    const glm::mat4& GetModelMatrix() const;

    /** @brief Ustawia nowa macierz transformacji.
     * @param modelMatrix - Nowa macierz.
     */
    void SetModelMatrix(const glm::mat4& modelMatrix);

    /** @brief Ustala nowa skale obiektu.
     * @param scale - Nowa skala.
     */
    void SetScale(const float& scale);

    /** @brief Ustala nowa skale obiektu.
     * @param scale - Nowa skala.
     */
    void SetScale(const glm::vec3 scale);

    /** @brief Zmienia aktualna skale obiektu o podana wartosc.
     * @param scale - Wartosc, o ktora nalezy zeskalowac obiekt.
     */
    void Scale(const float& scale);
    
    /** @brief Pobiera informacjie o wartosci, o jaka obiekt jest zeskalowany.
     * @return float - Aktualna wartosc skali obiektu.
     */
    float GetScale();

    /** @brief Pobiera informacjie o wartosci, o jaka obiekt jest zeskalowany.
     * @return glm::vec3 - Aktualna wartosc skali obiektu.
     */
    glm::vec3 GetScaleVec();

    /**
     * @brief Ustawia nowa pozycje obiektu.
     * @param x - Nowa pozycja x.
     * @param y - Nowa pozycja y.
     * @param z - Nowa pozycja z.
     */
    void SetPosition(const float& x, const float& y, const float& z);
    
    /**
     * @brief Ustawia nowa pozycje obiektu.
     * @param position - Nowa pozycja.
     */
    void SetPosition(const glm::vec3& position);

    /**
     * @brief Przesuwa obiekt.
     * @param x - Wartosc, o ktora nalezy przesunac obiekt po osi x.
     * @param y - Wartosc, o ktora nalezy przesunac obiekt po osi y.
     * @param z - Wartosc, o ktora nalezy przesunac obiekt po osi z.
     */
    void Move(const float& x, const float& y, const float& z);
    
    /**
     * @brief Przesuwa obiekt.
     * @param vector - Wektor przesuniecia obiektu.
     */
    void Move(const glm::vec3& vector);

    /** @brief Pobiera aktualna pozycje obiektu.
     * @return glm::vec3 - Pozycja obiektu.
     */
    glm::vec3 GetPosition();

    /**
     * @brief Ustawia nowa rotacje obiektu (wartosci w stopniach).
     * @param x - Nowa wartosc obrotu po osi x.
     * @param y - Nowa wartosc obrotu po osi y.
     * @param z - Nowa wartosc obrotu po osi z.
     */
    void SetRotation(const float& x, const float& y, const float& z);

    /**
     * @brief Obraca obiekt (wartosci w stopniach).
     * @param degrees - Nowe wartosci obrotu obiektu, (koleno po osiach X, Y i Z).
     */
    void SetRotation(const glm::vec3& degrees);

    /**
     * @brief Obraca obiekt (wartosci w stopniach).
     * @param x - Warto??ci, o jakie obiekt zostanie obrocony po osi x.
     * @param y - Warto??ci, o jakie obiekt zostanie obrocony po osi y.
     * @param z - Warto??ci, o jakie obiekt zostanie obrocony po osi z.
     */
    void Rotate(const float& x, const float& y, const float& z);

    /**
     * @brief Przesuwa obiekt.
     * @param degrees - Warto??ci, o jakie obiekt zostanie obrocony (koleno po osiach X, Y i Z).
     */
    void Rotate(const glm::vec3& degrees);

    /** @brief Pobiera informacje o obrocie obiektu.
     * @return glm::vec3 - Obrot po osiach (wartosci w stopniach).
     */
    glm::vec3 GetRotation();

    /** @brief Statyczne metody pozwalajace obliczyc macierz modelu bez potrzeby tworzenia instancji klasy (np dla ModelInstancesComponent).
     * @param px - Pozycja x.
     * @param py - Pozycja y.
     * @param pz - Pozycja z.
     * @param rx - Rotacja po osi x.
     * @param ry - Rotacja po osi y.
     * @param rz - Rotacja po osi z.
     * @param scale - Skala.
     * @return glm::mat4 - Macierz transformacji dla podanych wartosci.
     */
    static glm::mat4 Transform(const float& px, const float& py, const float& pz, const float& rx, const float& ry, const float& rz, const float& scale);
    
    /** @brief Statyczne metody pozwalajace obliczyc macierz modelu bez potrzeby tworzenia instancji klasy (np dla ModelInstancesComponent).
     * @param position - Pozycja obiektu.
     * @param rotation - Rotacja obiektu.
     * @param scale - Skala obiektu.
     * @return glm::mat4 - Macierz transformacji dla podanych wartosci.
     */
    static glm::mat4 Transform(const glm::vec3& position, const glm::vec3& rotation, const float& scale);
};

#endif // __TRANSFORM_COMPONENT_H__