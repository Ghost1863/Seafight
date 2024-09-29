#pragma once
#include "Ship.hpp"

class ShipManager {

private:
    std::vector<Ship*> ships;

public:
    ShipManager();
    ~ShipManager();
    std::vector<Ship*> getShips();
    Ship& getShipById(int s_id);
    Ship& getShipByCoordinates(Coordinates coords);

    void printShipsInfo();
    void registerDamage(Coordinates hitCoords);
};
