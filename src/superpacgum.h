//
// Created by 33767 on 19/01/2025.
//

#ifndef SUPERPACGUM_H
#define SUPERPACGUM_H
#include "framework.h"
#include "pacman.h"
#include "spirits.h"

#endif //SUPERPACGUM_H

bool eatPacgum(char **level, Pacman *pacman);

void resetGhost(char **level, const int row, const int col, MovingEntity * ghost, Textures *textures);

void resetTextureBase(MovingEntity * ghost, Textures *textures);

void superPacgumMode(char **level, RendererParameters *params, Textures *textures, MovingEntity **fourGhosts, int nbGhost);