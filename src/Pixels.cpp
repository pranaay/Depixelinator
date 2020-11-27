//
// Created by darthwho on 24/11/20.
//

#include "Pixels.h"

void Pixels::setRGB(unsigned short r, unsigned short g, unsigned short b) {
    R = r;
    G = g;
    B = b;
}

Pixels::Pixels() {

    adjacencyList.insert(std::pair<std::string, bool>("top", true));
    adjacencyList.insert(std::pair<std::string, bool>("topLeft", true));
    adjacencyList.insert(std::pair<std::string, bool>("topRight", true));
    adjacencyList.insert(std::pair<std::string, bool>("bottom", true));
    adjacencyList.insert(std::pair<std::string, bool>("bottomLeft", true));
    adjacencyList.insert(std::pair<std::string, bool>("bottomRight", true));
    adjacencyList.insert(std::pair<std::string, bool>("right", true));
    adjacencyList.insert(std::pair<std::string, bool>("left", true));

}

unsigned short Pixels::getR() {
    return R;
}

unsigned short Pixels::getG() {
    return G;
}

unsigned short Pixels::getB() {
    return B;
}


float Pixels::getX() const {
    return x;
}

void Pixels::setX(float x) {
    Pixels::x = x;
}

float Pixels::getY() const {
    return y;
}

void Pixels::setY(float y) {
    Pixels::y = y;
}
