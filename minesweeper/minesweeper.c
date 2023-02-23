#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <assert.h>
#include <time.h>

#include "Input.h"
#include "Array.h"
#include "GameGrid.h"
#include "CONSTS.h"

Input playerInput();

// MAIN //

/*
void displayArray(Array* arr) {
    for (int i = 0; i < arr->length; i++)
    {
        printf("%d\n", arr->array[i]);
    }
    printf("len: %d; size: %d\n\n", arr->length, arr->size);
}


int main()
{
    Array lol = newArray();
    displayArray(&lol);

    insertInto(&lol, 1);
    insertInto(&lol, 2);
    insertInto(&lol, 3);
    insertInto(&lol, 4);
    displayArray(&lol);

    //push(&lol, 1337);
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
}
*/

/// <summary>
/// Program's entry point
/// </summary>
/// <returns>int</returns>
int main()
{
    srand(time(NULL));

    int playing = 1;
    int won = 0;
    GameGrid game;
    initGrid(&game);

    int bombCount = countBombs(&game);
    
    while (playing) 
    {
        if (system("CLS")) system("clear");
        displayGameGrid(&game);
        printf("\nBOMBS: %d\nTO DIG: %d", bombCount, countFound(&game));
        Input input = playerInput();
        
        if (input.flag) {
            placeFlag(&game, input.x, input.y);
        }
        else {
            if (game.displayGrid[input.x + input.y * 20] != charset[12])
                 playing = !digAt(&game, input.x, input.y);
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

Input playerInput() {
    int playerInputX = -1;
    int playerInputY = -1;
    Input Return;
    Return.flag = 0;

    printf("\n\nFormat: 'X Y', add an 'f' at the start if you want to place a flag (Example: 'f 10 10', '5 8')");
    char* valid = "Where do you want to play?";
    while (!(playerInputX >= 0 && playerInputX < SIZE && playerInputY >= 0 && playerInputY < SIZE))
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
