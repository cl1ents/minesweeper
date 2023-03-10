#include <stdio.h>
#include <stdlib.h>
#include "GameGrid.h"
#include "Array.h"
#include "CArray.h"
#include "CONSTS.h"
#include "Input.h"

// VARIBLES //

const int diagonalCoords[4][2] = {
        {3, 3},
        {-3, 3},
        {3, -3},
        {-3, -3},
};


// FUNCTIONS //

void initGrid(GameGrid* grid);
void placeBombs(GameGrid* grid, const Input* firstInput);
void completeGrid(GameGrid* grid);

void placeFlag(GameGrid* grid, int x, int y);
int digAt(GameGrid* grid, int x, int y);
int countBombs(GameGrid* grid);
int countFound(GameGrid* grid);
int countNeighbours(GameGrid* grid, int x, int y);

void displayGameGrid(GameGrid* grid);

// Game logic

/// <summary>
/// Initialises grid
/// </summary>
/// <param name="grid"></param>
void initGrid(GameGrid* grid) 
{

    // static Array bombGrid = newArraySize(grid->arraySize);
    // grid->bombGrid;
    for (int i = 0; i < grid->arraySize; i++)
    {
        insertInto(grid->bombGrid, 0);
    }

    for (int i = 0; i < grid->arraySize; i++)
    {
        cInsertInto(grid->displayGrid, charset[0]);
    }
}

/// <summary>
/// absolute
/// </summary>
/// <param name="i"></param>
/// <returns>absolute i</returns>
int abs(int i)
{
    return i > 0 ? i : -i;
}

/// <summary>
/// checks whether or not the diagonals of x and y are bombs
/// </summary>
/// <param name="grid"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns>0 or 1</returns>
int checkDiagonal(GameGrid* grid, int x, int y)
{
    for (int i = 0; i < 4; i++)
    {
        int bx = x + diagonalCoords[i][0];
        int by = y + diagonalCoords[i][1];
        if (bx >= 0 && bx < grid->gridSize && by >= 0 && by < grid->gridSize && grid->bombGrid->array[bx + by * grid->gridSize] == 1)
        {
            return 0;
        }
    }

    return 1;
}

/// <summary>
/// place bombs according to the first input
/// </summary>
/// <param name="grid"></param>
/// <param name="firstInput"></param>
void placeBombs(GameGrid* grid, const Input* firstInput, int difficulty)
{
    int bombCount = grid->arraySize / difficulty;

    Array freeSpace = newArraySize(grid->arraySize);
    for (int i = 0; i < grid->arraySize; i++)
    {
        int x = i % grid->gridSize;
        int y = i / grid->gridSize;
        if (abs(x-firstInput->x) > 1 || abs(y - firstInput->y) > 1)
            insertInto(&freeSpace, i);
    }

    while (bombCount > 0 && freeSpace.length > 0)
    {
        int index = removeFrom(&freeSpace, rand() % freeSpace.length);
        int x = index % grid->gridSize;
        int y = index / grid->gridSize;
        if (grid->bombGrid->array[x + y * grid->gridSize] == 0 && 
            countNeighbours(grid, x, y) <= 7 && 
            checkDiagonal(grid, x, y))
        {
            grid->bombGrid->array[x + y * grid->gridSize] = 1;
            bombCount--;
        }
    }
    // free(freeSpace.array);
}

int countNeighbours(GameGrid* grid, int x, int y) {
    int count = 0;
    for (int bx = x - 1; bx <= x + 1; bx++)
    {
        for (int by = y - 1; by <= y + 1; by++)
        {
            if (bx >= 0 && bx < grid->gridSize && by >= 0 && by < grid->gridSize) {
                count += grid->bombGrid->array[bx + by * grid->gridSize];
            }
        }
    }
    return count;
}

/// <summary>
/// reveals everything in the grid
/// </summary>
/// <param name="grid"></param>
void completeGrid(GameGrid* grid) 
{
    for (int x = 0; x < grid->gridSize; x++)
    {
        for (int y = 0; y < grid->gridSize; y++)
        {
            int index = 1;
            if (grid->bombGrid->array[x + y * grid->gridSize] == 1) {
                index = 11;
            }
            else {
                index += countNeighbours(grid, x, y);
            }
            grid->displayGrid->array[x + y * grid->gridSize] = charset[index];
        }
    }
}

/// <summary>
/// Place flags at X Y
/// </summary>
/// <param name="grid"></param>
/// <param name="x"></param>
/// <param name="y"></param>
void placeFlag(GameGrid* grid, int x, int y) {
    if (grid->displayGrid->array[x + y * grid->gridSize] == charset[0]) {
        grid->displayGrid->array[x + y * grid->gridSize] = charset[12];
    }
    else if (grid->displayGrid->array[x + y * grid->gridSize] == charset[12]) {
        grid->displayGrid->array[x + y * grid->gridSize] = charset[0];
    }
}

/// <summary>
/// digs at a coordinate
/// </summary>
/// <param name="grid"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns>if there's a bomb, it returns 1, else 0</returns>
int digAt(GameGrid* grid, int x, int y)
{
    if (grid->bombGrid->array[x + y * grid->gridSize] == 1) {
        grid->displayGrid->array[x + y * grid->gridSize] = charset[11];
        return 1;
    }
    else {
        int index = 1;
        int count = countNeighbours(grid, x, y);
        index += count;
        grid->displayGrid->array[x + y * grid->gridSize] = charset[index];

        if (count == 0) {
            for (int bx = x - 1; bx <= x + 1; bx++)
            {
                if (bx >= 0 && bx < grid->gridSize)
                {
                    for (int by = y - 1; by <= y + 1; by++)
                    {
                        if (by >= 0 && by < grid->gridSize && (grid->displayGrid->array[bx + by * grid->gridSize] == charset[0] || grid->displayGrid->array[bx + by * grid->gridSize] == charset[12])) {
                            /*if (((bx == x - 1 && by == y - 1) || (bx == x + 1 && by == y - 1) || (bx == x + 1 && by == y + 1) || (bx == x - 1 && by == y + 1)))
                            {
                                if (countNeighbours(grid, bx, by) > 0)
                                {
                                    digAt(grid, bx, by);
                                }
                            }
                            else*/
                            //{
                                digAt(grid, bx, by);
                            //}
                        }
                    }
                }
            }
        }
        return 0;
    }
}

int countBombs(GameGrid* grid)
{
    int count = 0;
    for (int i = 0; i < grid->arraySize; i++) {
        count += grid->bombGrid->array[i];
    }
    return count;
}

int countFound(GameGrid* grid)
{
    int count = 0;
    for (int i = 0; i < grid->arraySize; i++) {
        count += grid->displayGrid->array[i] == charset[0];
    }
    return count;
}

// Display Functions

int findIndex(char character) {
    int index = -1;

    for (int i = 0; i < sizeof charset; i++) {
        if (charset[i] == character) {
            index = i;
            break;
        }
    }

    return index;
}


/// <summary>
/// displays the grid
/// </summary>
/// <param name="grid"></param>
void displayGameGrid(GameGrid* grid)
{
    printf("   Y ");
    for (int i = 0; i < grid->gridSize; i++)
    {
        printf("%02d ", i);
    }
    printf("\n X +");
    for (int i = 0; i < grid->gridSize; i++)
    {
        printf("---");
    }

    for (int x = 0; x < grid->gridSize; x++)
    {
        printf("\n%02d | ", x);
        for (int y = 0; y < grid->gridSize; y++)
        {
            int charInd = findIndex(grid->displayGrid->array[x + y * grid->gridSize]);
            char* colour = "";
            if (charInd >= 0) {
                colour = colours[charInd];
            }
            printf("%s%c  ", colour, grid->displayGrid->array[x + y * grid->gridSize]);
        }
        printf(ANSI_COLOR_RESET);
    }
}
