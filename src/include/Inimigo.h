#pragma once

#include "Tipos.h"

Inimigo *criarInimigo(float x, float y, float largura, float altura, Color cor);
void atualizarInimigo(Inimigo *inimigo, GameWorld *gw, float delta);
void destruirInimigo(Inimigo *inimigo);
void desenharInimigo(Inimigo *inimigo);