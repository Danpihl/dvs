#include "shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

Shader::Shader(const std::string& vertex_shader, const std::string& fragment_shader, const ShaderSource src)
{
    if (ShaderSource::FILE == src)
    {
        loadShaderFromFiles(vertex_shader, fragment_shader);
    }
    else if (ShaderSource::CODE == src)
    {
        loadShadersFromSourceCode(vertex_shader, fragment_shader);
    }
    else
    {
        throw std::runtime_error("Invalid option for shader source!");
    }
    std::cout << "PGM id: " << program_id_ << std::endl;
}

Shader Shader::createFromFiles(const std::string& vertex_shader_path, const std::string& fragment_shader_path)
{
    return Shader(vertex_shader_path, fragment_shader_path, ShaderSource::FILE);
}

Shader Shader::createFromCode(const char* const vertex_code, const char* const fragment_code)
{
    return Shader(vertex_code, fragment_code, ShaderSource::CODE);
}

void Shader::loadShaderFromFiles(const std::string& vertex_path, const std::string& fragment_path)
{
    std::string vertex_code, fragment_code;
    // Read the Vertex Shader code from the file
    std::ifstream VertexShaderStream(vertex_path, std::ios::in);
    if (VertexShaderStream.is_open())
    {
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        vertex_code = sstr.str();
        VertexShaderStream.close();
    }
    else
    {
        throw std::runtime_error("Not able to open " + vertex_path);
    }

    // Read the Fragment Shader code from the file
    std::ifstream FragmentShaderStream(fragment_path, std::ios::in);
    if (FragmentShaderStream.is_open())
    {
        std::stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        fragment_code = sstr.str();
        FragmentShaderStream.close();
    }
    else
    {
        throw std::runtime_error("Not able to open " + fragment_path);
    }

    loadShadersFromSourceCode(vertex_code, fragment_code);
}

void Shader::loadShadersFromSourceCode(const std::string& vertex_code, const std::string& fragment_code)
{
    GLint Result = GL_FALSE;
    int InfoLogLength;

    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Compile Vertex Shader
    char const* VertexSourcePointer = vertex_code.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);

        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    // Compile Fragment Shader
    char const* FragmentSourcePointer = fragment_code.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

    // Link the program
    printf("Linking program\n");
    program_id_ = glCreateProgram();
    std::cout << "Program id: " << program_id_ << std::endl;

    glAttachShader(program_id_, VertexShaderID);
    glAttachShader(program_id_, FragmentShaderID);
    glLinkProgram(program_id_);

    // Check the program
    glGetProgramiv(program_id_, GL_LINK_STATUS, &Result);
    glGetProgramiv(program_id_, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(program_id_, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDetachShader(program_id_, VertexShaderID);
    glDetachShader(program_id_, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
}

GLuint Shader::programId() const
{
    return program_id_;
}
