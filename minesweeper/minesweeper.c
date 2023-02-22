#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <assert.h>
#include "Input.h"
#include <time.h>

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
    COLOR_BLACK,
    ANSI_COLOR_RED
};

void initBombGrid(int grid[ARRAY_SIZE]);
void initGameGrid(char grid[ARRAY_SIZE]);
void displayGameGrid(char grid[ARRAY_SIZE]);
void completeGrid(int bombGrid[ARRAY_SIZE], char gameGrid[ARRAY_SIZE]);
int digAt(int bombGrid[ARRAY_SIZE], char gameGrid[ARRAY_SIZE], int x, int y);
Input playerInput();
int countBombs(int bombGrid[ARRAY_SIZE]);
int countFound(char gameGrid[ARRAY_SIZE]);

// MAIN //

/// <summary>
/// Program's entry point
/// </summary>
/// <returns>int</returns>
int main()
{
    srand(time(NULL));

    int playing = 1;
    int won = 0;
    int bombGrid[ARRAY_SIZE];
    initBombGrid(bombGrid);
    char gameGrid[ARRAY_SIZE];
    initGameGrid(gameGrid);

    int bombCount = countBombs(bombGrid);
    // digAt(bombGrid, gameGrid, 0, 4);
    
    while (playing) 
    {
        if (system("CLS")) system("clear");
        displayGameGrid(gameGrid);
        printf("\nBOMBS: %d\nTO DIG: %d", bombCount, countFound(gameGrid));
        Input input = playerInput();
        
        if (input.flag) {
            if (gameGrid[input.x + input.y * 20] == charset[0]) {
                gameGrid[input.x + input.y * 20] = charset[12];
            }
            else if (gameGrid[input.x + input.y * 20] == charset[12]) {
                gameGrid[input.x + input.y * 20] = charset[0];
            }
        }
        else {
            playing = !digAt(bombGrid, gameGrid, input.x, input.y);
        }

        if (bombCount == countFound(gameGrid))
        {
            won = 1;
            playing = 0;
        }
    }

    if (system("CLS")) system("clear");
    completeGrid(bombGrid, gameGrid);
    displayGameGrid(gameGrid);

    if (won) 
    {
        printf("\nYOU WON!");
    }
    else 
    {
        printf("\nYou Lost Lol.... Loser..... Lol?...........");
    }
}

// FUNCTIONS //

// Misc. functions

int countBombs(int bombGrid[ARRAY_SIZE]) 
{
    int count = 0;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        count += bombGrid[i];
    }
    return count;
}

int countFound(char gameGrid[ARRAY_SIZE])
{
    int count = 0;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        count += gameGrid[i] == charset[0];
    }
    return count;
}

/// <summary>
/// counts bombs around a coordinate
/// </summary>
/// <param name="bombGrid"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns>bomb count (int)</returns>
int countNeighbours(int bombGrid[ARRAY_SIZE], int x, int y) {
    int count = 0;
    for (int bx = x-1; bx <= x+1; bx++) 
    {
        for (int by = y-1; by <= y+1; by++) 
        {
            if (bx >= 0 && bx < SIZE && by >= 0 && by < SIZE) {
                count += bombGrid[bx + by * 20];
            }
        }
    }
    return count;
}

/// <summary>
/// reveals everything in the grid
/// </summary>
/// <param name="bombGrid"></param>
/// <param name="gameGrid"></param>
void completeGrid(int bombGrid[ARRAY_SIZE], char gameGrid[ARRAY_SIZE])
{
    for (int x = 0; x < SIZE; x++)
    {
        for (int y = 0; y < SIZE; y++)
        {
            int index = 1;
            if (bombGrid[x + y * 20] == 1) {
                index = 11;
            }
            else {
                index += countNeighbours(bombGrid, x, y);
            }
            gameGrid[x + y * 20] = charset[index];
        }
    }
}

/// <summary>
/// digs at a coordinate
/// </summary>
/// <param name="bombGrid"></param>
/// <param name="gameGrid"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns>if there's a bomb, it returns 1, else 0</returns>
int digAt(int bombGrid[ARRAY_SIZE], char gameGrid[ARRAY_SIZE], int x, int y)
{
    if (bombGrid[x + y * 20] == 1) {
        gameGrid[x + y * 20] = charset[11];
        return 1;
    }
    else {
        int index = 1;
        int count = countNeighbours(bombGrid, x, y);
        index += count;
        gameGrid[x + y * 20] = charset[index];

        if (count == 0) {
            for (int bx = x - 1; bx <= x + 1; bx++)
            {
                if (bx >= 0 && bx < SIZE)
                {
                    for (int by = y - 1; by <= y + 1; by++)
                    {
                        if (by >= 0 && by < SIZE && (gameGrid[bx + by * 20] == charset[0] || gameGrid[bx + by * 20] == charset[12])) {
                            if (((bx == x - 1 && by == y - 1) || (bx == x + 1 && by == y - 1) || (bx == x + 1 && by == y + 1) || (bx == x - 1 && by == y + 1)))
                            {
                                if (countNeighbours(bombGrid, bx, by) > 0) 
                                {
                                    digAt(bombGrid, gameGrid, bx, by);
                                }
                            }
                            else
                            {
                                digAt(bombGrid, gameGrid, bx, by);
                            }
                        }
                    }
                }
            }
        }
        return 0;
    }
}

// Display functions

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
void displayGameGrid(char grid[ARRAY_SIZE])
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
            int charInd = findIndex(grid[x + y * 20]);
            char* colour = "";
            if (charInd >= 0) {
                colour = colours[charInd];
            }
            printf("%s%c  ", colour, grid[x + y * 20]);
        }
        printf(ANSI_COLOR_RESET);
    }
}

// Init. functions

/// <summary>
/// initialises the bomb grid (places bombs randomly)
/// </summary>
/// <param name="grid"></param>
void initBombGrid(int grid[ARRAY_SIZE])
{
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        grid[i] = rand() % 10 == 1;
    }
}

/// <summary>
/// initialises the game grid
/// </summary>
/// <param name="grid"></param>
void initGameGrid(char grid[ARRAY_SIZE])
{
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        grid[i] = charset[0];
    }
}

// Input

/// <summary>
/// Manual flush!
/// </summary>
void Flush()
{
    while (getchar() != '\n');
}

/// <summary>
/// gets input from user
/// </summary>
Input playerInput()
{
    int playerInputX = -1;
    int playerInputY = -1;
    Input Return;
    Return.flag = 0;

    printf("\nDo you want to place a flag? (y/N)> ");


    //(void)scanf_s("%d%d", &playerInputX, &playerInputY);
    Return.flag = getchar() == 'y';

    char* msg = Return.flag ? "place a flag" : "dig";
    char* valid = "";

    while (!(playerInputX >= 0 && playerInputX < SIZE && playerInputY >= 0 && playerInputY < SIZE))
    {
        printf("\nSelect a %splace to %s (X Y)> ", valid, msg);

        (void)scanf_s("%d%d", &playerInputX, &playerInputY);
        Flush();
        valid = "VALID ";
    }

    Return.x = playerInputX;
    Return.y = playerInputY;
    
    return Return;
}