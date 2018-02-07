#include "game.h"
#include "display.h"
#include "general.h"
#include "setup.h"

gameLoc tetrominos[7][4][4] = {
  // I
  {{{1, 0}, {1, 1}, {1, 2}, {1, 3}},
   {{0, 2}, {1, 2}, {2, 2}, {3, 2}},
   {{3, 0}, {3, 1}, {3, 2}, {3, 3}},
   {{0, 1}, {1, 1}, {2, 1}, {3, 1}}},
  // J
  {{{0, 0}, {1, 0}, {1, 1}, {1, 2}},
   {{0, 1}, {0, 2}, {1, 1}, {2, 1}},
   {{1, 0}, {1, 1}, {1, 2}, {2, 2}},
   {{0, 1}, {1, 1}, {2, 0}, {2, 1}}},
  // L
  {{{0, 2}, {1, 0}, {1, 1}, {1, 2}},
   {{0, 1}, {1, 1}, {2, 1}, {2, 2}},
   {{1, 0}, {1, 1}, {1, 2}, {2, 0}},
   {{0, 0}, {0, 1}, {1, 1}, {2, 1}}},
  // O
  {{{0, 1}, {0, 2}, {1, 1}, {1, 2}},
   {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
   {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
   {{0, 1}, {0, 2}, {1, 1}, {1, 2}}},
  // S
  {{{0, 1}, {0, 2}, {1, 0}, {1, 1}},
   {{0, 1}, {1, 1}, {1, 2}, {2, 2}},
   {{1, 1}, {1, 2}, {2, 0}, {2, 1}},
   {{0, 0}, {1, 0}, {1, 1}, {2, 1}}},
  // T
  {{{0, 1}, {1, 0}, {1, 1}, {1, 2}},
   {{0, 1}, {1, 1}, {1, 2}, {2, 1}},
   {{1, 0}, {1, 1}, {1, 2}, {2, 1}},
   {{0, 1}, {1, 0}, {1, 1}, {2, 1}}},
  // Z
  {{{0, 0}, {0, 1}, {1, 1}, {1, 2}},
   {{0, 2}, {1, 1}, {1, 2}, {2, 1}},
   {{1, 0}, {1, 1}, {2, 1}, {2, 2}},
   {{0, 1}, {1, 0}, {1, 1}, {2, 0}}},
};

// Keeping track of the game state
char board[20][10] = {0};
tet currentTet;
tet nextTet;
unsigned char score = 0;
unsigned char speed = 1;
unsigned int seed = 321;
int tick = 0;

// Interrupt handlers
void GPIOA_Handler(void) {
	const char bounceTolerance = 150;
	if ((GPIOA_RIS & 0x4) == 0x4) {
		DelayMs(bounceTolerance);
		GPIOA_ICR |= 0x4;
		rotateTet(&currentTet);
		seed++;
		refresh();
	} else if ((GPIOA_RIS & 0x8) == 0x8) {
		DelayMs(bounceTolerance);
		GPIOA_ICR |= 0x8;
		mvRight(&currentTet);
		seed++;
		refresh();
	} else if ((GPIOA_RIS & 0x10) == 0x10) {
		DelayMs(bounceTolerance);
		GPIOA_ICR |= 0x10;
		mvLeft(&currentTet);
		seed++;
		refresh();
	}
}

void SysTick_Handler(void)
{
	mvDown(&currentTet);
	refresh();
	if (downCollision(&currentTet)) {
		for (int i = 0; i < 4; i++) {
			gameLoc offset = tetrominos[currentTet.type][currentTet.orientation][i];
			gameLoc origin = currentTet.origin;
			board[origin.row + offset.row][origin.column + offset.column] = true;
		}
		if (tick == -1)
			while(1);
		tick = 1;
	}
}

/*
 * Game drawing functions for drawing the game border and the tetrominos
 */
void drawGameBorder(void)
{
	for (int x = 1 + 4; x <= X_MAX; x++) {
		pixel(x, 1, WHITE);
		pixel(x, Y_MAX, WHITE);
	}
	for (int y = 1; y <= Y_MAX; y++) {
		pixel(1 + 4, y, WHITE);
		pixel(X_MAX, y, WHITE);
	}
}

void drawSquare(gameLoc *loc, char colour)
{
	unsigned char x = loc->row * PIXEL_SCALE_FACTOR + 1;
	unsigned char y = loc->column * PIXEL_SCALE_FACTOR - 3;
	block(x, y, colour);
}

void drawTet(tet *t, char colour)
{
	for (char i = 0; i < 4; i++) {
		gameLoc offset = tetrominos[t->type][t->orientation][i];
		gameLoc draw;
		draw.column = t->origin.column + offset.column;
		draw.row = t->origin.row + offset.row;
		drawSquare(&draw, colour);
	}
}

/*
 * Tetromino Movement Functions
 * These are triggered on interrupt and not called manually.
 */
void rotateTet(tet *t)
{
	drawTet(t, BLACK);
	if (!rotCollision(t))
		t->orientation = (t->orientation + 1) % 4;
	drawTet(t, WHITE);
}

void mvLeft(tet *t)
{
	drawTet(t, BLACK);
	if (!leftCollision(t))
		t->origin.column++;
	drawTet(t, WHITE);
}

void mvRight(tet *t)
{
	drawTet(t, BLACK);
	if (!rightCollision(t))
		t->origin.column--;
	drawTet(t, WHITE);
}

void mvDown(tet *t)
{
	drawTet(t, BLACK);
	if (!downCollision(t))
		t->origin.row++;
	drawTet(t, WHITE);
}

void newTet(gameLoc origin, char orientation, SHAPE type, tet *t)
{
	t->type = type;
	t->origin = origin;
	t->orientation = orientation;
}

/*
 * Boundary and collision checking code
 */

char downCollision(tet *t)
{
	for (char i = 0; i < 4; i++) {
		gameLoc offset = tetrominos[t->type][t->orientation][i];
		gameLoc origin = t->origin;
		if (board[origin.row + offset.row + 1][origin.column + offset.column]) { //Checking against other blocks
			beep(50);
			return true;
		}
		if (origin.row + offset.row + 1 > ROW_MAX) { // Checking against game boundaries
			beep(50);
			return true;
		}
	}
	return false;
}

char leftCollision(tet *t)
{
	for (char i = 0; i < 4; i++) {
		gameLoc offset = tetrominos[t->type][t->orientation][i];
		gameLoc origin = t->origin;
		if (board[origin.row + offset.row][origin.column + offset.column + 1]) //Checking against other blocks
			return true;
		if (origin.column + offset.column + 1 > COL_MAX) // Checking against game boundaries
			return true;
	}
	return false;
}

char rightCollision(tet *t)
{
	for (char i = 0; i < 4; i++) {
		gameLoc offset = tetrominos[t->type][t->orientation][i];
		gameLoc origin = t->origin;
		if (board[origin.row + offset.row][origin.column + offset.column - 1]) //Checking against other blocks
			return true;
		if (origin.column + offset.column - 1 < COL_MIN) // Checking against game boundaries
			return true;
	}
	return false;
}

char rotCollision(tet *t)
{
	gameLoc orig = t->origin;
	for (int i = 0; i < 4; i++) {
		gameLoc offset = tetrominos[t->type][(t->orientation + 1) % 4][i];
		if (offset.column + orig.column > COL_MAX || offset.column + orig.column < COL_MIN) //Check out of bounds horizontally
			return 1;
		if (offset.row + orig.row > ROW_MAX || offset.row + orig.row < COL_MIN) // Check out of bounds vertically
			return 1;
		if (board[orig.row + offset.row][orig.column + offset.column]) // Check for other blocks
			return 1;
	}
	return 0;
}

/*
 * Goal check and board compress algorithms
 */

char clearGoal(void)
{
	char goalCount = 0;
	char start = 0;
	for (char i = ROW_MAX; i >= ROW_MIN; i--) {
		char goal = true;
		for(char j = COL_MIN; j <= COL_MAX; j++) {
			if (!board[i][j]) {
				goal = false;
				break; // Don't Clear the row
			}
		}
		if (goal) { // Clear it!
			goalCount++;
			if (i > start)
				start = i;
			for (char k = COL_MIN; k <= COL_MAX; k++) {
				board[i][k] = false;
				gameLoc clear;
				clear.row = i;
				clear.column = k;
				drawSquare(&clear, BLACK);
			}
		}
	}
	if (goalCount) {
		compress(goalCount, start);
		beep(500);
	}
	return goalCount;
}

void compress(char lines, char start)
{
	for (char i = start - lines; i >= ROW_MIN; i--) {
		for (char j = COL_MIN; j <= COL_MAX; j++) {
			if (board[i][j]) {
				board[i][j] = false;
				board[i + lines][j] = true;
				gameLoc move;
				move.row = i;
				move.column = j;
				drawSquare(&move, BLACK);
				move.row = i + lines;
				drawSquare(&move, WHITE);
			}
		}
	}
}

char isgameOver()
{
	char row = ROW_MIN;
	for (int col = COL_MIN; col <= COL_MAX; col++) {
		if (board[row][col])
			return true;
	}
	return false;
}

/*
 * Main game loop
 */

void gameLoop(void)
{
	while(1) {
		gameLoc origin = {5, 1};
		if (isgameOver()) {
			NVIC_ST_CTRL &= ~0x1;
			beep(1000);
			while(1);
		}
		if (score > 2)
			setupSystick(3);
		newTet(origin, 0, (SHAPE)(rand(&seed) % 7), &currentTet);
		drawTet(&currentTet, WHITE);
		score += clearGoal();
		refresh();
		tick = 0; // Set by systick interrupt
		while(!tick); // Wait for the rest of the tick
	}
}
