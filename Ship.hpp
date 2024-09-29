#pragma once
#include "Structures.hpp"
#include <string>
#include <vector>


class Ship {

private:
    int length;
    bool isPlaced = false;
    bool isVertical = false;
    Coordinates coords{0,0};
    std::vector<ShipSegment*> segments;

public:
    Ship(int shipLength);
    ~Ship();
    int getLength();
    std::vector<ShipSegment*> getSegments();
    bool getIsPlaced();
    bool getIsVertical();
    Coordinates getCoords();
    bool isVerticalOrientation();
    
    void setCoords(Coordinates coords);
    void setIsPlaced(bool isPlaced);
    void setIsVertical(bool isVertical);
    void addSegment(ShipSegment* segment);
};









