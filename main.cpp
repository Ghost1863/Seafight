#include "GameField.hpp"
#include "ShipManager.hpp"

int main()
{
    GameField gf(10, 10);
    ShipManager sm;
    sm.createDefaultShips();
    gf.setAllShips(sm.getShips());

    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 5; j++){
            gf.attackCell(Coordinates{ i,j });
        }
    }
    sm.printShipsInfo();
    gf.drawField();
}
