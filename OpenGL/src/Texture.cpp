#include "Texture.h"
#include "vendor/stb_image/stb_image.h"

Texture::Texture(const std::string& path)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0),
	m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);

	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

    {
        FFunctionCallback functionToCall = [&]()
        {
            glGenTextures(1, &m_RendererID);
        };
        GLCall(functionToCall);
    }

    {
        FFunctionCallback functionToCall = [&]()
        {
            glBindTexture(GL_TEXTURE_2D, m_RendererID);
        };
        GLCall(functionToCall);
    }

    {
        FFunctionCallback functionToCall = []()
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        };
        GLCall(functionToCall);
    }

    {
        FFunctionCallback functionToCall = []()
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        };
        GLCall(functionToCall);
    }

    {
        FFunctionCallback functionToCall = []()
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        };
        GLCall(functionToCall);
    }

    {
        FFunctionCallback functionToCall = []()
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        };
        GLCall(functionToCall);
    }

    {
        FFunctionCallback functionToCall = [&]()
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
        };
        GLCall(functionToCall);
    }

    {
        FFunctionCallback functionToCall = []()
        {
            glBindTexture(GL_TEXTURE_2D, 0);
        };
        GLCall(functionToCall);
    }

	if (m_LocalBuffer)
	{
		stbi_image_free(m_LocalBuffer);
	}
}

Texture::~Texture()
{
    FFunctionCallback funcToCall = [&]()
    {
        glDeleteTextures(1, &m_RendererID);
    };
	GLCall(funcToCall);
}

void Texture::Bind(unsigned int slot) const
{
    {
        FFunctionCallback funcToCall = [&]()
        {
            glActiveTexture(GL_TEXTURE0 + slot);
        };
        GLCall(funcToCall);
    }

    {
        FFunctionCallback funcToCall = [&]()
        {
            glBindTexture(GL_TEXTURE_2D, m_RendererID);
        };
        GLCall(funcToCall);
    }
}

void Texture::Unbind() const
{
    FFunctionCallback funcCB = []()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    };
	GLCall(funcCB);
}
