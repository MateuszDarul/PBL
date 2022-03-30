#include "ShaderComponent.h"

ShaderComponent::ShaderComponent()
    :Component(5)
{

}

ShaderComponent::~ShaderComponent()
{
    glDeleteProgram(this->shaderProgram);
}

bool ShaderComponent::Create(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    std::string vertexShaderCode;
    std::string fragmentShaderCode;

    std::string tmp;

    std::fstream vertexShaderFile(vertexShaderPath, std::ios::in);
    if(!vertexShaderFile.good())
    {
        std::cerr << "ERROR::SHADER::VERTEX::FILE_NOT_FOUND" << std::endl;
        return false;
    }
    while(!vertexShaderFile.eof())
    {
        std::getline(vertexShaderFile, tmp);
        vertexShaderCode += tmp + '\n';
    }
    vertexShaderCode.pop_back();
    vertexShaderFile.close();

    std::fstream fragmentShaderFile(fragmentShaderPath, std::ios::in);
    if(!fragmentShaderFile.good())
    {
        std::cerr << "ERROR::SHADER::VERTEX::FILE_NOT_FOUND" << std::endl;
        return false;
    }
    while(!fragmentShaderFile.eof())
    {
        std::getline(fragmentShaderFile, tmp);
        fragmentShaderCode += tmp + '\n';
    }
    fragmentShaderCode.pop_back();
    fragmentShaderFile.close();

    tmp.clear();

    const char* vertexShaderCodeCSTR = vertexShaderCode.c_str();
    const char* fragmentShaderCodeCSTR = fragmentShaderCode.c_str();

    int success;
    char info_log[512];

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCodeCSTR, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, info_log);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << std::endl;
        std::cerr << info_log << std::endl;
        return false;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderCodeCSTR, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, info_log);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << std::endl;
        std::cerr << info_log << std::endl;
        return false;
    }

    this->shaderProgram = glCreateProgram();
    glAttachShader(this->shaderProgram, vertexShader);
    glAttachShader(this->shaderProgram, fragmentShader);
    glLinkProgram(this->shaderProgram);
    glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(this->shaderProgram, 512, NULL, info_log);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << std::endl;
        std::cerr << info_log << std::endl;
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

void ShaderComponent::Use()
{
    glUseProgram(this->shaderProgram);
}

void ShaderComponent::SetBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(this->shaderProgram, name.c_str()), (int)value);
}

void ShaderComponent::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(this->shaderProgram, name.c_str()), value);
}

void ShaderComponent::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(this->shaderProgram, name.c_str()), value);
}

void ShaderComponent::Set4Float(const std::string& name, float value_1, float value_2, float value_3, float value_4) const
{
    glUniform4f(glGetUniformLocation(this->shaderProgram, name.c_str()), value_1, value_2, value_3, value_4);
}

void ShaderComponent::SetVec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(this->shaderProgram, name.c_str()), 1, &value[0]);
}

void ShaderComponent::SetVec2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(this->shaderProgram, name.c_str()), x, y);
}

void ShaderComponent::SetVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(this->shaderProgram, name.c_str()), 1, &value[0]);
}

void ShaderComponent::SetVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(this->shaderProgram, name.c_str()), x, y, z);
}

void ShaderComponent::SetVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(this->shaderProgram, name.c_str()), 1, &value[0]);
}

void ShaderComponent::SetVec4(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(this->shaderProgram, name.c_str()), x, y, z, w);
}

void ShaderComponent::SetMat2(const std::string& name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(this->shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderComponent::SetMat3(const std::string& name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(this->shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderComponent::SetMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(this->shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

unsigned int ShaderComponent::GetID()
{
    return this->shaderProgram;
}
