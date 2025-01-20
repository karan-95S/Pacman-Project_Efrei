//
// Created by 33767 on 03/01/2025.
//

#ifndef SPIRITS_H
#define SPIRITS_H
#include <SDL_render.h>

#include "pacman.h"

#define INKY_RANDOMNESS 2
#define GHOST_SPEED 2
#define BLINKY_RANDOMNESS 5


typedef struct Coord Coord;
struct Coord
{
    int x;
    int y;
};

typedef enum EntityName EntityName;
enum EntityName
{
    BLINKY,
    CLYDE,
    INKY,
    PINKY
};

typedef struct MovingEntity MovingEntity;
struct MovingEntity
{
    Coord pos;
    Coord dir;
    EntityName name;
    SDL_Texture * texture;
};


char getGhostChar(EntityName name);

void initGhost(char **level, const int row, const int col, MovingEntity * ghost, EntityName name, SDL_Texture *texture);


void clydeMove(const char **level, MovingEntity *ghost, Coord *directions, int nbDir);

bool checkPacmanOnLine(const char **level, const MovingEntity *ghost, const Pacman *pacman);

void pinkyMove(const char **level, MovingEntity *ghost, const Pacman *pacman, Coord *directions, int nbDir);

void inkyMove(const char **level, MovingEntity *ghost, const Pacman *pacman, Coord *directions, int nbDir);

void ghostMove(const char **level, MovingEntity *ghost, Pacman *pacman);

bool verifKillPacman(const MovingEntity *entity1, const Pacman *pacman);

void blinkyMove(const char **level, MovingEntity *ghost, const Pacman *pacman, Coord *directions, int nbDir);

Coord getPacmanNextIntersec(const char **level, Pacman pacman);

Coord * getPotentialDirection(const char ** level, const MovingEntity * ghost, int * nbDir);
#endif //SPIRITS_H
