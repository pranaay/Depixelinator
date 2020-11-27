//
// Created by darthwho on 24/11/20.
//

#ifndef PROJECT_PIXELS_H
#define PROJECT_PIXELS_H

#include <map>

class Pixels {
private:
    unsigned short R;
    unsigned short G;
    unsigned short B;
    float x;
    float y;

public:
    //Pixel(unsigned short r,unsigned short g,unsigned short b);
    Pixels();

    unsigned short getR();

    unsigned short getG();

    unsigned short getB();

    std::map<std::string, bool> adjacencyList;

    void setRGB(unsigned short r, unsigned short g, unsigned short b);

    float getX() const;

    void setX(float x);

    float getY() const;

    void setY(float y);

};

#endif //PROJECT_PIXELS_H
