//
// Created by 33767 on 13/12/2024.
//
#include <stdio.h>

#include "pacman.h"

#include <SDL_keycode.h>

#include "framework.h"
#include "superpacgum.h"

void initPacman(char **level, const int row, const int col, Pacman *pacman) {

    // Initialisation de Pacman
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (level[i][j] == 'O') {
                pacman->pacmanX = j;
                pacman->pacmanY = i;
                level[i][j] = ' ';
            }
        }
    }

}

void eat(char **level, int x, int y) {
    if (level[y][x] == '.') {
        level[y][x] = ' ';
    }
}

bool win(const char **level) {

    for (int i = 0; i < 31; i++) {
        for (int j = 0; j < 28; j++) {
            if (level[i][j] == '.') {
                return false;
            }
        }
    }
    return true;
}

bool verifWall(char **level, int x, int y) {
    return level[y][x] != 'H';
}

void directionPacman(char **level, Pacman *pacman, int col, int row) {

    if (verifWall(level, pacman->pacmanX + pacman->dirX, pacman->pacmanY + pacman->dirY)) {
        pacman->pacmanX += pacman->dirX;
        pacman->pacmanY += pacman->dirY;
    }


    if (pacman->pacmanX < 0) pacman->pacmanX = col - 1;
    else if (pacman->pacmanX >= col) pacman->pacmanX = 0;

    if (pacman->pacmanY < 0) pacman->pacmanY = row - 1;
    else if (pacman->pacmanY >= row) pacman->pacmanY = 0;

    if (pacman->dirX == 1) pacman->pacmanAngle = 180;
    else if (pacman->dirX == -1) pacman->pacmanAngle = 0;
    else if (pacman->dirY == 1) pacman->pacmanAngle = 270;
    else if (pacman->dirY == -1) pacman->pacmanAngle = 90;

    eat(level, pacman->pacmanX, pacman->pacmanY);

}
