#include "Ship.hpp"
#include <iomanip>


Ship::Ship(int shipLength) : length(shipLength) {
    if (shipLength > 4 || shipLength < 1) {
        throw std::invalid_argument("Size must be in range [1,4]");
    }
    for (int i = 0; i < shipLength; i++){
        segments.push_back(new ShipSegment());
    }
}

Ship::~Ship() {
    for (auto& segment : segments) {
        delete segment;
    }
}

int Ship::getLength() {
    return length;
}

bool Ship::getIsVertical() {
    return isVertical;
}

std::vector<ShipSegment*> Ship::getSegments() {
    return segments;
}

bool Ship::getIsPlaced() {
    return isPlaced;
}

void Ship::setIsPlaced(bool isPlaced) {
    this->isPlaced = isPlaced;
}

void Ship::setIsVertical(bool isVertical) {
    this->isVertical = isVertical;
}


