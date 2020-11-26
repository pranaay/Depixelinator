#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION

#include <math.h>
#include "stb_image.h"
#include "Pixels.h"


#include<iostream>

Image::Image(const char *filename, unsigned int slot) {
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

    m_img = new Pixels *[m_Height];
    for (int i = 0; i < m_Height; i++) {
        m_img[i] = new Pixels[m_Width];
        // memset(m_img[i], (int)NULL, m_Height*sizeof(Pixels));
    }
    for (int i = 0; i < m_Height; i++) {
        for (int j = 0; j < m_Width; j++) {
            if (i == 0) {
                m_img[i][j].top = false;
                m_img[i][j].topright = false;
                m_img[i][j].topleft = false;
            }
            if (j == 0) {
                m_img[i][j].left = false;
                m_img[i][j].topleft = false;
                m_img[i][j].bottomleft = false;
            }
            if (i == m_Height - 1) {
                m_img[i][j].bottom = false;
                m_img[i][j].bottomleft = false;
                m_img[i][j].bottomright = false;
            }
            if (j == m_Width - 1) {
                m_img[i][j].right = false;
                m_img[i][j].topright = false;
                m_img[i][j].bottomright = false;
            }


            m_img[i][j].setRGB((short) m_Buffer[i * m_Width * 3 + 3 * j],
                               (short) m_Buffer[i * m_Width * 3 + 3 * j + 1],
                               (short) m_Buffer[i * m_Width * 3 + 3 * j + 2]
            );
        }
    }

    for (int i = 0; i < m_Height; i++) {
        for (int j = 0; j < m_Width; j++) {
            std::cout << (short) m_img[i][j].getR() << " "
                      << (short) m_img[i][j].getG() << " "
                      << (short) m_img[i][j].getB() << std::endl;
        }
    }


}

int Image::getWidth() {
    return m_Width;
}

unsigned char *Image::getBuffer() {
    return m_Buffer;
}

Image::~Image() {
    glDeleteTextures(1, &m_texId);
    if (m_Buffer)
        stbi_image_free(m_Buffer);
}

int Image::getHeight() {
    return m_Height;
}

unsigned int Image::getTexId() {
    return m_texId;
}

/*
 * Y = R *  .299000 + G *  .587000 + B *  .114000
U = R * -.168736 + G * -.331264 + B *  .500000 + 128
V = R *  .500000 + G * -.418688 + B * -.081312 + 128

 delta_Y= 48/255 ,
delta_U = 7/255 , or delta_V =6/255
 */
bool Image::isSimilar(Pixels A, Pixels B) {
    float Ay = A.getR() * 0.2990 + A.getG() * 0.5870 + A.getB() * 0.1140;
    float Au = A.getR() * -0.168736 + A.getG() * -0.331264f + A.getB() * 0.50 + 128;
    float Av = A.getR() * 0.500000 + A.getG() * -0.418688 + A.getB() * -0.081312 + 128;

    float By = B.getR() * 0.2990 + B.getG() * 0.5870 + B.getB() * 0.1140;
    float Bu = B.getR() * -0.168736 + B.getG() * -0.331264f + B.getB() * 0.50 + 128;
    float Bv = B.getR() * 0.500000 + B.getG() * -0.418688 + B.getB() * -0.081312 + 128;

    //std::cout << Ay << " " << Au << " " << Av << " " << std::endl;
    //std::cout << By << " " << Bu << " " << Bv << " " << std::endl;
    if (fabs(Ay - By) > 48.0 || fabs(Au - Bu) > 7.0 || fabs(Av - Bv) > 6.0) {
        return false;
    }

    return true;
}
