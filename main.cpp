#include "GameField.hpp"
#include "ShipManager.hpp"

int main()
{
    GameField gf(10, 10);
    ShipManager sh;
    gf.setAllShips(sh.getShips());
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            gf.attackCell(Coordinates{ i,j });
            sh.registerDamage(Coordinates{ i,j });
        }
    }
    sh.printShipsInfo();
    gf.drawField();
   
}
