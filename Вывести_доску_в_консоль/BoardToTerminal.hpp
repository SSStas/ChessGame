#pragma once 

enum TypeOfFigure
{
    empty = 0,
    pawn = 1,
    knight,
    bishop,
    rook,
    queen,
    king
};

enum NumOfColors
{
    BGBlack = 40,
    FontBlack = 30,
    BGWhite = 107,
    FontWhite = 97,
    BGBlue = 106
};

typedef struct CellCoordinats
{
    int x;
    int y;
} CellCoordinats;

typedef struct MassiveOfCells
{
    CellCoordinats *arr;
    int NumOfCells;
} MassiveOfCells;

typedef struct MyChar
{
    char c;
    int FontColor;
    int BGColor;
} MyChar;

class MyDesk{

private:    
    bool *BoolBoard;
    int *Board;
public:
    MyChar *ArrOfSymbols;
    void ToColorizeCell(int x, int y);
    MyChar *GetAdressOfCells(int x, int y);
    void DeskPrint();
    MyDesk(bool *BoolBoard, int *B);
    ~MyDesk();
};

//Принимает массив интов 8x8 из состояний клетки (см. enum TypeOfFigure) и 
//Массив выделенных клеток (см. MassiveCells) и выводит в консоль доску
int ToPrintABoard(const int *Board, MassiveOfCells CellsNeedToHL);

//Переводит одномерный массив клеток в доску состояний 
int MakeBoardOfHLsCeed(MassiveOfCells CellsNeedToHL, bool *BoolBoard);

