#include "ShipManager.hpp"
#include <iostream>

ShipManager::ShipManager() {
    ships.push_back(new Ship(0,4));
    ships.push_back(new Ship(1,3));
    ships.push_back(new Ship(2,3));
    ships.push_back(new Ship(3,2));
    ships.push_back(new Ship(4,2));
    ships.push_back(new Ship(5,2));
    ships.push_back(new Ship(6,1));
    ships.push_back(new Ship(7,1));
    ships.push_back(new Ship(8,1));
    ships.push_back(new Ship(9,1));
}

ShipManager::~ShipManager() {
    for (auto& ship : ships) {
        delete ship;
    }
}

std::vector<Ship*> ShipManager::getShips() {
    return ships;
}

Ship& ShipManager::getShipById(int s_id) {
    for (auto& ship : ships) {
        if (ship->getId() == s_id)
            return *ship;
    } 
}


Ship& ShipManager::getShipByCoordinates(Coordinates coords) {
    for (auto& ship : ships) {
        if (ship->getCoords()==coords)
            return *ship;
    }
}

void ShipManager::printShipsInfo() {
    std::cout << "ships amount: " << size(ships)-1 << "\n\n";
    for (auto& ship:ships) {
        std::cout << "id: " << ship->getId() << "\n"<<
            "length: " << ship->getLength() << "\n";
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

void ShipManager::registerDamage(Coordinates hitCoords) {
    for (auto& ship : ships) {
        for (auto& segment : ship->getSegments()) {
            if (segment->coord == hitCoords) {
                if (segment->status == SegmentStatus::INTACT) {
                    segment->status = SegmentStatus::DAMAGED;
                }
                else if (segment->status == SegmentStatus::DAMAGED) {
                    segment->status = SegmentStatus::DESTROYED;
                    bool isDestroyed = true;
                    for (auto& currSegm : ship->getSegments()) {
                        if (currSegm->status != SegmentStatus::DESTROYED) {
                            isDestroyed = false;
                        }
                    }
                    if (isDestroyed) {
                        std::cout << "ship with id: " << ship->getId() << " on coords x: " << ship->getCoords().x
                            << " y: " << ship->getCoords().y << " is destroyed\n";
                    }
                }
                else if (segment->status == SegmentStatus::DESTROYED) {
                    std::cout << "segment x: " << segment->coord.x << " y: " << segment->coord.y << " already destroyed\n";
                }
                return;
            }
        }
    }

}