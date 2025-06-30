#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define ESCAPE_CODE "\033[H\033[J"

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

void init_grid(Cell** grid, int COLS, int ROWS) {
    for (int i = 0; i < ROWS; i++) {
        grid[i] = malloc(COLS * sizeof(Cell));
    }
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            grid[y][x].currentState = DEAD;
            grid[y][x].nextState = DEAD;
        }
    }
}

void print_grid(Cell** grid, int COLS, int ROWS) {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            if (grid[y][x].currentState == DEAD) {
                printf(SYM_DEAD);
            } else {
                printf(SYM_ALIVE);
            }
        }
        printf("\n");
    }
}

void update_grid(Cell** grid, int COLS, int ROWS, int *alive_counter) {
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

            // Game rules
            if (grid[y][x].currentState == ALIVE) {
                if (neighbor_counter < 2) {
                    grid[y][x].nextState = DEAD;
                } else if (neighbor_counter < 4) {
                    grid[y][x].nextState = ALIVE;
                } else {
                    grid[y][x].nextState = DEAD;
                }
            } else {
                if (neighbor_counter == 3) {
                    grid[y][x].nextState = ALIVE;
                } else {
                    grid[y][x].nextState = DEAD;
                }
            }
        }
    }
    int local_alive_counter = 0;
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            grid[y][x].currentState = grid[y][x].nextState;
            if (grid[y][x].currentState == ALIVE) local_alive_counter++;
        }
    }
    *alive_counter = local_alive_counter;
}

void draw_acorn(Cell** grid, int COLS, int ROWS);
void draw_glider(Cell** grid, int COLS, int ROWS); 
void draw_r_pentomino(Cell** grid, int COLS, int ROWS);
void draw_diehart(Cell** grid, int COLS, int ROWS);

// entry point
int main(int argc, char** argv) {
    int ROWS = 35;
    int COLS = 130;
    int SPEED = 10;

    if (argc == 5) {
        COLS = strtol(argv[1], NULL, 10);
        ROWS = strtol(argv[2], NULL, 10);
        SPEED = strtol(argv[3], NULL, 10);
        if (COLS < 10 || ROWS < 10 || SPEED < 1) {
            printf("Minimal grid size is 10x10, minimal speed is 1 tick/s\nExiting...\n");
            return -1;
        }
    }

    Cell** grid = (Cell**)malloc(ROWS * COLS);

    init_grid(grid, COLS, ROWS);

    if (!strcmp(argv[4], "glider")) {
        draw_glider(grid, COLS, ROWS);
    } else if (!strcmp(argv[4], "diehart")) {
        draw_diehart(grid, COLS, ROWS);
    } else if (!strcmp(argv[4], "r-pentomino")) {
        draw_r_pentomino(grid, COLS, ROWS);
    } else {
        draw_acorn(grid, COLS, ROWS);
    }

    int generation_counter = 1;
    int alive_counter = -1;
    
    printf(ESCAPE_CODE);

    while (alive_counter != 0) {
        printf("Generation: %d\n", generation_counter);
        print_grid(grid, COLS, ROWS);

        usleep(1000000 / SPEED);
        printf(ESCAPE_CODE);

        update_grid(grid, COLS, ROWS, &alive_counter);
        generation_counter++;
    }

    return 0;
}

void draw_acorn(Cell** grid, int COLS, int ROWS) {
    int c_row = ROWS / 2;
    int c_col = COLS / 2;

    grid[c_row][c_col].currentState = ALIVE;
    grid[c_row + 1][c_col + 1].currentState = ALIVE;
    grid[c_row + 1][c_col + 2].currentState = ALIVE;
    grid[c_row + 1][c_col + 3].currentState = ALIVE;
    grid[c_row - 1][c_col - 2].currentState = ALIVE;
    grid[c_row + 1][c_col - 2].currentState = ALIVE;
    grid[c_row + 1][c_col - 3].currentState = ALIVE;
}

void draw_glider(Cell** grid, int COLS, int ROWS) {
    grid[1][1].currentState = ALIVE;
    grid[2][2].currentState = ALIVE;
    grid[3][0].currentState = ALIVE;
    grid[3][1].currentState = ALIVE;
    grid[3][2].currentState = ALIVE;
}

void draw_r_pentomino(Cell** grid, int COLS, int ROWS) {
    int c_row = ROWS / 2;
    int c_col = COLS / 2;
    grid[c_row][c_col].currentState = ALIVE;
    grid[c_row - 1][c_col].currentState = ALIVE;
    grid[c_row + 1][c_col].currentState = ALIVE;
    grid[c_row][c_col - 1].currentState = ALIVE;
    grid[c_row + 1][c_col + 1].currentState = ALIVE;

}

void draw_diehart(Cell** grid, int COLS, int ROWS) {
    int c_row = ROWS / 2;
    int c_col = COLS / 2;

    grid[c_row][c_col - 3].currentState = ALIVE;
    grid[c_row][c_col - 2].currentState = ALIVE;
    grid[c_row + 1][c_col - 3].currentState = ALIVE;
    grid[c_row - 1][c_col + 3].currentState = ALIVE;
    grid[c_row + 1][c_col + 2].currentState = ALIVE;
    grid[c_row + 1][c_col + 3].currentState = ALIVE;
    grid[c_row + 1][c_col + 4].currentState = ALIVE;
}
