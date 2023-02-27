#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <assert.h>
#include <time.h>

#include "Input.h"
#include "Array.h"
#include "CArray.h"
#include "GameGrid.h"
#include "CONSTS.h"

#define clear if (system("CLS")) system("clear")

Input playerInput(GameGrid * grid);
void game();
int difficulties[] = {
    9, 8, 5, 4
};

// MAIN //


void displayArray(Array* arr) {
    for (int i = 0; i < arr->length; i++)
    {
        printf("%d\n", arr->array[i]);
    }
    printf("len: %d; size: %d\n\n", arr->length, arr->size);
}

/*

int main()
{
    Array lol = newArray();
    displayArray(&lol);

    insertInto(&lol, 1);
    insertInto(&lol, 2);
    insertInto(&lol, 3);
    insertInto(&lol, 4);
    displayArray(&lol);

    insertInto(&lol, 1337);
    insertInto(&lol, 5);
    removeFrom(&lol, 6);
    displayArray(&lol);

    removeFrom(&lol, 7);
    insertInto(&lol, 8);
    insertInto(&lol, 9);

    displayArray(&lol);
    insertIntoIndex(&lol, 10, 0);
    insertIntoIndex(&lol, 11, 2);
    displayArray(&lol);
    free(lol.array);

    Array lol = newArray();

    insertInto(&lol, 1337);
    insertInto(&lol, 5);
    removeFrom(&lol, 1);
    displayArray(&lol);

    displayArray(&lol);
    insertIntoIndex(&lol, 10, 0);
    insertIntoIndex(&lol, 11, 2);
    displayArray(&lol);
    free(lol.array);
}

*/

/// <summary>
/// Program's entry point
/// </summary>
/// <returns>int</returns>
int main()
{
    while (1) {
        game();

        char c = ' ';
        printf("\n\n");
        while (c != 'y' && c != 'n')
        {
            printf("Do you want to restart? (y/n)> ");

            char str[20];
            fgets(str, 20, stdin);
            (void)sscanf_s(str, "%c", &c);
        }

        if (c == 'n') {
            break;
        }
    }
}

/// <summary>
/// Game initializing & Play
/// </summary>
/// <returns></returns>
void game()
{
    srand(time(NULL));

    printf("MINESWEEPER\n");
    printf("\n\nFormat: 'Difficulty Size',\ndifficulty can be a number from 0 to 3, from easiest to hardest.\nSize can be anything from 10 to 50.\nExample: '1 20'");
    char* valid = "Please type the desired difficulty and size";

    int difficulty = -1;
    int size = -1;

    while (!(difficulty >= 0 && difficulty <= 3 && size >= 10 && size <= 50))
    {
        printf("\n%s> ", valid);

        char str[20];
        fgets(str, 20, stdin);

        (void)sscanf_s(str, "%d%d", &difficulty, &size);
        valid = "Please type a VALID difficulty and size";
    }


    int playing = 1;
    int moveCount = 0;
    int won = 0;
    GameGrid game;

    Array bombGrid;
    game.bombGrid = &bombGrid;
    CArray displayGrid;
    game.displayGrid = &displayGrid;

    game.gridSize = size;
    game.arraySize = game.gridSize * game.gridSize;

    cInitArraySize(game.displayGrid, game.arraySize);
    initArraySize(game.bombGrid, game.arraySize);

    initGrid(&game);

    int bombCount = game.arraySize / difficulties[difficulty];
    
    while (playing) 
    {
        clear;
        displayGameGrid(&game);
        printf("\nBOMBS: %d\nTO DIG: %d", bombCount, countFound(&game));
        Input input = playerInput(&game);
        
        if (input.flag) {
            placeFlag(&game, input.x, input.y);
        }
        else {
            if (!moveCount)
            {
                placeBombs(&game, &input, difficulties[difficulty]);
                bombCount = countBombs(&game);
            }

            if (game.displayGrid->array[input.x + input.y * 20] != charset[12])
                playing = !digAt(&game, input.x, input.y);
                moveCount++;
        }

        if (bombCount == countFound(&game))
        {
            won = 1;
            playing = 0;
        }
    }

    if (system("CLS")) system("clear");
    completeGrid(&game);
    displayGameGrid(&game);

    if (won) 
    {
        printf("\nYOU WON!");
    }
    else 
    {
        printf("\nYou Lost Lol.... Loser..... Lol?...........");
    }

    free(displayGrid.array);
    free(bombGrid.array);
}

// FUNCTIONS //

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

Input playerInput(GameGrid *game) {
    int playerInputX = -1;
    int playerInputY = -1;
    Input Return;
    Return.flag = 0;

    printf("\n\nFormat: 'X Y', add an 'f' at the start if you want to place a flag (Example: 'f 10 10', '5 8')");
    char* valid = "Where do you want to play?";
    while (!(playerInputX >= 0 && playerInputX < game->gridSize && playerInputY >= 0 && playerInputY < game->gridSize))
    {
        printf("\n%s> ", valid);

        char str[20];
        fgets(str, 20, stdin);
        
        Return.flag = tolower(str[0]) == 'f';
        str[0] = Return.flag ? ' ' : str[0];
        (void)sscanf_s(str, "%d%d", &playerInputX, &playerInputY);
        valid = "Select a VALID place to play";
    }

    Return.x = playerInputX;
    Return.y = playerInputY;

    return Return;
}
