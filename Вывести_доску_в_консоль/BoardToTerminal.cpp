#include "BoardToTerminal.hpp"
#include <iostream>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define _DEBUG

const int WidhtOfDesk = 8;
const int HightOfDesk = 8;
const int IncorrertCoordinats = 1;

MyDesk::MyDesk(bool *CNToHL, int *B)
{
    Board = B;
    BoolBoard = CNToHL;
    ArrOfSymbols = (MyChar *)calloc(5 * 5 * WidhtOfDesk * HightOfDesk, sizeof(MyChar));
}

MyDesk::~MyDesk()
{
    free(ArrOfSymbols);
}

void MyDesk::DeskPrint()
{
    for (int i = 0; i < 5 * HightOfDesk; i++)
    {
        for (int j = 0; j < 5 * WidhtOfDesk; j++)
        {
            printf("\033[%dm\033[%dm%c\033[0m", ArrOfSymbols[i * 5 * WidhtOfDesk + j].FontColor, //
                   ArrOfSymbols[i * 5 * WidhtOfDesk + j].BGColor, ArrOfSymbols[i * 5 * WidhtOfDesk + j].c);
            // printf("\033[%dm\033[%dm%c\033[0m", FontWhite, BGBlue, ' ');
        }
        printf("\n");
    }
}

void MyDesk::ToColorizeCell(int x, int y)
{
    MyChar *AdressOfThisCell = GetAdressOfCells(x, y);
    int ColorOfCells;
    if ((x + y) % 2 == 0)
        ColorOfCells = BGWhite;
    else
        ColorOfCells = BGBlack;

    AdressOfThisCell->BGColor = ColorOfCells;
    AdressOfThisCell->c = ' ';
    // Раскрашиваю клетку в белый или чёрный цвет

    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
        {
            (AdressOfThisCell + j + i * 5 * WidhtOfDesk)->BGColor = ColorOfCells;
            (AdressOfThisCell + j + i * 5 * WidhtOfDesk)->c = ' ';
        }
    // Добавляю выделение

    if (BoolBoard[(HightOfDesk - y) * HightOfDesk + (x - 1)])
    {
        for (int i = 0; i < 1; i++)
            for (int j = 0; j < 5; j++)
                (AdressOfThisCell + j + i * 5 * WidhtOfDesk)->BGColor = BGBlue;

        for (int i = 4; i < 5; i++)
            for (int j = 0; j < 5; j++)
                (AdressOfThisCell + j + i * 5 * WidhtOfDesk)->BGColor = BGBlue;

        for (int j = 0; j < 1; j++)
            for (int i = 0; i < 5; i++)
                (AdressOfThisCell + j + i * 5 * WidhtOfDesk)->BGColor = BGBlue;

        for (int j = 4; j < 5; j++)
            for (int i = 0; i < 5; i++)
                (AdressOfThisCell + j + i * 5 * WidhtOfDesk)->BGColor = BGBlue;
    }

    // Добавляю фигуру

    int i = 2;
    int j = 2;

    switch (Board[(HightOfDesk - y) * HightOfDesk + (x - 1)])
    {
    case empty:
        (AdressOfThisCell + j + i * 5 * WidhtOfDesk)->c = ' ';
        break;
    case pawn:
        (AdressOfThisCell + j + i * 5 * WidhtOfDesk)->c = 'p';
        break;
    case knight:
        (AdressOfThisCell + j + i * 5 * WidhtOfDesk)->c = 'k';
        break;
    case bishop:
        (AdressOfThisCell + j + i * 5 * WidhtOfDesk)->c = 'b';
        break;
    case rook:
        (AdressOfThisCell + j + i * 5 * WidhtOfDesk)->c = 'r';
        break;
    case queen:
        (AdressOfThisCell + j + i * 5 * WidhtOfDesk)->c = 'q';
        break;
    case king:
        (AdressOfThisCell + j + i * 5 * WidhtOfDesk)->c = 'K';
        break;

    default:
        (AdressOfThisCell + j + i * 5 * WidhtOfDesk)->c = '!';
        break;
    }

    if ((AdressOfThisCell + j + i * 5 * WidhtOfDesk)->c != ' ')
    {
        int AnotherColor;

        if ((x + y) % 2 != 0)
            AnotherColor = BGWhite;
        else
            AnotherColor = BGBlack;

        if ((x + y) % 2 != 0)
            (AdressOfThisCell + j + i * 5 * WidhtOfDesk)->FontColor = FontBlack;
        else
            (AdressOfThisCell + j + i * 5 * WidhtOfDesk)->FontColor = FontWhite;

        (AdressOfThisCell + j + i * 5 * WidhtOfDesk)->BGColor = AnotherColor;
    }
}

MyChar *MyDesk::GetAdressOfCells(int x, int y)
{
    int i = HightOfDesk - y;
    int j = x - 1;

    return (ArrOfSymbols + 5 * 5 * i * WidhtOfDesk + j * 5);
}

int ToPrintABoard(const int *Board, MassiveOfCells CellsNeedToHL)
{

#ifdef _DEBUG
    assert(Board);
    assert(CellsNeedToHL.arr);
#endif

#ifndef _DEBUG
    if (Board == NULL || CellsNeedToHL.arr == NULL)
        return -1;
#endif

    bool BoolBoard[HightOfDesk][WidhtOfDesk];
    MakeBoardOfHLsCeed(CellsNeedToHL, (bool *)BoolBoard);

    class MyDesk Desk((bool *) BoolBoard,(int *) Board);

    for (int i = 1; i <= 8; i++)
        for (int j = 1; j <= 8; j++)
        {
            Desk.ToColorizeCell(i, j);
        }
        
    Desk.DeskPrint();

    return 1;
}

int MakeBoardOfHLsCeed(MassiveOfCells CellsNeedToHL, bool *BoolBoard)
{

    errno = 0;

#ifdef _DEBUG
    assert(CellsNeedToHL.arr);
    assert(BoolBoard);
#endif

#ifndef _DEBUG
    if (BoolBoard == NULL || CellsNeedToHL.arr == NULL)
        return -1;
#endif

    for (int i = 0; i < HightOfDesk; i++)
        for (int j = 0; j < WidhtOfDesk; j++)
        {
            BoolBoard[i * HightOfDesk + j] = 0;
        }

    for (int i = 0; i < CellsNeedToHL.NumOfCells; i++)
    {
        int x, y;
        x = (CellsNeedToHL.arr + i)->x;
        y = (CellsNeedToHL.arr + i)->y;

#ifdef _DEBUG
        if ((x > WidhtOfDesk) || (x < 1) || (y > HightOfDesk) || (y < 1))
        {
            fprintf(stderr, "in function \'%s\':", __FUNCTION__);
            fprintf(stderr, "Incorrect coordinats \n");
            errno = IncorrertCoordinats;
            continue;
        }
#endif

#ifndef _DEBUG
        if ((x > WidhtOfDesk) || (x < 1) || (y > HightOfDesk) || (y < 1))
        {
            errno = IncorrertCoordinats;
            continue;
        }
#endif

        BoolBoard[(HightOfDesk - y) * HightOfDesk + x - 1] = 1;
    }

    if (errno != 0)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

CellCoordinats *CharPosToXYPOS(char *CharPos, CellCoordinats *XYPOS)
{

#ifdef _DEBUG
    assert(CharPos);
    assert(XYPOS);
#endif

#ifndef _DEBUG
    if (CharPos == NULL || XYPOS == NULL)
        return NULL;
#endif
    return NULL;
}