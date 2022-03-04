#include "Shader.h"

Shader::Shader(std::string vertex_shader_path, std::string fragment_shader_path)
{
    std::string vertex_shader_code;
    std::string fragment_shader_code;

    std::string tmp;

    std::fstream vertex_shader_file(vertex_shader_path, std::ios::in);
    if(!vertex_shader_file.good())
    {
        std::cerr << "ERROR::SHADER::VERTEX::FILE_NOT_FOUND" << std::endl;
        exit(1);
    }
    while(!vertex_shader_file.eof())
    {
        std::getline(vertex_shader_file, tmp);
        vertex_shader_code += tmp + '\n';
    }
    vertex_shader_code.pop_back();
    vertex_shader_file.close();

    std::fstream fragment_shader_file(fragment_shader_path, std::ios::in);
    if(!fragment_shader_file.good())
    {
        std::cerr << "ERROR::SHADER::VERTEX::FILE_NOT_FOUND" << std::endl;
        exit(2);
    }
    while(!fragment_shader_file.eof())
    {
        std::getline(fragment_shader_file, tmp);
        fragment_shader_code += tmp + '\n';
    }
    fragment_shader_code.pop_back();
    fragment_shader_file.close();

    tmp.clear();

    const char* vertex_shader_code_cstr = vertex_shader_code.c_str();
    const char* fragment_shader_code_cstr = fragment_shader_code.c_str();

    int success;
    char info_log[512];

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_code_cstr, NULL);
    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << std::endl;
        std::cerr << info_log << std::endl;
        exit(3);
    }

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_code_cstr, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << std::endl;
        std::cerr << info_log << std::endl;
        exit(4);
    }

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << std::endl;
        std::cerr << info_log << std::endl;
        exit(5);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

Shader::~Shader()
{
    glDeleteProgram(shader_program);
}

void Shader::use()
{
    glUseProgram(shader_program);
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(shader_program, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(shader_program, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(shader_program, name.c_str()), value);
}

void Shader::set4Float(const std::string& name, float value_1, float value_2, float value_3, float value_4) const
{
    glUniform4f(glGetUniformLocation(shader_program, name.c_str()), value_1, value_2, value_3, value_4);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(shader_program, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(shader_program, name.c_str()), x, y);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(shader_program, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(shader_program, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(shader_program, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(shader_program, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(shader_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(shader_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(shader_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

unsigned int Shader::getID()
{
    return shader_program;
}
