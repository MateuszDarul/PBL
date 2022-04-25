#include "TextComponent.h"
#include "SceneNode.h"

TextComponent::TextComponent()
	: Component(13)
{

}

TextComponent::~TextComponent()
{

}

bool TextComponent::Create(const std::string& text, Font* font)
{
	if (!font) return false;

	this->font = font;
    this->text = text;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}

void TextComponent::Clear()
{
	this->font = nullptr;

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

bool TextComponent::Draw(std::shared_ptr<ShaderComponent> shader)
{
	if (!font || !shader) return false;

    // activate corresponding render state	
    shader->Use();
    shader->SetVec3("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    float x = 0.0f;
    float y = 0.0f;
    float scale = 0.01f * 0.25f;

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = font->characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void TextComponent::FaceCamera(std::shared_ptr<CameraComponent> camera, SceneNode* node)
{
    auto m = glm::inverse(camera->GetView());
    const auto& t = node->GetLocalTransformations()->GetPosition();
    m[3][0] = t.x;
    m[3][1] = t.y;
    m[3][2] = t.z;
    node->GetLocalTransformations()->SetModelMatrix(m);
}