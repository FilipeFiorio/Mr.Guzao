#include <stdlib.h>
#include "raylib/raylib.h"
#include "core/Tipos.h"

Jogador *criarJogador(float x, float y, float largura, float altura, Color cor) {

    Jogador *novoJogador = (Jogador*) malloc(sizeof(Jogador));

    novoJogador->ret = (Rectangle) {
        .x = x,
        .y = y,
        .width = largura,
        .height = altura,
    };
    novoJogador->vel = (Vector2) {0};
    novoJogador->cor = BLUE;

    novoJogador->velAndando = 300;
    novoJogador->velPulo = -500;
    novoJogador->velMaxQueda = 600;

    return novoJogador;
}

void destruirJogador(Jogador *j) {
    if(j != NULL) {
        free(j);
    }
}

void atualizarJogador(Jogador *j, GameWorld *gw, float delta) {

}

void desenharJogador(Jogador *j) {
    DrawRectangleRec(j->ret, j->cor);
}