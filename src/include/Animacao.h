#pragma once

#include "raylib/raylib.h"
#include "Tipos.h"

void criarQuadroAnimacao(Animacao *animacao, int quantidadeQuadros);
void inicializarQuadroAnimacao(
    QuadroAnimacao *quadros,
    int quantidadeQuadros,
    int duracaoPadrao,
    int xInicioFonte,
    int yInicioFonte,
    int larguraFonte,
    int alturaFonte,
    bool aoContrario,
    int espacamentoFonte
);
void destruirQuadroAnimacao(Animacao *animacao);
void atualizarAnimacao(Animacao *animacao, float delta);
int getNumeroQuadroAtualAnimacao(Animacao *animacao);
QuadroAnimacao *getQuadroAtualAnimacao(Animacao *animacao);
void reiniciarAnimacao(Animacao *animacao);
void sincronizarAnimacao(Animacao *destino, Animacao *origem);