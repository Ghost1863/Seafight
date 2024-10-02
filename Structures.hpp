#pragma once
#include <iostream>

enum class CellStatus { HIDDEN, DISCLOSED };
enum class CellValue {Empty, Miss, ShipSegment};
enum class SegmentStatus { INTACT, DAMAGED, DESTROYED };

struct Coordinates {
    int x;
    int y;
    
    bool operator==(const Coordinates& other) const {
        return x == other.x && y == other.y;
    }
};

struct ShipSegment {
    Coordinates coord;
    SegmentStatus status;
    ShipSegment() : coord({ 0, 0 }), status(SegmentStatus::INTACT) {}
    ShipSegment(Coordinates coord, SegmentStatus status) : coord(coord), status(status) {}

    bool operator==(const ShipSegment& other) const {
        return (coord == other.coord) && (status == other.status);
    }
};

struct FieldCell {
    Coordinates coord;
    CellStatus status;
    CellValue value;
    ShipSegment* shipSegment=nullptr;
};

