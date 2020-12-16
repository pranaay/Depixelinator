#ifndef IMAGE_H
#define IMAGE_H

#include "Pixels.h"
#include "GL/gl3w.h"

class Image {

public:
    ~Image();

    Image(const char *filename, unsigned int slot);

    int getWidth();

    int getHeight();

    unsigned int getTexId();

    unsigned char *getBuffer();

    bool isSimilar(Pixels A, Pixels B);

    void createSimilarityGraph();

    unsigned int getVbo() const;

    void setVbo(unsigned int vbo);

    unsigned int getIbo() const;

    void setIbo(unsigned int ibo);

    void createVbo();

    void createIbo();

    int getNumConn() const;

    void setNumConn(int numConn);

    void hueristicsTaversal();

    int lenCurve(int x, int y);

    int mappingX(std::string x);

    int mappingY(std::string x);

    void resetVisited();

private:
    int m_Height;
    int m_Width;
    int m_BPP;
    unsigned int m_texId;
    unsigned char *m_Buffer;
    Pixels **m_img;
    unsigned int VBO;
    unsigned int IBO;
    int numConn;
    int **visited;


};

#endif