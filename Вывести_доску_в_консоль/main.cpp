#include "BoardToTerminal.hpp"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n;
    scanf("%d", &n);

    MassiveOfCells CellsNeedToHL;
    CellsNeedToHL.arr = (CellCoordinats*) calloc(n, sizeof(CellCoordinats));
    CellsNeedToHL.NumOfCells = n;

    for (int i = 0; i < n; i++)
    {
        scanf("%d %d", &CellsNeedToHL.arr[i].x,  &CellsNeedToHL.arr[i].y);
    }

    int Board[8][8];
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            Board[i][j] = 0;
        }

    ToPrintABoard((int *) Board, CellsNeedToHL);

    free(CellsNeedToHL.arr);
}