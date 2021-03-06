#include "LineComponent.h"
#include "SceneNode.h"

LineComponent::LineComponent()
	: Component(14)
{

}

LineComponent::~LineComponent()
{

}

bool LineComponent::Create()
{
    pointsBuffer.push_back({{ 0.0f,  0.0f, 0.0f }, 0.0f });
    pointsBuffer.push_back({{ 1.0f,  0.0f, 0.0f }, 1.0f });
    

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * MAX_LINE_POINTS, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}

void LineComponent::Clear()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

bool LineComponent::Draw(std::shared_ptr<ShaderComponent> shader)
{
    shader->Use();
    shader->SetMat4("model", glm::mat4(1.0f)); //make line always relative to (0,0,0)
    shader->SetVec3("u_Color1", color1);
    shader->SetVec3("u_Color2", color2);

    glLineWidth(thickness);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(float) * pointsBuffer.size(), &pointsBuffer[0]);

	glDrawArrays(GL_LINE_STRIP, 0, pointsBuffer.size());

    return true;
}

void LineComponent::AddPoint(const glm::vec3& point)
{
    if (pointsBuffer.size() >= MAX_LINE_POINTS) return;

    const float diff = 1.0f / pointsBuffer.size();
    pointsBuffer.push_back({point, 1.0f});

    for (int i = pointsBuffer.size()-2; i >= 0; i--)
    {
        pointsBuffer[i].gradient = pointsBuffer[i+1].gradient - diff;
    }
}

void LineComponent::AddPoint(float x, float y, float z)
{
    AddPoint({x, y, z});
}

void LineComponent::RemoveLast(int count)
{
    pointsBuffer.erase(pointsBuffer.end() - count, pointsBuffer.end());
}

void LineComponent::RemoveAll()
{
    pointsBuffer.clear();
}

const glm::vec3& LineComponent::Get(int i)
{
    if (i < pointsBuffer.size())
    {
        return pointsBuffer[i].pos;
    }
}

void LineComponent::Set(int i, const glm::vec3& point)
{
    if (i < pointsBuffer.size())
    {
        pointsBuffer[i].pos = point;
    }
}

void LineComponent::Set(int i, float x, float y, float z)
{
    Set(i, {x, y, z});
}

int LineComponent::Count()
{
    return pointsBuffer.size();
}

