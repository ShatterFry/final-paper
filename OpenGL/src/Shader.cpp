#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "OpenGLSettings.h"

Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_RendererID(0)
{
    std::cout << "Shader::Shader" << std::endl;

	ShaderProgramSource source = ParseShader(filepath);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    FFunctionCallback funcCB = [&]()
    {
        glDeleteProgram(m_RendererID);
    };
	GLCall(funcCB);
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::cout << "Shader::ParseShader" << std::endl;

	std::ifstream stream(filepath);

    if(!stream.is_open())
    {
        std::cout << "Failed to open shader file " << filepath << std::endl;
    }
    else
    {
        std::cout << "Successfully opened shader file " << filepath << std::endl;
    }

	enum class ShaderType {
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << "\n";
		}
	}

	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(
	unsigned int type,
	const std::string& source
)
{
    std::cout << "Shader::CompileShader" << std::endl;

    unsigned int id = 0;
    {
        FFunctionCallback funcCB = [&]()
        {
            id = glCreateShader(type);
        };
        GLCall(funcCB);
    }

	const char* src = source.c_str();

    {
        FFunctionCallback funcCB = [&]()
        {
            glShaderSource(id, 1, &src, nullptr);
        };
        GLCall(funcCB);
    }

    {
        FFunctionCallback funcCB = [&]()
        {
            glCompileShader(id);
        };
        GLCall(funcCB);
    }

	int result = 0;

    {
        FFunctionCallback funcCB = [&]()
        {
            glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        };
        GLCall(funcCB);
    }

	if (result == GL_FALSE) {
		int length = 0;

        {
            FFunctionCallback funcCB = [&]()
            {
                glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            };
            GLCall(funcCB);
        }

		//char* message = (char*)alloca(length * sizeof(char));
        char* message = new char[length * sizeof(char)];

        {
            FFunctionCallback funcCB = [&]()
            {
                glGetShaderInfoLog(id, length, &length, message);
            };
            GLCall(funcCB);
        }

		std::cout << "Failed to compile "
			<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< " shader!" << std::endl;

		std::cout << message << std::endl;

        {
            FFunctionCallback funcCB = [&]()
            {
                glDeleteShader(id);
            };
            GLCall(funcCB);
        }

        delete[] message;
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(
	const std::string& vertexShader,
	const std::string& fragmentShader
)
{
    std::cout << "Shader::CreateShader" << std::endl;

    const int openglVersionMajor = OpenGLSettings::GetMajorVersion();
    const int openglVersionMinor = OpenGLSettings::GetMinorVersion();
    std::cout << "openglVersionMajor = " << openglVersionMajor << std::endl;
    std::cout << "openglVersionMinor = " << openglVersionMinor << std::endl;
    const int openglSLVersionMajor = OpenGLSettings::GetMajorSLVersion();
    const int openglSLVersionMinor = OpenGLSettings::GetMinorSLVersion();
    std::cout << "openglSLVersionMajor = " << openglSLVersionMajor << std::endl;
    std::cout << "openglSLVersionMinor = " << openglSLVersionMinor << std::endl;

    unsigned int program = 0;
    {
        FFunctionCallback funcCB = [&]()
        {
            program = glCreateProgram();
        };
        GLCall(funcCB);
    }

	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    {
        FFunctionCallback funcCB = [&]()
        {
            glAttachShader(program, vs);
        };
        GLCall(funcCB);
    }

    if (openglVersionMajor <= 3 && openglVersionMinor < 3)
    {
        glBindAttribLocation(program, 0, "position");
        glBindAttribLocation(program, 1, "texCoord");
    }

    {
        FFunctionCallback funcCB = [&]()
        {
            glAttachShader(program, fs);
        };
        GLCall(funcCB);
    }

    if (openglVersionMajor <= 3 && openglVersionMinor < 3)
    {
        glBindAttribLocation(program, 0, "color");
    }

    // TODO - glBindAttribLocation()

    {
        FFunctionCallback funcCB = [&]()
        {
            glLinkProgram(program);
        };
        GLCall(funcCB);
    }

    {
        FFunctionCallback funcCB = [&]()
        {
            glValidateProgram(program);
        };
        GLCall(funcCB);
    }

    {
        FFunctionCallback funcCB = [&]()
        {
            glDeleteShader(vs);
        };
        GLCall(funcCB);
    }

    {
        FFunctionCallback funcCB = [&]()
        {
            glDeleteShader(fs);
        };
        GLCall(funcCB);
    }

	return program;
}

void Shader::Bind() const
{
    FFunctionCallback funcCB = [&]()
    {
        glUseProgram(m_RendererID);
    };
	GLCall(funcCB);
}

void Shader::Unbind() const
{
    FFunctionCallback funcCB = []()
    {
        glUseProgram(0);
    };
	GLCall(funcCB);
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    FFunctionCallback funcCB = [&]()
    {
        glUniform1i(GetUniformLocation(name), value);
    };
	GLCall(funcCB);
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    FFunctionCallback funcCB = [&]()
    {
        glUniform1f(GetUniformLocation(name), value);
    };
	GLCall(funcCB);
}

void Shader::SetUniform4f(
	const std::string& name,
	float v0,
	float v1,
	float v2,
	float v3
)
{
    FFunctionCallback funcCB = [&]()
    {
        glUniform4f( GetUniformLocation(name), v0, v1, v2, v3 );
    };
	GLCall(funcCB);
}

void Shader::SetUniformMat4f(
	const std::string& name,
	const glm::mat4& matrix
)
{
    FFunctionCallback funcCB = [&]()
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE,
		&matrix[0][0]);
    };
	GLCall(funcCB);
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
	{
		return m_UniformLocationCache[name];
	}

    int location = 0;
    {
        FFunctionCallback funcCB = [&]()
        {
            location = glGetUniformLocation( m_RendererID, name.c_str() );
        };
        GLCall(funcCB);
    }

	if (location == -1)
	{
		std::cout << "Warning: uniform " << name << " doesn't exist!" <<
			std::endl;
	}

	m_UniformLocationCache[name] = location;

	return location;
}
