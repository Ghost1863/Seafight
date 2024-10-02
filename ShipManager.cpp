#include "ShipManager.hpp"
#include <iostream>


ShipManager::~ShipManager() {
    for (auto& ship : ships) {
        delete ship;
    }
}

std::vector<Ship*> ShipManager::getShips() {
    return ships;
}

Ship& ShipManager::getShipByCoordinates(Coordinates coords) {
    for (auto& ship : ships) {
        if (ship->getSegments()[0]->coord == coords)
            return *ship;
    }
}

void ShipManager::addShip(Ship* ship) {
    if (ship)
        ships.push_back(ship);
    else
        throw std::invalid_argument("Ship is null ptr");
}

void ShipManager::createDefaultShips() {
    std::vector<int> sizes = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

    for (int i = 0; i < size(sizes); i++) {
        ships.push_back(new Ship(sizes[i]));
    }
}

void ShipManager::printShipsInfo() {
    std::cout << "\nships amount: " << size(ships)-1 << "\n\n";
    for (auto& ship:ships) {
        std::cout << "length: " << ship->getLength() << "\n";
        for (int i = 0; i < size(ship->getSegments()); i++)
        {
            std::cout << "Segment " << i << " coords x: " << ship->getSegments()[i]->coord.x
                << " y: " << ship->getSegments()[i]->coord.y<<" status:";
            switch (ship->getSegments()[i]->status)
            {
            case SegmentStatus::INTACT:
                std::cout << "intact";
                break;
            case SegmentStatus::DAMAGED:
                std::cout << "damaged";
                break;
            case SegmentStatus::DESTROYED:
                std::cout << "destroyed";
                break;
            default:
                break;
            }
           std::cout<< "\n";
        }
        std::cout << "\n";
    }
}