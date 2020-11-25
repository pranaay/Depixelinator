#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Pixels.h"


#include<iostream>
Image::Image(const char *filename, unsigned int slot){
    stbi_set_flip_vertically_on_load(0);
    m_Buffer = stbi_load(filename, &m_Width, &m_Height, &m_BPP, 3);

    glGenTextures(1, &m_texId);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_texId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_Buffer);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    m_img = new Pixels*[m_Height];
    for(int i=0; i<m_Height;i++){
        m_img[i] = new Pixels[m_Width];
       // memset(m_img[i], (int)NULL, m_Height*sizeof(Pixels));
    }
    for(int i=0; i<m_Height; i++){
        for(int j=0; j<m_Width; j++){
            m_img[i][j].setRGB((short)m_Buffer[i*m_Width*3 + 3*j],
                                (short)m_Buffer[i*m_Width*3 + 3*j+1],
                                (short)m_Buffer[i*m_Width*3 + 3*j+2]
                                );
        }
    }

    for(int i =0; i<m_Height;i++){
        for(int j=0;j<m_Width; j++){
            std::cout<<(short)m_img[i][j].getR() <<" "
                     <<(short)m_img[i][j].getG() <<" "
                     <<(short)m_img[i][j].getB() <<std::endl;
        }
    }


}

int Image::getWidth(){
	return m_Width;
}

unsigned char* Image::getBuffer() {
    return m_Buffer;
}

Image::~Image() {
    glDeleteTextures(1,&m_texId);
    if (m_Buffer)
        stbi_image_free(m_Buffer);
}

int Image::getHeight(){
	return m_Height;
}

unsigned int Image::getTexId() {
    return m_texId;
}