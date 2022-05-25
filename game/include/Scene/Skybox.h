#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Components.h"
class Skybox
{
private:
	std::shared_ptr<ShaderComponent> shader;
	unsigned int VBO;
	unsigned int VAO;
	unsigned int texture;

public:
	Skybox();
	~Skybox();
	unsigned int GetTexture();
	void Create(std::string faces[], std::shared_ptr<ShaderComponent> shader);
	void Render(glm::mat4 transform);
};

#endif