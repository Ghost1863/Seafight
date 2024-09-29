#pragma once
#include <iostream>

enum class CellStatus { HIDDEN, DISCLOSED };
enum class CellValue { Empty, Miss, ShipPart, Hit, Destroyed };
enum class SegmentStatus { INTACT, DAMAGED, DESTROYED };

struct Coordinates {
    int x;
    int y;
    
    bool operator==(const Coordinates& other) const {
        return x == other.x && y == other.y;
    }
};

struct FieldCell {
    Coordinates coord;
    CellStatus status;
    CellValue value;
};

struct ShipSegment {
    Coordinates coord;
    SegmentStatus status;
    ShipSegment() : coord({ 0, 0 }), status(SegmentStatus::INTACT) {}
    ShipSegment(int hp, Coordinates coord, SegmentStatus status) : coord(coord), status(status) {}
};