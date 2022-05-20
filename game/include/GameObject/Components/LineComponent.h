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
    glm::vec3 color1 = {1.0f, 1.0f, 0.0f};
    glm::vec3 color2 = {1.0f, 1.0f, 0.0f};
    float thickness = 1.0f;

public:

    LineComponent();

    ~LineComponent();

    bool Create();
    void Clear();

    bool Draw(std::shared_ptr<ShaderComponent> shader);

    void AddPoint(const glm::vec3& point);
    void AddPoint(float x, float y, float z);
    void RemoveLast(int count = 1);
    void RemoveAll();

    const glm::vec3& Get(int index);
    void Set(int index, float x, float y, float z);
    void Set(int index, const glm::vec3& point);

    int Count();

private:
    unsigned int VAO; //!< Vertex Array Object.
    unsigned int VBO; //!< Vertex Buffer Object.

    std::vector<LineVertex> pointsBuffer;
};
