#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// DEFINITIONS //
#define size 20
#define arraySize size*size

static const char charset[] = ". 123456789XF";

void initBombGrid(int grid[arraySize]);
void initGameGrid(char grid[arraySize]);
void displayGameGrid(char grid[arraySize]);
void completeGrid(int bombGrid[arraySize], char gameGrid[arraySize]);
int digAt(int bombGrid[arraySize], char gameGrid[arraySize], int x, int y);
void playerInput();

// MAIN //
int main()
{
    int bombGrid[arraySize];
    initBombGrid(bombGrid);
    char gameGrid[arraySize];
    initGameGrid(gameGrid);

    completeGrid(bombGrid, gameGrid);
    
    displayGameGrid(gameGrid);
    playerInput();
}

// FUNCTIONS //

// Misc. functions
int countNeighbours(int bombGrid[arraySize], int x, int y) {
    int count = 0;
    for (int bx = x-1; bx <= x+1; bx++) 
    {
        for (int by = y-1; by <= y+1; by++) 
        {
            if (bx >= 0 && bx < size && by >= 0 && by < size) {
                count += bombGrid[bx + by * 20];
            }
        }
    }
    return count;
}

void completeGrid(int bombGrid[arraySize], char gameGrid[arraySize])
{
    for (int x = 0; x < size; x++)
    {
        for (int y = 0; y < size; y++)
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

int digAt(int bombGrid[arraySize], char gameGrid[arraySize], int x, int y)
{
    if (bombGrid[x + y * 20]) {
        return 1;
    }
    else {
        int index = 1;
        int count = countNeighbours(bombGrid, x, y);
        index += count;
        gameGrid[x + y * 20] = charset[index];

        if (count > 0) {
            return 0;
        }
        else {
            for (int bx = x - 1; bx <= x + 1; bx++)
            {
                for (int by = y - 1; by <= y + 1; by++)
                {
                    if (bx != x && by != y) {
                        digAt(bombGrid, gameGrid, bx, by);
                    }
                }
            }
        }
    }
}

// Display functions
void displayGameGrid(char grid[arraySize])
{
    printf("     ");
    for (int i = 0; i < size; i++)
    {
        printf("%02d ", i);
    }

    for (int x = 0; x < size; x++)
    {
        printf("\n%02d | ", x);
        for (int y = 0; y < size; y++)
        {
            printf("%c  ", grid[x + y * 20]);
        }
    }
}

// Init. functions
void initBombGrid(int grid[arraySize])
{
    for (int i = 0; i < arraySize; i++)
    {
        grid[i] = rand() % 10 == 1;
    }
}

void initGameGrid(char grid[arraySize])
{
    for (int i = 0; i < arraySize; i++)
    {
        grid[i] = charset[0];
    }
}


// Input
void playerInput()
{
    char playerCoordinate[10];
    printf("\nSelect coordinate where to dig :\n");
    scanf_s("%s", playerCoordinate);
    fgetc(stdin);
    printf("%s lol", playerCoordinate);
}