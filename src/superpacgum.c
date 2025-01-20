//
// Created by 33767 on 19/01/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include "superpacgum.h"

#include "framework.h"
#include "pacman.h"
#include "spirits.h"

bool eatPacgum(char **level, Pacman *pacman) {
    if (level[pacman->pacmanY][pacman->pacmanX] == 'G') {
        level[pacman->pacmanY][pacman->pacmanX] = ' ';
        return true;
    }
    return false;
}
void resetGhost(char **level, const int row, const int col, MovingEntity * ghost, Textures *textures) {
    char spirit = getGhostChar(ghost->name);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (level[i][j] == spirit) {
                ghost->pos.x = j;
                ghost->pos.y = i;
                resetTextureBase(ghost, textures);
            }
        }
    }
}

void resetTextureBase(MovingEntity * ghost, Textures *textures) {
    switch (ghost->name) {
        case CLYDE:
            ghost->texture = textures->textureClyde;
        break;
        case PINKY:
            ghost->texture = textures->texturePinky;
        break;
        case INKY:
            ghost->texture = textures->textureInky;
        break;
        case BLINKY:
            ghost->texture = textures->textureBlinky;
        break;
        default:
            break;
    }
}


void superPacgumMode(char **level, RendererParameters *params, Textures *textures, MovingEntity **fourGhosts, int nbGhost) {
    for (int i = 0; i < nbGhost; i++) {
        fourGhosts[i]->texture = textures->textureBLue;
    }
    int nbDir = 0;
    for (int i = 0; i < nbGhost; i++) {

        fourGhosts[i]->dir.x = -fourGhosts[i]->dir.x;
        fourGhosts[i]->dir.y = -fourGhosts[i]->dir.y;

        Coord *directions = getPotentialDirection(level, fourGhosts[i], &nbDir);

        clydeMove(level, fourGhosts[i], directions, nbDir);
    }
}

