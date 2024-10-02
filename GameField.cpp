#include "GameField.hpp"
#include <random>
#include <chrono>


void setColor(int foreground, int background = 40, int attributes = 0) {
	std::cout << "\033[" << attributes << ";" << foreground << ";" << background << "m";
}
void resetColor() {
	std::cout << "\033[0m"; 
}

GameField::GameField(int gf_width, int gf_height) {
	width = gf_width;
	height = gf_height;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			field.push_back(FieldCell{ Coordinates{x,y}, CellStatus::DISCLOSED, CellValue::Empty });
		}
	}
}

GameField::GameField(const GameField& other)
	: height(other.height), width(other.width), field(other.field) {}

GameField& GameField::operator=(const GameField& other) {
	if (this != &other) {
		width = other.width;
		height = other.height;
		field = other.field;
	}
	return *this;
}

GameField::GameField(GameField&& other) 
	: height(other.height), width(other.width), field(std::move(other.field)){
		other.width = 0;
		other.height = 0;
		other.field.clear();
}

GameField& GameField::operator=(GameField&& other) {
	if (this != &other) {
		width = other.width;
		height = other.height;
		field = std::move(other.field);
		other.width = 0;
		other.height = 0;
	}
	return *this;
}

void GameField::drawField() {
	std::cout << " ";
	for (int i = 0; i < width; i++)
	{	
		setColor(36);
		std::cout << "  "<<i<<" ";
	}
	resetColor();
	std::cout << "\n ";
	for (int i = 0; i < width; i++)
	{
		std::cout << "+ - ";
	}
	std::cout << "+ \n | ";
	for (auto& cell : field) {
		
		if (cell.status == CellStatus::HIDDEN) {
			setColor(34);//BLUE
			std::cout << "~";
		}
		else {
			if (cell.status == CellStatus::DISCLOSED) 
				if (cell.shipSegment != nullptr) {
					switch (cell.shipSegment->status) {
					case SegmentStatus::INTACT:
						setColor(32);//GREEN
						std::cout << "O";
						break;

					case SegmentStatus::DAMAGED:
						setColor(33);//YELLOW
						std::cout << "O";
						break;

					case SegmentStatus::DESTROYED:
						setColor(31);//RED
						std::cout << "O";
						break;
					default:
						break;
					}
				}
				else {
					switch (cell.value) {
					case CellValue::Empty:
						setColor(34);//BLUE
						std::cout << "#";
						break;
					case CellValue::Miss:
						setColor(31);//RED
						std::cout << "x";
						break;
					default:
						break;
					}
				}
		}
		resetColor();
		if (cell.coord.x == width - 1) {
			std::cout << " | ";
			setColor(36);
			std::cout << cell.coord.y;
			resetColor();
		}
		else
			std::cout << " | ";
		if (cell.coord.x == width-1) {
			std::cout << "\n ";
			for (int i = 0; i < width; i++)
			{
				std::cout << "+ - ";
			}
			std::cout << "+ ";
			if ((cell.coord.y)!=height-1)
			std::cout << "\n | ";
		}
	}
	std::cout << '\n';
}

bool GameField::checkCurrentCoord(int x,int y) {
	if (x<0 || x>width - 1 || y<0 || y>height - 1) {
		return false;
	}
	return true;
}

bool GameField::checkCoordsAround(int x, int y) {
	if (checkCurrentCoord(x,y)) {
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if (checkCurrentCoord(x + i,y + j)) {
					if (field[x + i + (y + j) * width].shipSegment != nullptr) {
						return false;
					}
				}
			}
		}
	}
	else return false;
	
	return true;
}

void GameField::setShip(Coordinates coords,Ship* ship, bool isVertical) {
	if (!ship)
		return;
	bool ableToPlaceShip = true;
	if (checkCoordsAround(coords.x, coords.y)) {
		for (int i = 1; i < ship->getLength(); i++)
		{	
			if (isVertical) {
				ableToPlaceShip = checkCoordsAround(coords.x, coords.y+i);
			}
			else {
				ableToPlaceShip= checkCoordsAround(coords.x + i, coords.y);
			}
			if (!ableToPlaceShip)
				return;
		}
		ship->getSegments()[0]->coord = Coordinates{ coords.x ,coords.y };
		field[coords.x + coords.y * width].shipSegment = ship->getSegments()[0];
		field[coords.x + (coords.y) * width].value = CellValue::ShipSegment;
	}
	else {
		return;
	}

	ship->setIsVertical(isVertical);
	ship->setIsPlaced(true);

	if (isVertical) {
		//start point is up
		for (int i = 1; i < ship->getLength(); i++)
		{
			ship->getSegments()[i]->coord= Coordinates{ coords.x ,coords.y + i };
			field[coords.x + (coords.y + i) * width].shipSegment = ship->getSegments()[i];
			field[coords.x + (coords.y + i) * width].value = CellValue::ShipSegment;
		}
	}
	else {
		//start point is left
		for (int i = 1; i <  ship->getLength(); i++)
		{
			ship->getSegments()[i]->coord = Coordinates{ coords.x+i,coords.y};
			field[coords.x + i + (coords.y * width)].shipSegment = ship->getSegments()[i];
			field[coords.x + i + (coords.y * width)].value = CellValue::ShipSegment;
		}
	}
}

void GameField::setAllShips(std::vector<Ship*> ships) {
	for (auto& ship : ships) {
		while (!ship->getIsPlaced()) {

			std::mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());
			std::uniform_int_distribution<int> distr(0, 9);
			std::uniform_int_distribution<int> distr_bool(0, 1);

			int x = distr(gen);
			int y = distr(gen);
			bool random_bool = static_cast<bool>(distr_bool(gen));

			this->setShip(Coordinates{ x, y }, ship, random_bool);
			
		}
	}
}

void GameField::attackCell(Coordinates coords) {
	if (!checkCurrentCoord(coords.x, coords.y)) {
		std::cout << "Coordinates x: " << coords.x << " y:" << coords.y << " out of range\n";
		return;
	}
	FieldCell& cell = field[coords.x + coords.y * width];
	cell.status = CellStatus::DISCLOSED;
	/*if (cell.value == CellValue::ShipSegment) {
		switch (cell.shipSegment->status) {
		case SegmentStatus::INTACT:
			cell.shipSegment->status = SegmentStatus::DAMAGED;
			std::cout << "segment damaged x: " << coords.x << " y: " << coords.y << "\n";
			break;

		case SegmentStatus::DAMAGED:
			cell.shipSegment->status = SegmentStatus::DESTROYED;
			std::cout << "segment destroyed x: " << coords.x << " y: " << coords.y << "\n";
			break;

		case SegmentStatus::DESTROYED:
			std::cout << "segment x: " << coords.x << " y: " << coords.y << "was already destroyed \n";
			break;
		default:
			break;
		}
	}
	else {
		switch (cell.value) {
		case CellValue::Empty:
			cell.value = CellValue::Miss;
			std::cout << "Miss x: " << coords.x << " y: " << coords.y << "\n";
			break;
		case CellValue::Miss:
			std::cout << "Was attacked x: " << coords.x << " y: " << coords.y << "\n";
			break;
		default:
			break;
		}
	}
	*/
	switch (cell.value)
	{
	case CellValue::Empty:
		cell.value = CellValue::Miss;
		std::cout << "Miss x: " << coords.x << " y: " << coords.y << "\n";
		break;
	case CellValue::Miss:
		std::cout << "Was attacked x: " << coords.x << " y: " << coords.y << "\n";
		break;
	case CellValue::ShipSegment: {
		switch (cell.shipSegment->status) {
		case SegmentStatus::INTACT:
			cell.shipSegment->status = SegmentStatus::DAMAGED;
			std::cout << "segment hit x: " << coords.x << " y: " << coords.y << "\n";
			break;

		case SegmentStatus::DAMAGED:
			cell.shipSegment->status = SegmentStatus::DESTROYED;
			std::cout << "segment destroyed x: " << coords.x << " y: " << coords.y << "\n";
			break;

		case SegmentStatus::DESTROYED:
			std::cout << "segment x: " << coords.x << " y: " << coords.y << "was already destroyed \n";
			break;
		default:
			break;
		}
	}
	default:
		break;
	}
}