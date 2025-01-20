//
// Created by 33767 on 13/12/2024.
//

#ifndef PACMAN_H
#define PACMAN_H

#define PACMAN_SPEED 1

typedef struct Pacman Pacman;
typedef struct Pacman{
    int pacmanX;
    int pacmanY;
    int dirX;
    int dirY;
    int pacmanAngle;
};

void initPacman(char **level, const int row, const int col, Pacman *pacman);
void eat(char ** level, int x, int y);

void move(char **level, int in, int pacmanX, int pacmanY, int pacmanAngle, bool play, int col, int row);

bool verifWall(char **level, int x, int y);

void directionPacman(char **level, Pacman *pacman, int col, int row);

bool win(const char **level);
#endif //PACMAN_H

