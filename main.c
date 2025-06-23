#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ROWS 30
#define COLS 100
#define SPEED 20
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
            if (neighbor_counter <= 1 && grid[y][x].currentState == ALIVE) {
                grid[y][x].nextState = DEAD;
            } else if (neighbor_counter <= 3 && grid[y][x].currentState == ALIVE) {
                grid[y][x].nextState = ALIVE;
            } else if (neighbor_counter <= 8 && grid[y][x].currentState == ALIVE) {
                grid[y][x].nextState = DEAD;
            } else if (neighbor_counter == 3 && grid[y][x].currentState == DEAD) {
                grid[y][x].nextState = ALIVE;
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

    grid[15][50].currentState = ALIVE;
    grid[14][50].currentState = ALIVE;
    grid[16][50].currentState = ALIVE;
    grid[15][49].currentState = ALIVE;
    grid[16][51].currentState = ALIVE;
    while (print_grid(grid) != 0) {
        usleep(SPEED * 1000);
        system("clear");
        update_grid(grid);
    }

    return 0;
}