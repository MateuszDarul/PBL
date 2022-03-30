#ifndef __TRANSFORM_COMPONENT_H__
#define __TRANSFORM_COMPONENT_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Component.h"

class TransformComponent :public Component
{
private:
    glm::mat4 modelMatrix; //!< Zapisana macierz transformacji.

    glm::vec3 position; //!< Pozycja obiektu.
    glm::vec3 rotation; //!< Rotacja obiektu.
    float scale; //!< Skala obiektu.

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
    glm::mat4 GetModelMatrix() const;

    /** @brief Ustawia nowa macierz transformacji.
     * @param modelMatrix - Nowa macierz.
     */
    void SetModelMatrix(const glm::mat4& modelMatrix);

    /** @brief Oblicza nowa macierz modelu dla aktualnych wartosci pozycji, rotacji i skali.
     * @return glm::mat4 - Nowo wyliczona macierz modelu (TransformComponent::modelMatrix jest nadpisywana automatycznie).
     */
    glm::mat4 CalculateModelMatrix();

    /** @brief Ustala nowa skale obiektu.
     * @param scale - Nowa skala.
     */
    void SetScale(const float& scale);

    /** @brief Poprawia aktualna skale obiektu o podana wartosc.
     * @param scale - Wartosc, o ktora nalezy zeskalowac obiekt.
     */
    void Scale(const float& scale);

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

    /**
     * @brief Ustawia nowa rotacje obiektu.
     * @param x - Nowa wartosc obrotu po osi x.
     * @param y - Nowa wartosc obrotu po osi y.
     * @param z - Nowa wartosc obrotu po osi z.
     */
    void SetRotation(const float& x, const float& y, const float& z);

    /**
     * @brief Przesuwa obiekt.
     * @param degrees - Nowe wartosci obrotu obiektu, (koleno po osiach X, Y i Z).
     */
    void SetRotation(const glm::vec3& degrees);

    /**
     * @brief Przesuwa obiekt.
     * @param x - Wartości, o jakie obiekt zostanie obrocony po osi x.
     * @param y - Wartości, o jakie obiekt zostanie obrocony po osi y.
     * @param z - Wartości, o jakie obiekt zostanie obrocony po osi z.
     */
    void Rotate(const float& x, const float& y, const float& z);

    /**
     * @brief Przesuwa obiekt.
     * @param degrees - Wartości, o jakie obiekt zostanie obrocony (koleno po osiach X, Y i Z).
     */
    void Rotate(const glm::vec3& degrees);

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