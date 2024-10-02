#pragma once
#include "Structures.hpp"
#include <string>
#include <vector>


class Ship {

private:
    int length;
    bool isPlaced = false;
    bool isVertical = false;
    std::vector<ShipSegment*> segments;

public:
    Ship(int shipLength);
    ~Ship();
    int getLength();
    std::vector<ShipSegment*> getSegments();
    bool getIsPlaced();
    bool getIsVertical();
    bool isVerticalOrientation();
    
    void setIsPlaced(bool isPlaced);
    void setIsVertical(bool isVertical);
};









