#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION

#include <math.h>
#include "stb_image.h"
#include "Pixels.h"
#include <vector>
#include<bits/stdc++.h>
#include "utils.h"
#include<iostream>

Image::Image(const char *filename, unsigned int slot) {
    stbi_set_flip_vertically_on_load(1);
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

    }
    for (int i = 0; i < m_Height; i++) {
        for (int j = 0; j < m_Width; j++) {
            if (i == 0) {
                m_img[i][j].adjacencyList.find("top")->second = false;
                m_img[i][j].adjacencyList.find("topRight")->second = false;
                m_img[i][j].adjacencyList.find("topLeft")->second = false;
            }
            if (j == 0) {
                m_img[i][j].adjacencyList.find("left")->second = false;
                m_img[i][j].adjacencyList.find("bottomLeft")->second = false;
                m_img[i][j].adjacencyList.find("topLeft")->second = false;
            }
            if (i == m_Height - 1) {
                m_img[i][j].adjacencyList.find("bottom")->second = false;
                m_img[i][j].adjacencyList.find("bottomRight")->second = false;
                m_img[i][j].adjacencyList.find("bottomLeft")->second = false;
            }
            if (j == m_Width - 1) {
                m_img[i][j].adjacencyList.find("right")->second = false;
                m_img[i][j].adjacencyList.find("topRight")->second = false;
                m_img[i][j].adjacencyList.find("bottomRight")->second = false;
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
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
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

void Image::createSimilarityGraph() {
    for (int i = 0; i < m_Height; i++) {
        for (int j = 0; j < m_Width; j++) {
            if (m_img[i][j].adjacencyList.find("top")->second && !isSimilar(m_img[i][j], m_img[i - 1][j])) {
                m_img[i][j].adjacencyList.find("top")->second = false;
            }

            if (m_img[i][j].adjacencyList.find("topRight")->second && !isSimilar(m_img[i][j], m_img[i - 1][j + 1])) {
                m_img[i][j].adjacencyList.find("topRight")->second = false;
            }
            if (m_img[i][j].adjacencyList.find("topLeft")->second && !isSimilar(m_img[i][j], m_img[i - 1][j - 1])) {
                m_img[i][j].adjacencyList.find("topLeft")->second = false;
            }
            if (m_img[i][j].adjacencyList.find("bottom")->second && !isSimilar(m_img[i][j], m_img[i + 1][j])) {
                m_img[i][j].adjacencyList.find("bottom")->second = false;
            }
            if (m_img[i][j].adjacencyList.find("bottomLeft")->second && !isSimilar(m_img[i][j], m_img[i + 1][j - 1])) {
                m_img[i][j].adjacencyList.find("bottomLeft")->second = false;
            }
            if (m_img[i][j].adjacencyList.find("bottomRight")->second && !isSimilar(m_img[i][j], m_img[i + 1][j + 1])) {
                m_img[i][j].adjacencyList.find("bottomRight")->second = false;
            }
            if (m_img[i][j].adjacencyList.find("left")->second && !isSimilar(m_img[i][j], m_img[i][j - 1])) {
                m_img[i][j].adjacencyList.find("left")->second = false;
            }
            if (m_img[i][j].adjacencyList.find("right")->second && !isSimilar(m_img[i][j], m_img[i][j + 1])) {
                m_img[i][j].adjacencyList.find("right")->second = false;
            }

        }
    }


}

unsigned int Image::getVbo() const {
    return VBO;
}

void Image::setVbo(unsigned int vbo) {
    VBO = vbo;
}

unsigned int Image::getIbo() const {
    return IBO;
}

void Image::setIbo(unsigned int ibo) {
    IBO = ibo;
}

void Image::createVbo() {
    float vertexData[2 * m_Width * m_Height] = {0};
    for (int i = 0; i < m_Height; i++) {
        for (int j = 0; j < m_Width; j++) {
            vertexData[i * m_Width * 2 + j * 2] = (-0.85 + (1.7 / std::max(m_Height, m_Width)) * j);
            vertexData[i * m_Width * 2 + j * 2 + 1] = -0.85 + (1.7 / std::max(m_Height, m_Width)) * i;
            m_img[i][j].setX(vertexData[i * m_Width * 2 + j * 2]);
            m_img[i][j].setY(vertexData[i * m_Width * 2 + j * 2 + 1]);
        }
    }
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 2 * m_Width * m_Height * sizeof(float), vertexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void *) 0);
}

void Image::createIbo() {
    std::vector<unsigned int> indices;
    for (int i = 0; i < m_Height; i++) {
        for (int j = 0; j < m_Width; j++) {
            if (m_img[i][j].adjacencyList.find("top")->second) {
                indices.push_back(i * m_Width + j);
                indices.push_back((i - 1) * m_Width + j);
            }
            if (m_img[i][j].adjacencyList.find("topRight")->second) {
                indices.push_back(i * m_Width + j);
                indices.push_back((i - 1) * m_Width + j + 1);
            }
            if (m_img[i][j].adjacencyList.find("topLeft")->second) {
                indices.push_back(i * m_Width + j);
                indices.push_back((i - 1) * m_Width + j - 1);
            }
            if (m_img[i][j].adjacencyList.find("bottom")->second) {
                indices.push_back(i * m_Width + j);
                indices.push_back((i + 1) * m_Width + j);
            }
            if (m_img[i][j].adjacencyList.find("bottomRight")->second) {
                indices.push_back(i * m_Width + j);
                indices.push_back((i + 1) * m_Width + j + 1);
            }
            if (m_img[i][j].adjacencyList.find("bottomLeft")->second) {
                indices.push_back(i * m_Width + j);
                indices.push_back((i + 1) * m_Width + j - 1);
            }
            if (m_img[i][j].adjacencyList.find("right")->second) {
                indices.push_back(i * m_Width + j);
                indices.push_back((i) * m_Width + j + 1);
            }
            if (m_img[i][j].adjacencyList.find("left")->second) {
                indices.push_back(i * m_Width + j);
                indices.push_back((i) * m_Width + j - 1);
            }
        }
    }
    numConn = indices.size();
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

int Image::getNumConn() const {
    return numConn;
}

void Image::setNumConn(int numConn) {
    Image::numConn = numConn;
}

void Image::hueristicsTaversal() {

    for (int i = 0; i <= m_Height - 1; i++) {
        for (int j = 0; j < m_Width - 1; j++) {
            if (m_img[i][j].adjacencyList.find("bottomRight")->second &&
                m_img[i + 1][j].adjacencyList.find("topRight")->second &&
                !m_img[i][j].adjacencyList.find("bottom")->second &&
                !m_img[i][j].adjacencyList.find("right")->second &&
                !m_img[i + 1][j].adjacencyList.find("right")->second &&
                !m_img[i][j + 1].adjacencyList.find("bottom")->second) {
                //std::cerr << "working here" << i << " " << j << std::endl;
                int w_lr = 0, w_rl = 0;
                //Curves heuristic

                resetVisited();
                int len_lr = lenCurve(i, j);
                //std::cerr << len_lr << " len_lr ";
                resetVisited();
                int len_rl = lenCurve(i + 1, j);
                //std::cerr << len_rl << " len_rl" << std::endl;
                if (len_lr > len_rl) {
                    w_lr = len_lr - len_rl;
                    w_rl = 0;
                } else if (len_lr < len_rl) {
                    w_rl = len_rl - len_lr;
                    w_lr = 0;
                } else {
                    w_lr = 0;
                    w_rl = 0;
                }

                //sparse heuristics
                int top = 0, bottom = 0;
                int l = 0, r = 0;
                int t = 0, b = 0;
                if (i - 4 < 0) {
                    t = 0;
                    b = i + 4 - (i - 4);
                } else if (i + 4 >= m_Height) {
                    b = m_Height - 1;
                    t = i - 4 + (m_Height - (i + 4));
                } else {
                    t = i - 4;
                    b = i + 4;
                }
                if (j - 4 < 0) {
                    l = 0;
                    r = j + 4 - (j - 4);
                } else if (j + 4 >= m_Width) {
                    r = m_Width - 1;
                    l = j - 4 + (m_Width - (j + 4));
                } else {
                    l = j - 4;
                    r = j + 4;
                }
                for (int ii = t; ii < b; ii++) {
                    for (int jj = l; jj < r; jj++) {
                        if (isSimilar(m_img[i][j], m_img[ii][jj])) {
                            top++;
                        }
                        if (isSimilar(m_img[i + 1][j], m_img[ii][jj])) {
                            bottom++;
                        }
                    }
                }
                if (top > bottom) {
                    w_rl += top - bottom;
                    w_lr += 0;
                } else if (bottom < top) {
                    w_lr += bottom - top;
                    w_rl += 0;
                } else {
                    w_lr += 0;
                    w_rl += 0;
                }
                //island heuristics
                if (m_img[i][j].valency() == 1 || m_img[i + 1][j + 1].valency() == 1) {
                    w_lr += 5;
                }
                if (m_img[i + 1][j].valency() == 1 || m_img[i][j + 1].valency() == 1) {
                    w_rl += 5;
                }
                //final evaluation
                if (w_lr > w_rl) {
                    m_img[i + 1][j].adjacencyList.find("topRight")->second = false;
                    m_img[i][j + 1].adjacencyList.find("bottomLeft")->second = false;
                } else if (w_lr < w_rl) {
                    m_img[i][j].adjacencyList.find("bottomRight")->second = false;
                    m_img[i + 1][j + 1].adjacencyList.find("topLeft")->second = false;
                } else {
                    m_img[i][j].adjacencyList.find("bottomRight")->second = false;
                    m_img[i + 1][j + 1].adjacencyList.find("topLeft")->second = false;
                    m_img[i + 1][j].adjacencyList.find("topRight")->second = false;
                    m_img[i][j + 1].adjacencyList.find("bottomLeft")->second = false;

                }
            }
        }
    }

}

int Image::lenCurve(int x, int y) {
    if (x >= m_Height || y >= m_Width) return -1;
    if (visited[x][y] == 1) return -1;
    visited[x][y] = 1;
    if (m_img[x][y].valency() != 2 && m_img[x][y].valency() != 1) {
        return 0;
    } else if (m_img[x][y].valency() == 1) {
        std::vector <std::string> l = m_img[x][y].getList();
        return (lenCurve(x + mappingX(l[0]), y + mappingY(l[0])) + 1);

    } else {
        std::vector <std::string> l = m_img[x][y].getList();
        return (lenCurve(x + mappingX(l[0]), y + mappingY(l[0])) +
                lenCurve(x + mappingX(l[1]), y + mappingY(l[1])) + 2);
    }

}

int Image::mappingX(std::string x) {
    if (x == "topRight") return -1;
    if (x == "topLeft") return -1;
    if (x == "top") return -1;
    if (x == "right") return 0;
    if (x == "left") return 0;
    if (x == "bottomRight") return 1;
    if (x == "bottomLeft") return 1;
    if (x == "bottom") return 1;
}

int Image::mappingY(std::string x) {
    if (x == "topRight") return 1;
    if (x == "topLeft") return -1;
    if (x == "top") return 0;
    if (x == "right") return 1;
    if (x == "left") return -1;
    if (x == "bottomRight") return 1;
    if (x == "bottomLeft") return -1;
    if (x == "bottom") return 0;
}

void Image::resetVisited() {
    visited = new int *[m_Height];
    for (int ii = 0; ii < m_Height; ii++) {
        visited[ii] = new int[m_Width];
        memset(visited[ii], 0, m_Width * sizeof(int));
    }
}

void Image::render() {
    resetVisited();
    for (int i = 0; i < m_Height; i++) {
        for (int j = 0; j < m_Width; j++) {
            if (!visited[i][j]) {
                if (m_img[i][j].valency() == 1) {

                }
            }
        }
    }
}

unsigned int Image::fitCurve(std::vector<float> points) {

    std::vector<float> bezier;
    std::vector<float> ax, ay;
    std::vector<float> bx, by;

    int sz = points.size() / 3;
    float x[4] = {0}, y[4] = {0};
    float delta_t = 1.0 / (50 - 1.0);
    float t;
    if (sz == 0) {
        return 0;
    } else if (sz == 1) {
        bezier.push_back(points[0]);
        bezier.push_back(points[1]);
        bezier.push_back(0.0);
        return 0;
    } else if (sz == 2) {
        bezier.push_back(points[0]);
        bezier.push_back(points[1]);
        bezier.push_back(0.0);
        bezier.push_back(points[3]);
        bezier.push_back(points[4]);
        bezier.push_back(0.0);
        return 0;
    } else {
        ax = thomasAlgo(points, 0);
        ay = thomasAlgo(points, 1);
        bx = getB(points, ax, 0);
        by = getB(points, ay, 1);
    }


    for (int i = 0; i < (sz - 1); i += 1) {

        x[0] = points[i * 3];
        y[0] = points[i * 3 + 1];
        x[1] = ax[i + 1];
        y[1] = ay[i + 1];
        x[2] = bx[i + 1];
        y[2] = by[i + 1];
        x[3] = points[i * 3 + 3];
        y[3] = points[i * 3 + 4];

        bezier.push_back(x[0]);
        bezier.push_back(y[0]);
        bezier.push_back(0.0);

        t = 0.0;
        for (float j = 1; j < (50 - 1); j++) {
            t += delta_t;
            bezier.push_back(x[0] * pow((1 - t), 3) + 3 * x[1] * pow((1 - t), 2) * t + 3 * x[2] * pow(t, 2) * (1 - t) +
                             x[3] * pow(t, 3));
            bezier.push_back(y[0] * pow((1 - t), 3) + 3 * y[1] * pow((1 - t), 2) * t + 3 * y[2] * pow(t, 2) * (1 - t) +
                             y[3] * pow(t, 3));
            bezier.push_back(0.0);
        }
    }
    bezier.push_back(x[3]);
    bezier.push_back(y[3]);
    bezier.push_back(0.0);
    unsigned int bez;
    glGenBuffers(1, &bez);
    glBindBuffer(GL_ARRAY_BUFFER, bez);
    glBufferData(GL_ARRAY_BUFFER, bezier.size() * sizeof(float), &bezier[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return bez;
}

std::vector<float> Image::thomasAlgo(std::vector<float> &points, int flag) {
    float W;
    int sz = points.size() / 3;
    std::vector<float> ans(sz), ai, bi, ci, di; //All are one-indexed

    di.push_back(0);
    di.push_back(points[0 + flag] + 2 * points[3 + flag]);
    for (int i = 1; i < sz - 2; i++) {
        di.push_back(2 * (2 * points[3 * i + flag] + points[3 * (i + 1) + flag]));
    }
    di.push_back(8 * points[3 * (sz - 2) + flag] + points[3 * (sz - 1) + flag]);

    ai.push_back(0);
    ai.push_back(0);
    for (int i = 1; i < sz - 2; i++) {
        ai.push_back(1);
    }
    ai.push_back(2);

    bi.push_back(0);
    bi.push_back(2);
    for (int i = 1; i < sz - 2; i++) {
        bi.push_back(4);
    }
    bi.push_back(7);

    ci.push_back(0);
    for (int i = 0; i < sz; i++) {
        ci.push_back(1);
    }

    for (int i = 2; i <= sz - 1; i++) {
        W = ai[i] / bi[i - 1];
        bi[i] = bi[i] - W * ci[i - 1];
        di[i] = (di[i] - W * di[i - 1]);

    }
    ans[sz - 1] = di[sz - 1] / bi[sz - 1];
    for (int i = sz - 2; i >= 1; i--) {
        ans[i] = (di[i] - ci[i] * ans[i + 1]) / bi[i];
    }
    return ans;


}

std::vector<float> Image::getB(std::vector<float> &points, std::vector<float> &ai, int flag) {
    int sz = points.size() / 3;
    std::vector<float> b;
    b.push_back(0);
    for (int i = 0; i < sz - 2; i++) {
        b.push_back(2 * points[3 * (i + 1) + flag] - ai[i + 2]);
    }
    b.push_back((ai[sz - 1] + points[3 * (sz - 1) + flag]) / 2);

    return b;
}