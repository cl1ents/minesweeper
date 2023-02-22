#define SIZE 20
#define ARRAY_SIZE SIZE*SIZE

typedef struct GameGrid {
	int bombGrid[ARRAY_SIZE];
	char displayGrid[ARRAY_SIZE];
} GameGrid;

void initGrid(GameGrid* grid);
void completeGrid(GameGrid* grid);
int digAt(GameGrid* grid, int x, int y);
int countBombs(GameGrid* grid);
int countFound(GameGrid* grid);
int countNeighbours(GameGrid* grid, int x, int y);

void displayGameGrid(GameGrid* grid);