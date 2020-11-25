//
// Created by darthwho on 24/11/20.
//

#include "Pixels.h"

void Pixels::setRGB(unsigned short r,unsigned short g, unsigned short b){
    R = r;
    G = g;
    B = b;
}

Pixels::Pixels() {

}
unsigned short Pixels::getR(){
    return R;
}

unsigned short Pixels::getG(){
    return G;
}
unsigned short Pixels::getB(){
    return B;
}
