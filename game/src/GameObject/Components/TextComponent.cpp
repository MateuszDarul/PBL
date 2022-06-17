#include "TextComponent.h"
#include "SceneNode.h"

float TextComponent::s_ScaleFactor = 0.01f * 0.25f;
float TextComponent::s_LineSpacing = 0.05f;

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

    calcCurrentWidth();

    return true;
}

void TextComponent::Clear()
{
	this->font = nullptr;

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

float TextComponent::GetWidth() const
{
    return currentWidth;
}

float TextComponent::GetHeight() const
{
    return currentHeight;
}

void TextComponent::calcCurrentWidth()
{
    currentWidth = 0.0f;
    maxLineHeight = 0.0f;

    int linesCount = 1;
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        if (*c == '\n')
        {
            linesCount += 1;
            continue;
        }
        currentWidth += font->characters[*c].Size.x;
        if (font->characters[*c].Size.y > maxLineHeight) maxLineHeight = font->characters[*c].Size.y;
    }

    currentWidth *= s_ScaleFactor;
    maxLineHeight *= s_ScaleFactor;
    currentHeight = (maxLineHeight + s_LineSpacing) * linesCount - s_LineSpacing;
}

bool TextComponent::Draw(std::shared_ptr<ShaderComponent> shader)
{
	if (!font || !shader) return false;

    if (alwaysSeen)
    {
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_ALWAYS);
    }

    // activate corresponding render state	
    shader->Use();
    if (isGuiElement) shader->SetMat4("transform", GameApplication::GetOrthoProjection());

    shader->SetVec3("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    float scale = s_ScaleFactor;
    float x = (isCentered) ? (-font->characters[text.at(0)].Size.x * scale * text.size() * 0.5f) : 0.0f;
    float y = 0.0f;

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        if (*c == '\n')
        {
            x = 0.0f;
            y -= maxLineHeight + s_LineSpacing;
            continue;
        }

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

    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);


    return true;
}

void TextComponent::SetText(const std::string& text)
{
    this->text = text;
    calcCurrentWidth();
}

std::string TextComponent::GetText()
{
    return text;
}

void TextComponent::FaceCamera(std::shared_ptr<CameraComponent> camera)
{
    auto node = GetOwner();
    auto m = glm::inverse(camera->GetView());
    const auto& t = node->GetComponent<TransformComponent>()->GetPosition();
    m[3][0] = t.x;
    m[3][1] = t.y;
    m[3][2] = t.z;
    node->GetComponent<TransformComponent>()->SetModelMatrix(m);
}