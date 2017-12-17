#include "OpenGLTexture.hpp"

#include <cstdio>

#include "../ErrorCodes.hpp"

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

namespace game_engine {

    OpenGLTexture::OpenGLTexture() {
        is_inited_ = false;
    }

    int OpenGLTexture::Init(std::string file_path, texture_type type) {
        if (is_inited_) return -1;

        int ret;
        if (type == texture_type::TEXTURE_BMP) ret = LoadBMP(file_path.c_str(), &texture_);
        else ret = LoadDDS(file_path.c_str(), &texture_);

        if (ret != 0) return ret;

        is_inited_ = true;
        return 0;
    }

    int OpenGLTexture::Destory() {

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

    int OpenGLTexture::LoadBMP(const char * imagepath, GLuint * texture_id) {

	    // Data read from the header of the BMP file
	    unsigned char header[54];
	    unsigned int dataPos;
	    unsigned int imageSize;
	    unsigned int width, height;
	    // Actual RGB data
	    unsigned char * data;

	    // Open the file
	    FILE * file = fopen(imagepath,"rb");
	    if (!file){
            return Error::ERROR_ASSET_NOT_FOUND;
	    }

	    // Read the header, i.e. the 54 first bytes

	    // If less than 54 bytes are read, problem
	    if ( fread(header, 1, 54, file)!=54 ){ 
	    	fclose(file);
            return Error::ERROR_ASSET_BMP_ERROR;
	    }
	    // A BMP files always begins with "BM"
	    if ( header[0]!='B' || header[1]!='M' ){
	    	fclose(file);
            return Error::ERROR_ASSET_BMP_ERROR;
	    }
	    // Make sure this is a 24bpp file
	    if ( *(int*)&(header[0x1E])!=0  ){
            fclose(file);
            return Error::ERROR_ASSET_BMP_ERROR;
        }
	    if ( *(int*)&(header[0x1C])!=24 ){
            fclose(file); 
            return Error::ERROR_ASSET_BMP_ERROR;
        }

	    // Read the information about the image
	    dataPos    = *(int*)&(header[0x0A]);
	    imageSize  = *(int*)&(header[0x22]);
	    width      = *(int*)&(header[0x12]);
	    height     = *(int*)&(header[0x16]);

	    // Some BMP files are misformatted, guess missing information
	    if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
	    if (dataPos==0)      dataPos=54; // The BMP header is done that way

	    // Create a buffer
	    data = new unsigned char [imageSize];

	    // Read the actual data from the file into the buffer
	    fread(data,1,imageSize,file);

	    // Everything is in memory now, the file can be closed.
	    fclose (file);

	    // Create one OpenGL texture
	    GLuint textureID;
	    glGenTextures(1, &textureID);
	    
	    // "Bind" the newly created texture : all future texture functions will modify this texture
	    glBindTexture(GL_TEXTURE_2D, textureID);

	    // Give the image to OpenGL
	    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	    // OpenGL has now copied the data. Free our own version
	    delete [] data;

	    // Poor filtering, or ...
	    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

	    // ... nice trilinear filtering ...
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	    // ... which requires mipmaps. Generate them automatically.
	    glGenerateMipmap(GL_TEXTURE_2D);

	    // Return the ID of the texture we just created
        *texture_id = textureID;
        
        return 0;
    }

}