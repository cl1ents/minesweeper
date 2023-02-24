#define SIZE 20
#define ARRAY_SIZE SIZE*SIZE

typedef struct Input Input;
typedef struct Array Array;
typedef struct CArray CArray;

typedef struct GameGrid {
	Array * bombGrid;
	CArray * displayGrid;
	int gridSize;
	int arraySize;
} GameGrid;

void initGrid(GameGrid* grid);
void completeGrid(GameGrid* grid);
void placeBombs(GameGrid* grid, const Input* firstInput, int difficulty);

void placeFlag(GameGrid* grid, int x, int y);
int digAt(GameGrid* grid, int x, int y);
int countBombs(GameGrid* grid);
int countFound(GameGrid* grid);
int countNeighbours(GameGrid* grid, int x, int y);

void displayGameGrid(GameGrid* grid);