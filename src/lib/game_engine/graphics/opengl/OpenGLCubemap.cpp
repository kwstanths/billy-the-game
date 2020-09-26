#include "OpenGLCubemap.hpp"

#include "external/stb_image/stb_image.h"

#include "game_engine/core/ErrorCodes.hpp"

namespace game_engine { namespace graphics { namespace opengl {

    OpenGLCubemap::OpenGLCubemap()
    {
        is_inited_ = false;
    }

    int OpenGLCubemap::Init(std::vector<std::string>& faces)
    {
        int ret = LoadSTB(faces, &texture_);

        is_inited_ = true;
        return ret;
    }

    int OpenGLCubemap::Destroy()
    {
        return 0;
    }

    bool OpenGLCubemap::IsInited()
    {
        return false;
    }

    GLuint OpenGLCubemap::GetID()
    {
        if (!is_inited_) return 0;
     
        return texture_;
    }

    void OpenGLCubemap::ActivateTexture(int texture_id)
    {
        glActiveTexture(GL_TEXTURE0 + texture_id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, GetID());
    }

    int OpenGLCubemap::LoadSTB(std::vector<std::string>& faces, GLuint * texture_id)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, channels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            unsigned char * data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
            
            if (!data) {
                stbi_image_free(data);
                glDeleteTextures(1, &textureID);
                return Error::ERROR_ASSET_NOT_FOUND;
            }

            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        *texture_id = textureID;
        return 0;
    }

}
}
}