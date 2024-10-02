#pragma once
#include "Ship.hpp"
#include "Structures.hpp"
#include <vector>
#include <iostream>


class GameField {

private:	
	int width;
	int height;
	std::vector<FieldCell> field;

public:
	GameField(int width,int height);
	GameField(const GameField& other);
	GameField& operator=(const GameField& other);
	GameField(GameField&& other);
	GameField& operator=(GameField&& other);

	void drawField();
	bool checkCurrentCoord(int x,int y);
	bool checkCoordsAround(int x, int y);
	void setShip(Coordinates coords, Ship* ship, bool isVertical);
	void setAllShips(std::vector<Ship*> ships);
	void attackCell(Coordinates coords);
};