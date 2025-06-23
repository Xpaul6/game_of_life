#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ROWS 40
#define COLS 150
#define SPEED 100 // fraction of a second for one tick
#define SYM_DEAD "-"
#define SYM_ALIVE "#"

typedef enum {
    DEAD,
    ALIVE,
} State;

typedef struct {
   State currentState;
   State nextState; 
} Cell;

void init_grid(Cell grid[ROWS][COLS]) {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            grid[y][x].currentState = DEAD;
            grid[y][x].nextState = DEAD;
        }
    }
}

int print_grid(Cell grid[ROWS][COLS]) {
    int alive_counter = 0;
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            if (grid[y][x].currentState == DEAD) {
                printf(SYM_DEAD);
            } else {
                alive_counter++;
                printf(SYM_ALIVE);
            }
        }
        printf("\n");
    }
    return alive_counter;
}

void update_grid(Cell grid[ROWS][COLS]) {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            int neighbor_counter = 0;
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    if (dy == 0 && dx == 0) continue;
                    int row = (y + dy + ROWS) % ROWS;
                    int col = (x + dx + COLS) % COLS;
                    if (grid[row][col].currentState == ALIVE) {
                        neighbor_counter++;
                    }
                }
            }

            Cell currentCell = grid[y][x];
            if (currentCell.currentState == ALIVE) {
                if (neighbor_counter < 2) {
                    currentCell.nextState = DEAD;
                } else if (neighbor_counter < 4) {
                    currentCell.nextState = ALIVE;
                } else {
                    currentCell.nextState = DEAD;
                }
            } else {
                if (neighbor_counter == 3) {
                    currentCell.nextState = ALIVE;
                } else {
                    currentCell.nextState = DEAD;
                }
            }
        }
    }
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            grid[y][x].currentState = grid[y][x].nextState;
        }
    }
}

int main() {
    Cell grid[ROWS][COLS] = {0};
    init_grid(grid);

    // simple glider
    // grid[1][1].currentState = ALIVE;
    // grid[2][2].currentState = ALIVE;
    // grid[3][0].currentState = ALIVE;
    // grid[3][1].currentState = ALIVE;
    // grid[3][2].currentState = ALIVE;

    int c_row = ROWS / 2;
    int c_col = COLS / 2;
    grid[c_row][c_col].currentState = ALIVE;
    grid[c_row - 1][c_col].currentState = ALIVE;
    grid[c_row + 1][c_col].currentState = ALIVE;
    grid[c_row][c_col - 1].currentState = ALIVE;
    grid[c_row + 1][c_col + 1].currentState = ALIVE;

    while (print_grid(grid) != 0) {
        usleep(1000000 / SPEED);
        system("clear");
        update_grid(grid);
    }

    return 0;
}