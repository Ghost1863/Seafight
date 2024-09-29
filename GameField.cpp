#include "GameField.hpp"
#include <random>
#include <chrono>

void setColor(int foreground, int background = 40, int attributes = 0) {
	std::cout << "\033[" << attributes << ";" << foreground << ";" << background << "m";
}
void resetColor() {
	std::cout << "\033[0m";
}

GameField::GameField(int g_width, int g_height) {
	width = g_width;
	height = g_height;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			field.push_back(FieldCell{ Coordinates{x,y}, CellStatus::DISCLOSED, CellValue::Empty });
		}
	}
}

GameField::GameField(const GameField& other)
	: height(other.height), width(other.width), field(other.field) {}


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
			setColor(33);
			std::cout << "~";
		}
		else {
			if (cell.status == CellStatus::DISCLOSED) 
				switch (cell.value)
				{
				case CellValue::ShipPart:
					setColor(32);//GREEN
					std::cout << "O";
					break;
				case CellValue::Empty:
					setColor(34);//BLUE
					std::cout << "#";
					break;
				case CellValue::Miss:
					setColor(31);//RED
					std::cout << "x";
					break;
				case CellValue::Hit:
					setColor(33);//YELLOW
					std::cout << "O";
					break;
				case CellValue::Destroyed:
					setColor(31);//RED
					std::cout << "O";
					break;
				default:
					break;
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
					if (field[x + i + (y + j) * width].value == CellValue::ShipPart) {
						return false;
					}
				}
			}
		}
	}
	else return false;
	
	return true;
}

bool GameField::setShip(Coordinates coords,Ship* ship, bool isVertical) {
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
				return false;
		}
		field[coords.x + coords.y * width].value = CellValue::ShipPart;
		ship->addSegment(new ShipSegment (2, { coords.x,coords.y}, SegmentStatus::INTACT));
	}
	else {
		return false;
	}

	ship->setIsVertical(isVertical);
	ship->setCoords(coords);
	if (isVertical) {
		//start point is up
		for (size_t i = 1; i < ship->getLength(); i++)
		{
			field[coords.x + (coords.y+i)*width].value = CellValue::ShipPart;
			ship->addSegment( new ShipSegment(2, { coords.x,int(coords.y+i) }, SegmentStatus::INTACT));
		}
	}
	else {
		//start point is left
		for (size_t i = 1; i <  ship->getLength(); i++)
		{
			field[coords.x + coords.y * width + i].value = CellValue::ShipPart;
			ship->addSegment(new ShipSegment(2, { int(coords.x+i),coords.y }, SegmentStatus::INTACT));
		}
	}
}

void GameField::setAllShips(std::vector<Ship*> ships) {

	std::mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());
	for (auto& ship : ships) {
		while (!ship->getIsPlaced()) {

			std::mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());
			std::uniform_int_distribution<int> distr(0, 9);
			std::uniform_int_distribution<int> distr_bool(0, 1);

			int x = distr(gen);
			int y = distr(gen);
			bool random_bool = static_cast<bool>(distr_bool(gen));

			if (this->setShip(Coordinates{ x, y }, ship, random_bool)) {
				ship->setIsPlaced(true);
			}
			
		}
	}
}

void GameField::attackCell(Coordinates coords) {
	if (!checkCurrentCoord(coords.x, coords.y)) {
		std::cout << "Coordinates" << coords.x << " " << coords.y << "out of range\n";
		return;
	}
	FieldCell& cell = field[coords.x + coords.y * width];
	switch (cell.value) {
	case CellValue::ShipPart:
		cell.status = CellStatus::DISCLOSED;
		cell.value = CellValue::Hit;
		break;
	case CellValue::Hit:
		cell.value = CellValue::Destroyed;
		break;
	case CellValue::Empty:
		cell.status = CellStatus::DISCLOSED;
		cell.value = CellValue::Miss;
		std::cout << "Miss x: "<<coords.x<<" y: "<<coords.y<<"\n";
		break;
	default:
		std::cout << "x: "<<coords.x<<" y: "<<coords.y<< " Was attacked!\n";
		break;
	}
}