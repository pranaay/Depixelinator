//
// Created by darthwho on 24/11/20.
//

#ifndef PROJECT_PIXELS_H
#define PROJECT_PIXELS_H
class Pixels{
private:
    unsigned short R;
    unsigned short G;
    unsigned short B;

public:
    //Pixel(unsigned short r,unsigned short g,unsigned short b);
    Pixels();
    unsigned short getR();
    unsigned short getG();
    unsigned short getB();

    void setRGB(unsigned short r,unsigned short g, unsigned short b);

};
#endif //PROJECT_PIXELS_H
