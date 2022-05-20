#pragma once

#include <memory>

#include <glad/glad.h>

#include "Component.h"
#include "ShaderComponent.h"
#include "CameraComponent.h"


#define MAX_LINE_POINTS 100

struct LineVertex
{
    glm::vec3 pos;
    float gradient;
};

/** @brief Komponent reprezentujacy linie.
 */
class LineComponent :public Component
{
public:
    glm::vec3 color1 = {1.0f, 1.0f, 0.0f};  //!< Kolor poczatku lini
    glm::vec3 color2 = {1.0f, 1.0f, 0.0f};  //!< Kolor konca lini
    float thickness = 1.0f;                 //!< Grubosc lini

public:

    /**
     * @brief Konstruktor domyslny
     */
    LineComponent();

    /**
     * @brief Destruktor domyslny
     */
    ~LineComponent();

    /**
     * @brief Inicjalizuje linie i tworzy bufory
     * 
     * @return true Powodzenie
     * @return false Niepowodzenie
     */
    bool Create();

    /**
     * @brief Czysci bufory
     * 
     */
    void Clear();

    /**
     * @brief Rysuje linie na ekranie
     * 
     * @param shader Shader do rysowania lini
     * @return true Powodzenie
     * @return false Niepowodzenie
     */
    bool Draw(std::shared_ptr<ShaderComponent> shader);

    /**
     * @brief Dodaje nowy punkt na koncu lini
     * 
     * @param point punkt
     */
    void AddPoint(const glm::vec3& point);

    /**
     * @brief Dodaje nowy punkt na koncu lini
     * 
     */
    void AddPoint(float x, float y, float z);

    /**
     * @brief Usuwa ostatnie N punktow z lini
     * 
     * @param count liczba punktow do usuniecia (domsylnie - 1)
     */
    void RemoveLast(int count = 1);

    /**
     * @brief Usuwa wszystkie punkty z lini
     * 
     */
    void RemoveAll();

    /**
     * @brief Zwraca punkt o danym indeksie
     * 
     * @param index pierwszy punkt to index 0
     */
    const glm::vec3& Get(int index);

    /**
     * @brief Ustawia dany punkt na danym indeksie
     * 
     */
    void Set(int index, const glm::vec3& point);

    /**
     * @brief Ustawia dany punkt na danym indeksie
     * 
     */
    void Set(int index, float x, float y, float z);

    /**
     * @brief Zwraca liczbe punktow lini
     */
    int Count();

private:
    unsigned int VAO; //!< Vertex Array Object.
    unsigned int VBO; //!< Vertex Buffer Object.

    std::vector<LineVertex> pointsBuffer;
};
