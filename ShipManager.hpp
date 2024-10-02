#pragma once
#include "Ship.hpp"

class ShipManager {

private:
    std::vector<Ship*> ships;

public:
    ShipManager() : ships() {};
    ~ShipManager();

    std::vector<Ship*> getShips();
    Ship& getShipByCoordinates(Coordinates coords);

    void addShip(Ship* ship);
    void createDefaultShips();
    void printShipsInfo();
};
