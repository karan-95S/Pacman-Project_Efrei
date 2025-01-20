#include "main.h"

#include <stdio.h>

#include "firstLevel.h"

#include  "pacman.h"
#include "spirits.h"
#include "superpacgum.h"

int main(int argc, char* argv[]) {
    RendererParameters *params = malloc(sizeof(RendererParameters));
    Textures *textures = malloc(sizeof(Textures));
    initWindowed(params, textures, 31, 28);

    struct Textures structTexture;
    update(params);
    structTexture.texturePacman = GetTexture("pacman.bmp", params);
    structTexture.textureClyde = GetTexture("clyde.bmp", params);
    structTexture.texturePinky = GetTexture("pinky.bmp", params);
    structTexture.textureInky = GetTexture("inky.bmp", params);
    structTexture.textureBlinky = GetTexture("blinky.bmp", params);
    structTexture.textureBLue = GetTexture("blue.bmp", params);

    int newX = 0, newY = 0;  // Direction souhaitée

    char **level;
    const int row = 31;
    const int col = 28;
    loadFirstLevel(&level);

    // Initialisation de Pacman
    Pacman pacman;
    initPacman(level, row, col, &pacman);
    pacman.pacmanAngle = 0;

    //Initialiser les fantômes (clyde en premier)
    MovingEntity clyde;
    initGhost(level, row, col, &clyde, CLYDE, structTexture.textureClyde);

    MovingEntity pinky;
    initGhost(level, row, col, &pinky, PINKY, structTexture.texturePinky);

    MovingEntity inky;
    initGhost(level, row, col, &inky, INKY, structTexture.textureInky);

    MovingEntity blinky;
    initGhost(level, row, col, &blinky, BLINKY, structTexture.textureBlinky);

    Uint32 lastMoveTime = 0, moveDelay = 100;  // Contrôle du temps entre les déplacements
    bool play = true;

    int pacmanSpeed = 0;
    int ghostSpeed = 0;

    int nbGhost =4;
    MovingEntity *fourGhosts[4] = {&clyde, &pinky, &inky, &blinky};

    Uint32 timerSuperPacgum = 0;
    bool superPacgumLaunch = false;

    while (play) {
        Uint32 currentTime = SDL_GetTicks();

        SDL_RenderClear(params->renderer);
        drawLevel(level, row, col, params, textures);
        drawSpriteOnGrid(structTexture.texturePacman, pacman.pacmanX, pacman.pacmanY, pacman.pacmanAngle, params);
        drawSpriteOnGrid(clyde.texture, clyde.pos.x, clyde.pos.y, 0, params);
        drawSpriteOnGrid(pinky.texture, pinky.pos.x, pinky.pos.y, 0, params);
        drawSpriteOnGrid(inky.texture, inky.pos.x, inky.pos.y, 0, params);
        drawSpriteOnGrid(blinky.texture, blinky.pos.x, blinky.pos.y, 0, params);

        SDL_RenderPresent(params->renderer);

        // Gérer l'entrée utilisateur
        switch (getInput()) {
            case SDLK_DOWN:
                newX = 0; newY = 1;
                break;
            case SDLK_UP:
                newX = 0; newY = -1;
                break;
            case SDLK_LEFT:
                newX = -1; newY = 0;
                break;
            case SDLK_RIGHT:
                newX = 1; newY = 0;
                break;
            case SDLK_ESCAPE:
                play = false;
                break;
        }

        // Applique la direction souhaitée si elle est valide
        if (verifWall(level, pacman.pacmanX + newX, pacman.pacmanY + newY)) {
            pacman.dirX = newX;
            pacman.dirY = newY;
        }

        if (currentTime - lastMoveTime >= moveDelay) {
            if (pacmanSpeed >= PACMAN_SPEED) {
                directionPacman(level, &pacman, col, row);
                pacmanSpeed =0;
            }else {
                pacmanSpeed++;
            }
            if (ghostSpeed >= GHOST_SPEED) {
                ghostMove(level, &clyde, &pacman);
                ghostMove(level, &pinky, &pacman);
                ghostMove(level, &inky, &pacman);
                ghostMove(level, &blinky, &pacman);
                ghostSpeed =0;
            }else {
                ghostSpeed++;
            }

            lastMoveTime = currentTime;
        }

        if(!superPacgumLaunch) {
            if (verifKillPacman(&clyde, &pacman) || verifKillPacman(&pinky, &pacman) || verifKillPacman(&inky, &pacman) || verifKillPacman(&blinky, &pacman)) {
                printf("\nTu as Perdu ! RIP !!!\n");
                break;
            }
        }

        if(eatPacgum(level, &pacman)) {
            superPacgumLaunch = true;
            timerSuperPacgum = SDL_GetTicks();
            superPacgumMode(level, params, &structTexture, fourGhosts, nbGhost);

        }

        if(superPacgumLaunch) {
            for(int i= 0; i<nbGhost; i++) {
                if(verifKillPacman(fourGhosts[i], &pacman)) {
                    if (fourGhosts[i]->texture == structTexture.textureBLue) {
                        resetGhost(level, row, col, fourGhosts[i], &structTexture);
                    }

                }
            }
            if (verifKillPacman(&clyde, &pacman) || verifKillPacman(&pinky, &pacman) || verifKillPacman(&inky, &pacman) || verifKillPacman(&blinky, &pacman)) {
                if ((clyde.texture != structTexture.textureBLue) || (pinky.texture != structTexture.textureBLue)  || (inky.texture != structTexture.textureBLue) || (blinky.texture != structTexture.textureBLue)) {
                    printf("\nTu as Perdu ! RIP !!!\n");
                    break;
                }
            }
        }


        if (SDL_GetTicks() - timerSuperPacgum >= 10000) {
            for (int i = 0; i < nbGhost; i++) {
                resetTextureBase(fourGhosts[i], &structTexture);
            }
            superPacgumLaunch = false;
        }


        if (win(level)) {
            printf("Victoire ! T'es un GOAT !\n");
            break;
        }
    }

    // Libération des ressources
    SDL_DestroyTexture(structTexture.texturePacman);
    SDL_DestroyRenderer(params->renderer);
    SDL_DestroyWindow(params->window);
    SDL_Quit();
    free(level);

    return 0;
}
