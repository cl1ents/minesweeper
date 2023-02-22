#include <stdio.h>
#include <stdlib.h>
#include "GameGrid.h"

// DEFINITIONS //
#define SIZE 20
#define ARRAY_SIZE SIZE*SIZE

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define COLOR_BLACK  "\033[0;30m"
#define COLOR_WHITE "\033[0;37m"

static const char charset[] = ". 123456789*F";
static const char* colours[] = {
    COLOR_WHITE,
    ANSI_COLOR_RESET,
    ANSI_COLOR_BLUE,
    ANSI_COLOR_GREEN,
    ANSI_COLOR_RED,
    ANSI_COLOR_MAGENTA,
    ANSI_COLOR_YELLOW,
    ANSI_COLOR_CYAN,
    COLOR_BLACK,
    COLOR_WHITE,
    ANSI_COLOR_RESET,
    COLOR_WHITE,
    ANSI_COLOR_RED
};

// FUNCTIONS //

void initGrid(GameGrid* grid);
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
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        grid->bombGrid[i] = rand() % 10 == 1;
    }

    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        grid->displayGrid[i] = charset[0];
    }
}

int countNeighbours(GameGrid* grid, int x, int y) {
    int count = 0;
    for (int bx = x - 1; bx <= x + 1; bx++)
    {
        for (int by = y - 1; by <= y + 1; by++)
        {
            if (bx >= 0 && bx < SIZE && by >= 0 && by < SIZE) {
                count += grid->bombGrid[bx + by * 20];
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
    for (int x = 0; x < SIZE; x++)
    {
        for (int y = 0; y < SIZE; y++)
        {
            int index = 1;
            if (grid->bombGrid[x + y * 20] == 1) {
                index = 11;
            }
            else {
                index += countNeighbours(grid, x, y);
            }
            grid->displayGrid[x + y * 20] = charset[index];
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
    if (grid->displayGrid[x + y * 20] == charset[0]) {
        grid->displayGrid[x + y * 20] = charset[12];
    }
    else if (grid->displayGrid[x + y * 20] == charset[12]) {
        grid->displayGrid[x + y * 20] = charset[0];
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
    if (grid->bombGrid[x + y * 20] == 1) {
        grid->displayGrid[x + y * 20] = charset[11];
        return 1;
    }
    else {
        int index = 1;
        int count = countNeighbours(grid, x, y);
        index += count;
        grid->displayGrid[x + y * 20] = charset[index];

        if (count == 0) {
            for (int bx = x - 1; bx <= x + 1; bx++)
            {
                if (bx >= 0 && bx < SIZE)
                {
                    for (int by = y - 1; by <= y + 1; by++)
                    {
                        if (by >= 0 && by < SIZE && (grid->displayGrid[bx + by * 20] == charset[0] || grid->displayGrid[bx + by * 20] == charset[12])) {
                            if (((bx == x - 1 && by == y - 1) || (bx == x + 1 && by == y - 1) || (bx == x + 1 && by == y + 1) || (bx == x - 1 && by == y + 1)))
                            {
                                if (countNeighbours(grid, bx, by) > 0)
                                {
                                    digAt(grid, bx, by);
                                }
                            }
                            else
                            {
                                digAt(grid, bx, by);
                            }
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
    for (int i = 0; i < ARRAY_SIZE; i++) {
        count += grid->bombGrid[i];
    }
    return count;
}

int countFound(GameGrid* grid)
{
    int count = 0;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        count += grid->displayGrid[i] == charset[0];
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
    for (int i = 0; i < SIZE; i++)
    {
        printf("%02d ", i);
    }
    printf("\n X +");
    for (int i = 0; i < SIZE; i++)
    {
        printf("---");
    }

    for (int x = 0; x < SIZE; x++)
    {
        printf("\n%02d | ", x);
        for (int y = 0; y < SIZE; y++)
        {
            int charInd = findIndex(grid->displayGrid[x + y * 20]);
            char* colour = "";
            if (charInd >= 0) {
                colour = colours[charInd];
            }
            printf("%s%c  ", colour, grid->displayGrid[x + y * 20]);
        }
        printf(ANSI_COLOR_RESET);
    }
}
