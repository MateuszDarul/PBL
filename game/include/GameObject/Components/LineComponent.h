#pragma once

#include <memory>

#include <glad/glad.h>

#include "Component.h"
#include "ShaderComponent.h"
#include "CameraComponent.h"


#define MAX_LINE_POINTS 100

/** @brief Komponent reprezentujacy model.
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

    glm::vec4& Get(int index);

private:
    unsigned int VAO; //!< Vertex Array Object.
    unsigned int VBO; //!< Vertex Buffer Object.

    std::vector<glm::vec4> pointsBuffer;
    // float pointsBuffer[4*3] = { 1.0f,  2.0f, 3.0f,  0.0f,
    //                             1.0f, -5.0f, 2.5f,  0.5f,
    //                             0.0f, -4.0f, 2.0f,  1.0f }; //4 * x y z gradient
};
