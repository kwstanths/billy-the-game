#include "OpenGLTexture.hpp"

#include <cstdio>
#include <cstring>

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image/stb_image.h"

#include "game_engine/ErrorCodes.hpp"

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

namespace game_engine {
namespace graphics {
namespace opengl {

    OpenGLTexture::OpenGLTexture() {
        is_inited_ = false;
    }

    int OpenGLTexture::Init(std::string file_path, OpenGLTextureType type) {
        if (is_inited_) return -1;

        int ret;
        if (type == OpenGLTextureType::TEXTURE_STB) ret = LoadSTB(file_path.c_str(), &texture_);
        else ret = LoadDDS(file_path.c_str(), &texture_);

        if (ret != 0) return ret;

        is_inited_ = true;
        return 0;
    }

    int OpenGLTexture::Destroy() {

        glDeleteTextures(1, &texture_);

        is_inited_ = false;
        return 0;
    }

    bool OpenGLTexture::IsInited() {
        return is_inited_;
    }

    GLuint OpenGLTexture::GetID() {
        if (!is_inited_) return 0;

        return texture_;
    }

    int OpenGLTexture::LoadDDS(const char * imagepath, GLuint * texture_id) {

        unsigned char header[124];

        FILE *fp;

        /* try to open the file */
        fp = fopen(imagepath, "rb");
        if (fp == NULL) return Error::ERROR_ASSET_NOT_FOUND;

        /* verify the type of file */
        char filecode[4];
        fread(filecode, 1, 4, fp);
        if (strncmp(filecode, "DDS ", 4) != 0) {
            fclose(fp);
            return Error::ERROR_ASSET_NOT_DDS;
        }

        /* get the surface desc */
        fread(&header, 124, 1, fp);

        unsigned int height = *(unsigned int*)&(header[8]);
        unsigned int width = *(unsigned int*)&(header[12]);
        unsigned int linearSize = *(unsigned int*)&(header[16]);
        unsigned int mipMapCount = *(unsigned int*)&(header[24]);
        unsigned int fourCC = *(unsigned int*)&(header[80]);


        unsigned char * buffer;
        unsigned int bufsize;
        /* how big is it going to be including all mipmaps? */
        bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
        buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
        fread(buffer, 1, bufsize, fp);
        /* close the file pointer */
        fclose(fp);

        unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
        unsigned int format;
        switch (fourCC)
        {
        case FOURCC_DXT1:
            format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            break;
        case FOURCC_DXT3:
            format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            break;
        case FOURCC_DXT5:
            format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            break;
        default:
            free(buffer);
            return Error::ERROR_ASSET_DDS_COMPRESSION;
        }

        // Create one OpenGL texture
        GLuint textureID;
        glGenTextures(1, &textureID);

        // "Bind" the newly created texture : all future texture functions will modify this texture
        glBindTexture(GL_TEXTURE_2D, textureID);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
        unsigned int offset = 0;

        /* load the mipmaps */
        for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
        {
            unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
            glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
                0, size, buffer + offset);

            offset += size;
            width /= 2;
            height /= 2;

            // Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
            if (width < 1) width = 1;
            if (height < 1) height = 1;

        }

        free(buffer);

        *texture_id = textureID;
        return 0;
    }

    int OpenGLTexture::LoadSTB(const char * imagepath, GLuint * texture_id) {

        int width, height, channels;
        unsigned char * data = stbi_load(imagepath, &width, &height, &channels, STBI_rgb);
        if (!data) return Error::ERROR_ASSET_NOT_FOUND;

        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        /* Configure wrapping and zooming behaviour */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        /* Generate the mipmaps */
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);

        *texture_id = textureID;

        return 0;
    }

}
}
}
