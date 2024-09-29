#include "ShipManager.hpp"
#include <iostream>

ShipManager::ShipManager() {
    std::vector<int> sizes = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

    for (int i = 0; i < size(sizes); i++) {
        ships.push_back(new Ship (sizes[i]));
    }
}

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
        if (ship->getCoords()==coords)
            return *ship;
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
                        std::cout <<"ship on coords x: " << ship->getCoords().x
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