// Created by 33767 on 03/01/2025.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "spirits.h"

#include <time.h>

char getGhostChar(EntityName name) {
    switch (name) {
        case CLYDE: return 'C';
        case PINKY: return 'P';
        case BLINKY: return 'B';
        case INKY: return 'I';
        default: return ' '; // Valeur par défaut en cas d'erreur
    }
}


void initGhost(char **level, const int row, const int col, MovingEntity * ghost, EntityName name, SDL_Texture *texture) {
    ghost->name = name;
    char spirit = getGhostChar(ghost->name);
    ghost->texture = texture;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (level[i][j] == spirit) {
                ghost->pos.x = j;
                ghost->pos.y = i;
            }
        }
    }
}


Coord * getPotentialDirection(const char ** level, const MovingEntity * ghost, int * nbDir) {

    int x = ghost->pos.x;
    int y = ghost->pos.y;

    int dirX = ghost->dir.x;
    int dirY = ghost->dir.y;

    Coord directions[] = {
        {0, -1},
        {0, 1},
        {-1, 0},
        {1, 0}
    };

    *nbDir = 0;
    Coord *possibleDirections = NULL;

    for (int i = 0; i < 4; i++) {
        int newX = x + directions[i].x;
        int newY = y + directions[i].y;

        if ((verifWall(level, newX, newY)) && !(newX == x - dirX && newY == y - dirY)) {
            (*nbDir)++;
            possibleDirections = realloc(possibleDirections, (*nbDir) * sizeof(Coord));
            possibleDirections[*nbDir - 1] = (Coord){newX, newY};
        }
    }
    return possibleDirections;
}

Coord getPacmanNextIntersec(const char **level, Pacman pacman) {


    int possibleDirectionsPacman = 0;
    if (verifWall(level, pacman.pacmanX + 1, pacman.pacmanY)) possibleDirectionsPacman++;
    if (verifWall(level, pacman.pacmanX - 1, pacman.pacmanY)) possibleDirectionsPacman++;
    if (verifWall(level, pacman.pacmanX, pacman.pacmanY + 1)) possibleDirectionsPacman++;
    if (verifWall(level, pacman.pacmanX, pacman.pacmanY - 1)) possibleDirectionsPacman++;

    if (possibleDirectionsPacman > 2) {
        return (Coord){pacman.pacmanX, pacman.pacmanY};
    }

    if (pacman.dirX == 0 && pacman.dirY == 0) {
        return (Coord){pacman.pacmanX, pacman.pacmanY};
    }

    pacman.pacmanX += pacman.dirX;
    pacman.pacmanY += pacman.dirY;

    if (!verifWall(level, pacman.pacmanX, pacman.pacmanY)) {
        return (Coord){pacman.pacmanX, pacman.pacmanY};
    }

    return getPacmanNextIntersec(level, pacman);
}

void blinkyMove(const char **level, MovingEntity *ghost, const Pacman *pacman, Coord *directions, int nbDir) {


    Coord DirectionsCiblePacman = getPacmanNextIntersec(level, *pacman);
    MovingEntity blinkyFutur = *ghost;
    int directionGhostToPacmanX = DirectionsCiblePacman.x;
    int directionGhostToPacmanY = DirectionsCiblePacman.y;

    srand((unsigned int)time(NULL));
    if (rand() % BLINKY_RANDOMNESS == 0) {
        clydeMove(level, ghost, directions, nbDir);
    }

    blinkyFutur.pos.x = directionGhostToPacmanX;
    blinkyFutur.pos.y = directionGhostToPacmanY;

    if(verifWall(level, ghost->pos.x, ghost->pos.y)) {
        inkyMove(level, ghost, &blinkyFutur, directions, nbDir);
    }

}



void clydeMove(const char **level, MovingEntity *ghost, Coord *directions, int nbDir) {
    if (nbDir == 0) {
        ghost->dir.x = -ghost->dir.x;
        ghost->dir.y = -ghost->dir.y;
    } else if (nbDir == 1) {
        ghost->dir.x = directions[0].x - ghost->pos.x;
        ghost->dir.y = directions[0].y - ghost->pos.y;
    } else {
        srand(time(NULL));
        int randomIndex = rand() % nbDir;
        ghost->dir.x = directions[randomIndex].x - ghost->pos.x;
        ghost->dir.y = directions[randomIndex].y - ghost->pos.y;
    }
}

bool checkPacmanOnLine(const char **level, const MovingEntity *ghost, const Pacman *pacman) {

    if (ghost->pos.y == pacman->pacmanY) {
        int startX;
        int endX;

        if (ghost->pos.x < pacman->pacmanX) {
            startX = ghost->pos.x + 1;
        } else {
            startX = pacman->pacmanX + 1;
        }

        if (ghost->pos.x > pacman->pacmanX) {
            endX = ghost->pos.x;
        } else {
            endX = pacman->pacmanX;
        }

        for (int x = startX; x < endX; x++) {
            if (level[ghost->pos.y][x] == 'H') return false;
        }
        return true;
    }

    if (ghost->pos.x == pacman->pacmanX) {
        int startY;
        int endY;

        if (ghost->pos.y < pacman->pacmanY) {
            startY = ghost->pos.y + 1;
        } else {
            startY = pacman->pacmanY + 1;
        }

        if (ghost->pos.y > pacman->pacmanY) {
            endY = ghost->pos.y;
        } else {
            endY = pacman->pacmanY;
        }

        for (int y = startY; y < endY; y++) {
            if (level[y][ghost->pos.x] == 'H') return false;
        }
        return true;
    }

    return false;
}


void pinkyMove(const char **level, MovingEntity *ghost, const Pacman *pacman, Coord *directions, int nbDir) {
    if (checkPacmanOnLine(level, ghost, pacman)) {
        int pacmanDirX = (pacman->pacmanX > ghost->pos.x) - (pacman->pacmanX < ghost->pos.x);
        int pacmanDirY = (pacman->pacmanY > ghost->pos.y) - (pacman->pacmanY < ghost->pos.y);


        if (verifWall(level, ghost->pos.x + pacmanDirX, ghost->pos.y + pacmanDirY)) {
            if (!(pacmanDirX == -ghost->dir.x && pacmanDirY == -ghost->dir.y)) {
                ghost->dir.x = pacmanDirX;
                ghost->dir.y = pacmanDirY;
                return;
            }
        }
    }

    clydeMove(level, ghost, directions, nbDir);
}

void inkyMove(const char **level, MovingEntity *ghost, const Pacman *pacman, Coord *directions, int nbDir) {
    int diffX = pacman->pacmanX - ghost->pos.x;
    int diffY = pacman->pacmanY - ghost->pos.y;

    if (abs(diffX) > abs(diffY)) {
        if (diffX > 0 && ghost->dir.x != -1 && verifWall(level, ghost->pos.x + 1, ghost->pos.y)) {
            ghost->dir.x = 1;
            ghost->dir.y = 0;
        } else if (diffX < 0 && ghost->dir.x != 1 && verifWall(level, ghost->pos.x - 1, ghost->pos.y)) {
            ghost->dir.x = -1;
            ghost->dir.y = 0;
        } else {
            if (diffY > 0 && ghost->dir.y != -1 && verifWall(level, ghost->pos.x, ghost->pos.y + 1)) {
                ghost->dir.x = 0;
                ghost->dir.y = 1;
            } else if (diffY < 0 && ghost->dir.y != 1 && verifWall(level, ghost->pos.x, ghost->pos.y - 1)) {
                ghost->dir.x = 0;
                ghost->dir.y = -1;
            } else {
                clydeMove(level, ghost, directions, nbDir);
            }
        }
    } else {

        if (diffY > 0 && ghost->dir.y != -1 && verifWall(level, ghost->pos.x, ghost->pos.y + 1)) {
            ghost->dir.x = 0;
            ghost->dir.y = 1;
        } else if (diffY < 0 && ghost->dir.y != 1 && verifWall(level, ghost->pos.x, ghost->pos.y - 1)) {
            ghost->dir.x = 0;
            ghost->dir.y = -1;
        } else {

            if (diffX > 0 && ghost->dir.x != -1 && verifWall(level, ghost->pos.x + 1, ghost->pos.y)) {
                ghost->dir.x = 1;
                ghost->dir.y = 0;
            } else if (diffX < 0 && ghost->dir.x != 1 && verifWall(level, ghost->pos.x - 1, ghost->pos.y)) {
                ghost->dir.x = -1;
                ghost->dir.y = 0;
            } else {

                clydeMove(level, ghost, directions, nbDir);
            }
        }
    }
}


void specificGhostMovement(const char **level, MovingEntity *ghost, Pacman *pacman, Coord *directions, int nbDir) {
    switch (ghost->name) {
        case CLYDE:
            clydeMove(level, ghost, directions, nbDir);
        break;
        case PINKY:
            pinkyMove(level, ghost, pacman, directions, nbDir);
        break;
        case INKY:
            inkyMove(level, ghost, pacman, directions, nbDir);
        break;
        case BLINKY:
            blinkyMove(level, ghost, pacman, directions, nbDir);
        break;
        default:
            break;
    }
}


bool verifKillPacman(const MovingEntity *ghost, const Pacman *pacman) {

    int ghostDirX = ghost->pos.x + ghost->dir.x;
    int ghostDirY = ghost->pos.y + ghost->dir.y;

    int pacmanDirX = pacman->pacmanX + pacman->dirX;
    int pacmanDirY = pacman->pacmanY + pacman->dirY;

    if (ghost->pos.x == pacman->pacmanX && ghost->pos.y == pacman->pacmanY) {
        return true;
    }

    if (ghostDirX == pacman->pacmanX && ghostDirY == pacman->pacmanY &&
        pacmanDirX == ghost->pos.x && pacmanDirY == ghost->pos.y) {
        return true;
        }

    return false;
}


void ghostMove(const char **level, MovingEntity *ghost, Pacman *pacman) {
    int row = 31;
    int col = 28;
    int nbDir = 0;
    Coord *directions = getPotentialDirection(level, ghost, &nbDir);

    if (nbDir == 0) {
        ghost->dir.x = -ghost->dir.x;
        ghost->dir.y = -ghost->dir.y;
    } else {
        specificGhostMovement(level, ghost, pacman, directions, nbDir);
    }

    ghost->pos.x += ghost->dir.x;
    ghost->pos.y += ghost->dir.y;

    if (ghost->pos.x < 0) {
        ghost->pos.x = col - 1;
    } else if (ghost->pos.x >= col) {
        ghost->pos.x = 0;
    }

    free(directions);
}