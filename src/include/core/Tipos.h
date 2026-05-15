#pragma once

#include <stdbool.h>
#include "raylib/raylib.h"

typedef enum EstadoJogo {
    ESTADO_JOGO_MENU,
    ESTADO_JOGO_WORLD_MAP,
    ESTADO_JOGO_GAMEPLAY,
    ESTADO_JOGO_PAUSE,
    ESTADO_JOGO_GAME_OVER

} EstadoJogo;

typedef struct Jogador {
    Rectangle ret;
    Vector2 vel;
    Color cor;
    float velAndando;
    float velPulo;
    float velMaxQueda;
} Jogador;

typedef struct GameWorld {
    Jogador *jogador;
    float gravidade;
    EstadoJogo estadoJogo;
}GameWorld;

